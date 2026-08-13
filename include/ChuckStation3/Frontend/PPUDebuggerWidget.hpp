#pragma once

#include <common.hpp>

#include <Frontend/GameWindow.hpp>
#include <Frontend/MemoryWatchpointDialog.hpp>
#include <PlayStation3.hpp>
#include "Frontend/UI/ui_ppu_debugger.h"
#include <QtWidgets>


using MemoryWatchpointType = MemoryWatchpointDialog::MemoryWatchpointType;

class DisabledWidgetOverlay;

class PPUDebuggerWidget : public QWidget {
    Q_OBJECT
    
public:
    PPUDebuggerWidget(PlayStation3* ps3, GameWindow* game_window, QWidget* parent = nullptr);
    PlayStation3* ps3;
    GameWindow* game_window;
    Ui::PPUDebugger ui;
    
    void enable();
    void disable();
    
private:
    DisabledWidgetOverlay* disabled_overlay;
    
    struct MemoryWatchpoint {
        u32 addr;
        MemoryWatchpointType type;
    };
    
    std::vector<u32> exec_breakpoints;
    std::vector<MemoryWatchpoint> mem_watchpoints;
    
    bool isReadWatchpoint(const MemoryWatchpoint& watchpoint) {
        if (watchpoint.type == MemoryWatchpointType::Read)
            return true;
        else return false;
    }
    
    std::unordered_map<u64, std::function<void(u64)>>* getWatchpointList(const MemoryWatchpoint& watchpoint, bool& is_read_watchpoint) {
        is_read_watchpoint = isReadWatchpoint(watchpoint);
        
        if (is_read_watchpoint) return &ps3->mem.watchpoints_r;
        else                    return &ps3->mem.watchpoints_w;
    }
    
    void updateDisasm();
    void updateRegisters();
    void updateWatchpoints();
    void scrollToAddress(u32 addr);
    void scrollToPC();
    
    bool eventFilter(QObject* obj, QEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
};
