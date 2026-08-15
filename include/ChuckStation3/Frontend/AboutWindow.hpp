#pragma once

#include "Frontend/UI/ui_about.h"

#include <QtWidgets>
#include <common.hpp>

class AboutWindow : public QWidget {
    Q_OBJECT

public:
    AboutWindow(QWidget* parent = nullptr);
    Ui::About ui;
};
