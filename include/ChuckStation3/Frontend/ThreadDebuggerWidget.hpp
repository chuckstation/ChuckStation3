#pragma once

#include <common.hpp>

#include <atomic>
#include <thread>
#include <chrono>

#include <Frontend/GameWindow.hpp>
#include "Frontend/UI/ui_thread_debugger.h"
#include <QtWidgets>
#include <QTimer>
#include <PlayStation3.hpp>


class ThreadDebuggerWidget : public QWidget {
    Q_OBJECT

public:
    ThreadDebuggerWidget(PlayStation3* ps3, QWidget* parent = nullptr);
    PlayStation3* ps3;
    Ui::ThreadDebugger ui;
    
private:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    
    QTimer timer;
    void update();
    
    void setListItem(QTableWidget* table, int row, int column, std::string str);
};
