#pragma once
#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QString>
#include <QtWidgets/QFrame>
#include <ui_QtEchec.h>
#include "AbsChessPiece.h"
using std::vector;
using std::shared_ptr;

using ChessPiecesData::AbsChessPiece;

class DisplayManager
{

public:
	struct SpawnedPiece;
	DisplayManager();
	DisplayManager(QFrame* chessFrameParent, Ui::QtEchecClass* _ui);
	void setUpChessUi();
	
	void togglePlacementIndication(bool show, int gridX, int gridY);
	
	void summonPiece(std::shared_ptr<AbsChessPiece> pieceData);
	void movePieceToPosition(const std::shared_ptr<AbsChessPiece> piece, int gridX, int gridY);
	void setBackgroundColor(const std::shared_ptr<AbsChessPiece> piece, int gridX, int gridY, bool transparent);

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
	void movePieceToPosition(QWidget* piece, int gridX, int gridY);
	void setBackgroundColor(QWidget* piece, int gridX, int gridY, bool transparent);
	QFrame* chessFrame;
	const SpawnedPiece getSpawnedPiece(const const std::shared_ptr<AbsChessPiece> piece);

	vector<QPushButton*> allChessCasesButtons;
	vector<SpawnedPiece> spawnedPieces;
	

	Ui::QtEchecClass* ui;
};


