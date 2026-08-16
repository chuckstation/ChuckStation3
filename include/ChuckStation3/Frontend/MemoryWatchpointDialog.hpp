
#pragma once

#include <QtWidgets>
#include <common.hpp>

class MemoryWatchpointDialog : public QDialog {
    Q_OBJECT

public:
    MemoryWatchpointDialog(QWidget* parent = nullptr);

    enum class MemoryWatchpointType { Read, Write };

    u32                  addr;
    MemoryWatchpointType type;

private:
    QLineEdit* addr_field;
    QComboBox* type_combo_box;
};
