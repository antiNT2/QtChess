/**
* View script that interacts with Qt to display the actual game on screen
* \file   DisplayManager.h | DisplayManager.cpp
* \author BEN RHOUMA Ahmed and KEZOUH Ryan
* \date   April 6th 2022
*/

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
namespace ChessDisplay
{
	class DisplayManager : public QObject
	{

		Q_OBJECT

	private:
		struct SpawnedPiece; //forward declaration

	public:
		DisplayManager();
		DisplayManager(QFrame* chessFrameParent, Ui::QtEchecClass* _ui);
		~DisplayManager();
		void setUpChessUi();

		void operator=(const DisplayManager& other);

		void togglePlacementIndication(bool show, int gridX, int gridY);

		void setBackgroundColor(int gridX, int gridY, bool transparent);

		void displayMessage(const QString messageToShow);

		void displayDialogueBox(const QString messageToShow);

		const int getNumberOfRows();
		const int getNumberOfColumns();
		QPushButton* getButtonAtPosition(int gridX, int gridY);

		const vector<SpawnedPiece>& getSpawnedPieces();

		std::string getPieceNameFromType(ChessPiecesData::PieceType type);

	public slots:
		void summonPiece(const std::shared_ptr<AbsChessPiece> pieceData);
		void removePiece(const std::shared_ptr<AbsChessPiece> pieceToRemove);
		void movePieceToPosition(const std::shared_ptr<AbsChessPiece> piece, int gridX, int gridY);
		void setPlayerTurnIndicator(bool isPlayer1Turn);
		void deleteAllPieces();

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
		enum class CaseType
		{
			Normal,
			PlacementIndicator,
			DeathCase
		};
		QString getButtonStyleSheet(bool isRedCase, CaseType caseType);
		void movePieceToPosition(QWidget* piece, int gridX, int gridY);
		void setBackgroundColor(QWidget* piece, bool transparent);
		const SpawnedPiece getSpawnedPiece(const std::shared_ptr<AbsChessPiece> piece);
		const SpawnedPiece& getSpawnedPiece(int gridX, int gridY);
		const bool pieceExistInPosition(int gridX, int gridY);

		vector<QPushButton*> allChessCasesButtons;
		vector<SpawnedPiece> spawnedPieces;


		Ui::QtEchecClass* ui = nullptr;
		QFrame* chessFrame = nullptr;
	};
}


