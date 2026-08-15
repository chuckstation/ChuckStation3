#pragma once

#include "Frontend/UI/ui_settings.h"

#include <Frontend/GameWindow.hpp>
#include <PlayStation3.hpp>
#include <QtWidgets>
#include <common.hpp>

class SettingsWidget : public QWidget {
    Q_OBJECT

public:
    SettingsWidget(PlayStation3* ps3, QWidget* parent = nullptr);
    PlayStation3* ps3;
    Ui::Settings  ui;
};
