#pragma once
#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QtWidgets/QFrame>
using namespace std;

class DisplayManager
{
public:
	DisplayManager();
	DisplayManager(QFrame* chessFrameParent);
	void setUpChessUi();
	void movePieceToPosition(QWidget* piece, int gridX, int gridY);
	vector<QPushButton*>& getAllChessButtons();

private:
	QPushButton* createChessCase(QWidget* parent);
	QFrame* chessFrame;
	vector<QPushButton*> allChessButtons;
};


