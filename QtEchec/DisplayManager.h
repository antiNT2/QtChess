#pragma once
#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QString>
#include <QtWidgets/QFrame>
#include "AbsChessPiece.h"
#include <ui_QtEchec.h>
using std::vector;
using std::shared_ptr;

class DisplayManager
{

public:
	struct SpawnedPiece;
	DisplayManager();
	DisplayManager(QFrame* chessFrameParent, Ui::QtEchecClass* _ui);
	void setUpChessUi();
	void movePieceToPosition(QWidget* piece, int gridX, int gridY);
	void togglePlacementIndication(bool show, int gridX, int gridY);
	
	void summonPiece(std::shared_ptr<AbsChessPiece> pieceData);

	void displayMessage(QString messageToShow);

	const int getNumberOfRows();
	const int getNumberOfColumns();
	QPushButton* getButtonAtPosition(int gridX, int gridY);

	const vector<SpawnedPiece>& getSpawnedPieces();

private:

	struct SpawnedPiece
	{
		SpawnedPiece(std::shared_ptr<AbsChessPiece> _pieceData, QPushButton* _spawnedPieceVisual)
		{
			pieceData = _pieceData;
			spawnedPieceVisual = _spawnedPieceVisual;
		}
		std::shared_ptr<AbsChessPiece> pieceData;
		QPushButton* spawnedPieceVisual;
	};

	QPushButton* createChessCase(QWidget* parent);
	QPushButton* createPieceVisual(std::shared_ptr<AbsChessPiece> pieceData);
	QString getButtonStyleSheet(bool isRedCase, bool isPlacementIndicator);
	QFrame* chessFrame;

	vector<QPushButton*> allChessCasesButtons;
	vector<SpawnedPiece> spawnedPieces;
	

	Ui::QtEchecClass* ui;
};


