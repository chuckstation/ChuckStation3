#include "PPUDebuggerWidget.hpp"
#include "DisabledWidgetOverlay.hpp"


static int getLinesInViewport(QListWidget* list_widget) {
    int viewport_height = list_widget->viewport()->height();
    QFontMetrics fm = QFontMetrics(list_widget->font());
    int line_height = fm.height();

    int visible_lines = viewport_height / line_height;
    return visible_lines;
}

static std::pair<int, int> getVisibleLineRange(QListWidget* list_widget, QScrollBar* scroll_bar) {
    int first_line = scroll_bar->value();
    int visible_lines = getLinesInViewport(list_widget);
    
    return {first_line, visible_lines};
}

PPUDebuggerWidget::PPUDebuggerWidget(PlayStation3* ps3, GameWindow* game_window, QWidget* parent) : QWidget(parent, Qt::Window), ps3(ps3), game_window(game_window) {
    ui.setupUi(this);

    // Setup disabled widget overlay
    disabled_overlay = new DisabledWidgetOverlay(this, tr("Pause the emulator to use the PPU Debugger"));
    disabled_overlay->resize(size());   // Fill the whole screen
    disabled_overlay->raise();
    disabled_overlay->hide();
    
    // Monospace font
    QFont mono_font = QFont("Courier New");
    mono_font.setStyleHint(QFont::Monospace);
    ui.disasmListWidget->setFont(mono_font);
    ui.registerTextEdit->setFont(mono_font);
    ui.watchpointsListWidget->setFont(mono_font);
    
    // To forward scrolling from the list widget to the external scrollbar
    ui.disasmListWidget->installEventFilter(this);
    
    // Setup scroll bar
    ui.verticalScrollBar->setRange(0, INT32_MAX);
    ui.verticalScrollBar->setPageStep(getLinesInViewport(ui.disasmListWidget));
    ui.verticalScrollBar->hide();
    connect(ui.verticalScrollBar, &QScrollBar::valueChanged, this, &PPUDebuggerWidget::updateDisasm);
    
    ui.registerTextEdit->setReadOnly(true);
    
    // Setup buttons
    connect(ui.stepButton, &QPushButton::clicked, this, [this]() {
        this->game_window->pause_sema.release();
        // Wait for the game thread to complete the emulator step & reset the flag
        while (!this->game_window->stepped);
        this->game_window->stepped = false;
        // Update disasm & register view
        updateDisasm();
        updateRegisters();
        scrollToPC();
    });
    
    connect(ui.goToAddressButton, &QPushButton::clicked, this, [this]() {
        bool ok;
        QString addr_text = QInputDialog::getText(this, tr("Go to address"), tr("Enter hexadecimal address:"), QLineEdit::Normal, "", &ok);
        if (ok) {
            u32 addr = addr_text.toUInt(&ok, 16);
            if (ok) scrollToAddress(addr);
            else QMessageBox::critical(this, tr("Invalid address"), tr("Invalid hexadecimal address"));
        }
    });
    
    connect(ui.goToPcButton, &QPushButton::clicked, this, [this]() {
        scrollToPC();
    });
    
    // Watchpoints
    connect(ui.addMemoryWatchpointButton, &QPushButton::clicked, this, [this]() {
        // This is so we can format our indices as 3 digits (0-999)
        // Nobody will ever need that many watchpoints anyway
        if (mem_watchpoints.size() >= 1000) {
            QMessageBox::critical(this, tr("Failed to set watchpoint"), tr("Can't set more than 1000 watchpoints"));
            return;
        }
        
        MemoryWatchpointDialog dialog;
        if (dialog.exec() == QDialog::Accepted) {
            MemoryWatchpoint watchpoint = { dialog.addr, dialog.type };
            
            bool is_read_watchpoint;
            auto watchpoints = getWatchpointList(watchpoint, is_read_watchpoint);
                
            // Error if a watchpoint is already set for this address
            if (watchpoints->contains(watchpoint.addr)) {
                QMessageBox::critical(this, tr("Failed to set watchpoint"), tr("A watchpoint was already set on this address"));
                return;
            }
            
            // Set the watchpoint
            mem_watchpoints.push_back(watchpoint);
            updateWatchpoints();
            (*watchpoints)[watchpoint.addr] = std::bind(&GameWindow::breakOnNextInstr, this->game_window, std::placeholders::_1);
            this->ps3->mem.markAsSlowMem(watchpoint.addr >> PAGE_SHIFT, is_read_watchpoint, !is_read_watchpoint);
        }
    });
    
    connect(ui.watchpointsListWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
        // Get index of the watchpoint by parsing the string
        const std::string text = item->text().toStdString();
        const auto idx_str = text.substr(1, 2);
        bool ok;
        u32 idx = QString::fromStdString(idx_str).toUInt(&ok);
        if (!ok) {
            QMessageBox::critical(this, tr("Failed to set watchpoint"), tr("Failed to set watchpoint"));    // Unreachable
            return;
        }
        
        // Remove the watchpoint
        auto watchpoint = mem_watchpoints[idx];
        bool is_read_watchpoint;
        auto watchpoints = getWatchpointList(watchpoint, is_read_watchpoint);
        mem_watchpoints.erase(mem_watchpoints.begin() + idx);
        updateWatchpoints();
        (*watchpoints).erase(watchpoint.addr);
    });
    
    // Breakpoints
    connect(ui.disasmListWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
        // Find out the address of this instruction by parsing it from the disasm string...
        // I know it's a bit weird but it works
        const std::string text = item->text().toStdString();
        const auto addr_str = text.substr(2, 8);    // There is probably no need to omit "0x" but whatever
        bool ok;
        u32 addr = QString::fromStdString(addr_str).toUInt(&ok, 16);
        if (!ok) {
            QMessageBox::critical(this, tr("Failed to set breakpoint"), tr("Failed to set breakpoint"));
            return;
        }
        
        // Remove it if it was already set
        for (int i = 0; i < exec_breakpoints.size(); i++) {
            if (exec_breakpoints[i] == addr) {
                // Remove the breakpoint
                exec_breakpoints.erase(exec_breakpoints.begin() + i);
                updateDisasm();
                this->ps3->mem.watchpoints_r.erase(addr);
                return;
            }
        }
        
        // Set the breakpoint
        exec_breakpoints.push_back(addr);
        updateDisasm();
        this->ps3->mem.watchpoints_r[addr] = std::bind(&GameWindow::breakOnNextInstrIfExec, this->game_window, std::placeholders::_1);
        this->ps3->mem.markAsSlowMem(addr >> PAGE_SHIFT, true, false);
    });
    
    disabled_overlay->show();
    setWindowTitle("PPU Debugger");
    hide();
}

void PPUDebuggerWidget::enable() {
    disabled_overlay->hide();
    scrollToPC();
    updateDisasm();
    updateRegisters();
}

void PPUDebuggerWidget::disable() {
    disabled_overlay->show();
}

void PPUDebuggerWidget::updateDisasm() {
    auto [first, amount] = getVisibleLineRange(ui.disasmListWidget, ui.verticalScrollBar);
    ui.disasmListWidget->clear();
    first = (first + 3) & ~3;   // Align to 4 bytes
    amount *= sizeof(u32);    // Size of instruction
    for (u32 addr = first; addr < first + amount; addr += 4) {
        if (ps3->mem.isMapped(addr).first) {
            const Instruction instr = { .raw = ps3->mem.read<u32>(addr) };
            auto disasm = PPUDisassembler::disasm(ps3->ppu->state, instr, &ps3->mem, addr);
            
            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(disasm));
            if (ps3->ppu->state.pc == addr)
                item->setBackground(Qt::darkGreen);
            else {
                // Check if this address is a breakpoint
                for (auto& i : exec_breakpoints) {
                    if (i == addr)
                        item->setBackground(Qt::darkRed);
                }
            }
            
            ui.disasmListWidget->addItem(item);
        } else ui.disasmListWidget->addItem(QString::fromStdString(std::format("0x{:08x}   |     unmapped memory", addr)));
    }
}

void PPUDebuggerWidget::updateRegisters() {
    auto& state = ps3->ppu->state;
    auto curr_thread = ps3->thread_manager.getCurrentThread();
    std::string t;
    t.reserve(2048);
    
    // Current thread
    t += std::format("Current thread: {:d} \"{:s}\"\n\n", curr_thread->id, curr_thread->name);
    
    t += std::format("pc:   0x{:08x}\n", state.pc);
    t += std::format("lr:   0x{:08x}\n", state.lr);
    t += std::format("ctr:  0x{:08x}\n", state.ctr);
    t += std::format("cr:   0x{:08x}\n", state.cr.raw);
    
    t += "\nGPRs\n";
    for (int i = 0; i < 10; i++)
        t += std::format("r{:01d}:     0x{:016x}\n", i, state.gprs[i]);
    for (int i = 10; i < 32; i++)
        t += std::format("r{:02d}:    0x{:016x}\n", i, state.gprs[i]);
    
    t += "\nFPRs\n";
    for (int i = 0; i < 10; i++)
        t += std::format("f{:01d}:     {:f}\n", i, state.fprs[i]);
    for (int i = 10; i < 32; i++)
        t += std::format("f{:02d}:    {:f}\n", i, state.fprs[i]);

    t += "\nVRs\n";
    for (int i = 0; i < 10; i++)
        t += std::format("v{:01d}:   {{0x{:08x}, 0x{:08x}, 0x{:08x}, 0x{:08x}}}\n      ({:f}, {:f}, {:f}, {:f})\n", i, state.vrs[i].w[3], state.vrs[i].w[2], state.vrs[i].w[1], state.vrs[i].w[0], *(float*)&state.vrs[i].w[3], *(float*)&state.vrs[i].w[2], *(float*)&state.vrs[i].w[1], *(float*)&state.vrs[i].w[0]);
    for (int i = 10; i < 32; i++)
        t += std::format("v{:02d}:  {{0x{:08x}, 0x{:08x}, 0x{:08x}, 0x{:08x}}}\n      ({:f}, {:f}, {:f}, {:f})\n", i, state.vrs[i].w[3], state.vrs[i].w[2], state.vrs[i].w[1], state.vrs[i].w[0], *(float*)&state.vrs[i].w[3], *(float*)&state.vrs[i].w[2], *(float*)&state.vrs[i].w[1], *(float*)&state.vrs[i].w[0]);
    
    ui.registerTextEdit->setPlainText(QString::fromStdString(t));
}

void PPUDebuggerWidget::updateWatchpoints() {
    auto type_to_str = [](MemoryWatchpointType& type) -> std::string {
        switch (type) {
            case MemoryWatchpointType::Read:    return tr("Read").toStdString();
            case MemoryWatchpointType::Write:   return tr("Write").toStdString();
            default:    Helpers::panic("Unknown MemoryWatchpointType\n");   // Unreachable
        }
    };
    
    ui.watchpointsListWidget->clear();
    for (int i = 0; i < mem_watchpoints.size(); i++) {
        auto& watchpoint = mem_watchpoints[i];
        std::string watchpoint_str = std::format("[{:03d}]    0x{:08x}    {:s}", i, watchpoint.addr, type_to_str(watchpoint.type));
        ui.watchpointsListWidget->addItem(QString::fromStdString(watchpoint_str));
    }
}

void PPUDebuggerWidget::scrollToAddress(u32 addr) {
    // Make it so the instruction shows up in the middle of the disasm view and not at the top
    u32 middle = getLinesInViewport(ui.disasmListWidget) / 2;
    middle *= sizeof(u32);  // Size of instruction
    middle = (middle + 3) & ~3; // Align to 4 bytes
    const s32 line = addr - middle; // Would make it a s64 to fit addresses up to 0xffffffff, but Qt scrollbar values are s32 anyway
    ui.verticalScrollBar->setValue(std::max(line, 0));
}

void PPUDebuggerWidget::scrollToPC() {
    scrollToAddress(ps3->ppu->state.pc);
}

bool PPUDebuggerWidget::eventFilter(QObject* obj, QEvent* event) {
    // Forward scroll events from the list widget to the external scrollbar
    if (obj == ui.disasmListWidget && event->type() == QEvent::Wheel) {
        QWheelEvent* wheel_event = (QWheelEvent*)event;
        int wheel_steps = wheel_event->angleDelta().y() / 60;
        int new_scroll_value = ui.verticalScrollBar->value() - wheel_steps;
        new_scroll_value = qBound(ui.verticalScrollBar->minimum(), new_scroll_value, ui.verticalScrollBar->maximum());
        ui.verticalScrollBar->setValue(new_scroll_value);
        return true;
    }
    
    return QWidget::eventFilter(obj, event);
}

void PPUDebuggerWidget::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    
    // If the widget is enabled (aka overlay is hidden) update pc, disasm and registers
    if (!disabled_overlay->isVisible())
        enable();
}
 
void PPUDebuggerWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    disabled_overlay->resize(event->size());
    updateDisasm();
}
