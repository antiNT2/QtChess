#pragma once
#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QString>
#include <QtWidgets/QFrame>
using namespace std;

class DisplayManager
{
public:
	DisplayManager();
	DisplayManager(QFrame* chessFrameParent);
	void setUpChessUi();
	void movePieceToPosition(QWidget* piece, int gridX, int gridY);
	void togglePlacementIndication(bool show, int gridX, int gridY);
	QPushButton* getButtonAtPosition(int gridX, int gridY);
	vector<QPushButton*>& getAllChessButtons();

private:
	QPushButton* createChessCase(QWidget* parent);
	QFrame* chessFrame;
	vector<QPushButton*> allChessButtons;
	QString getButtonStyleSheet(bool isRedCase, bool isPlacementIndicator);
};


