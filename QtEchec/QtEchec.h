/**
* Controller that links the model and the view together
* \file   QtEchec.h | QtEchec.cpp
* \author BEN RHOUMA Ahmed and KEZOUH Ryan
* \date   April 6th 2022
*/

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtEchec.h"
#include "DisplayManager.h"
#include "GameStateManager.h"

class QtEchec : public QMainWindow
{
	Q_OBJECT

public:
	QtEchec(QWidget* parent = Q_NULLPTR);

private:
	Ui::QtEchecClass ui;
	ChessDisplay::DisplayManager displayManager;
	GameStateManager gameStateManager;
	std::vector<QMetaObject::Connection> allConnections;
	GameStateManager::InitialBoardPiecesPosition selectedDefaultConfiguration = GameStateManager::InitialBoardPiecesPosition::Default;

	void initializeGame(bool reset);
	void resetGame();
	void showWelcomeWindow();
};
