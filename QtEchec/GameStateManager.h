#pragma once
#include "DisplayManager.h"
#include "AbsChessPiece.h"

using ChessPiecesData::AbsChessPiece;

class GameStateManager
{
public:
	GameStateManager();
	GameStateManager(DisplayManager* _displayManager);
	void instantiateInitialPieces();
	void selectPiece(const shared_ptr<AbsChessPiece> pieceToSelect);
	void deselectCurrentPiece();
	template<typename T> void instantiatePiece(ChessPiecesData::PiecePosition position, bool isPlayer1);
	void moveCurrentPiece(ChessPiecesData::PiecePosition destination);

private:
	DisplayManager* displayManager;
	ChessPiecesData::ChessPiecesHolder piecesList;
	bool isPlayer1Turn = true;
	void setCurrentAllowedDestinations(std::vector<ChessPiecesData::PiecePosition> allowedDestinations);
	bool isValidPiecePosition(ChessPiecesData::PiecePosition pos);
	bool isPositionIncludedInCurrentAllowedPos(ChessPiecesData::PiecePosition pos);
	bool movePiece(const std::shared_ptr<AbsChessPiece> pieceToMove, ChessPiecesData::PiecePosition destination);

	shared_ptr<AbsChessPiece> currentSelectedPiece;
	std::vector<ChessPiecesData::PiecePosition> currentAllowedDestinations;
};
