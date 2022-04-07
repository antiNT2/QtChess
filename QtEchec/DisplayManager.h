#pragma once
#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QString>
#include <QtWidgets/QFrame>
#include <ui_QtEchec.h>
#include "AbsChessPiece.h"
using std::vector;
using std::shared_ptr;

#include <QObject>

using ChessPiecesData::AbsChessPiece;

class DisplayManager : public QObject
{

	Q_OBJECT

public:
	struct SpawnedPiece;
	DisplayManager();
	DisplayManager(QFrame* chessFrameParent, Ui::QtEchecClass* _ui);
	void setUpChessUi();

	void operator=(const DisplayManager& other);
	
	void togglePlacementIndication(bool show, int gridX, int gridY);
	
	void setBackgroundColor(int gridX, int gridY, bool transparent);

	void displayMessage(QString messageToShow);

	const int getNumberOfRows();
	const int getNumberOfColumns();
	QPushButton* getButtonAtPosition(int gridX, int gridY);

	const vector<SpawnedPiece>& getSpawnedPieces();

public slots:
	void summonPiece(std::shared_ptr<AbsChessPiece> pieceData);
	void movePieceToPosition(const std::shared_ptr<AbsChessPiece> piece, int gridX, int gridY);

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
	void setBackgroundColor(QWidget* piece, bool transparent);
	QFrame* chessFrame;
	const SpawnedPiece getSpawnedPiece(const std::shared_ptr<AbsChessPiece> piece);
	const SpawnedPiece& getSpawnedPiece(int gridX, int gridY);

	vector<QPushButton*> allChessCasesButtons;
	vector<SpawnedPiece> spawnedPieces;
	

	Ui::QtEchecClass* ui;
};


