#pragma once

#include "Frontend/UI/ui_thread_debugger.h"

#include <Frontend/GameWindow.hpp>
#include <PlayStation3.hpp>
#include <QTimer>
#include <QtWidgets>
#include <atomic>
#include <chrono>
#include <common.hpp>
#include <thread>

class ThreadDebuggerWidget : public QWidget {
    Q_OBJECT

public:
    ThreadDebuggerWidget(PlayStation3* ps3, QWidget* parent = nullptr);
    PlayStation3*      ps3;
    Ui::ThreadDebugger ui;

private:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

    QTimer timer;
    void   update();

    void setListItem(QTableWidget* table, int row, int column, std::string str);
};
