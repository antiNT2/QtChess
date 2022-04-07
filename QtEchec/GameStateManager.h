#pragma once
// #include "DisplayManager.h"
#include "AbsChessPiece.h"

#include <QObject>
using std::vector;
using std::shared_ptr;

using ChessPiecesData::AbsChessPiece;

class GameStateManager : public QObject
{
	Q_OBJECT

public:
	GameStateManager();
	//GameStateManager(DisplayManager* _displayManager);
	void instantiateInitialPieces();
	void selectPiece(const shared_ptr<AbsChessPiece> pieceToSelect);
	void deselectCurrentPiece();
	template<typename T> void instantiatePiece(ChessPiecesData::PiecePosition position, bool isPlayer1);
	void moveCurrentPiece(ChessPiecesData::PiecePosition destination);

private:
	//DisplayManager* displayManager;
	ChessPiecesData::ChessPiecesHolder piecesList;
	bool isPlayer1Turn = true;
	void setCurrentAllowedDestinations(std::vector<ChessPiecesData::PiecePosition> allowedDestinations);
	bool isValidPiecePosition(ChessPiecesData::PiecePosition pos);
	bool isPositionIncludedInCurrentAllowedPos(ChessPiecesData::PiecePosition pos);
	bool movePiece(const std::shared_ptr<AbsChessPiece> pieceToMove, ChessPiecesData::PiecePosition destination);
	void destroyPiece(const std::shared_ptr<AbsChessPiece> pieceToDestroy);

	shared_ptr<AbsChessPiece> currentSelectedPiece;
	std::vector<ChessPiecesData::PiecePosition> currentAllowedDestinations;

signals:
	void onNoPieceSelected();
	void onIllegalMoveChosen();
	void onInstantiatePiece(const std::shared_ptr<AbsChessPiece> newPiece);
	void onRemoveAllowedDestination(int x, int y);
	void onAddAllowedDestination(int x, int y);
	void onSelectPiece(int x, int y);
	void onDeselectPiece(int x, int y);
	void onPieceMoved(const std::shared_ptr<AbsChessPiece> pieceToMove, int gridX, int gridY);
	void onRemovedPiece(const std::shared_ptr<AbsChessPiece> removedPiece);
};
