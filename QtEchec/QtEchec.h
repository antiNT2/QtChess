#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtEchec.h"
#include "DisplayManager.h"
#include "GameStateManager.h"

class QtEchec : public QMainWindow
{
    Q_OBJECT

public:
    QtEchec(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtEchecClass ui;
    DisplayManager displayManager;
    GameStateManager gameStateManager;
};