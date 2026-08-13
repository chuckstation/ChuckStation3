#include "MemoryWatchpointDialog.hpp"


MemoryWatchpointDialog::MemoryWatchpointDialog(QWidget* parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    addr_field = new QLineEdit(this);
    layout->addWidget(new QLabel(tr("Address:"), this));
    layout->addWidget(addr_field);
    
    type_combo_box = new QComboBox(this);
    type_combo_box->addItem(tr("Read"), (int)MemoryWatchpointType::Read);
    type_combo_box->addItem(tr("Write"), (int)MemoryWatchpointType::Write);
    layout->addWidget(new QLabel(tr("Break on:"), this));
    layout->addWidget(type_combo_box);
    
    QPushButton* confirm_button = new QPushButton(tr("Confirm"), this);
    layout->addWidget(confirm_button);
    connect(confirm_button, &QPushButton::clicked, this, [this]() {
        // Verify address is valid
        bool ok;
        addr = addr_field->text().toUInt(&ok, 16);
        if (!ok) {
            QMessageBox::critical(this, tr("Invalid address"), tr("Invalid hexadecimal address"));
            return;
        }
        
        // Watchpoint type
        type = (MemoryWatchpointType)type_combo_box->currentData().toInt();

        accept();
    });
    
    setWindowTitle(tr("Memory watchpoint"));
}
