#pragma once
#include "DisplayManager.h"
#include "AbsChessPiece.h"
#include <qwidget.h>

class GameStateManager
{
public:
	GameStateManager();
	GameStateManager(DisplayManager* _displayManager);
	void instantiateInitialPieces();
	void selectPiece(const shared_ptr<AbsChessPiece> pieceToSelect);
	template<typename T> void instantiatePiece(AbsChessPiece::PiecePosition position, bool isPlayer1);

private:
	DisplayManager* displayManager;
	std::vector<shared_ptr<AbsChessPiece>> allChessPieces;
	bool isPlayer1Turn = true;

	//std::vector<AbsChessPiece::PiecePosition> computeAllowedDestinations(shared_ptr<AbsChessPiece> piece);
	const shared_ptr<AbsChessPiece> getPieceAtPosition(AbsChessPiece::PiecePosition pos);

	shared_ptr<AbsChessPiece> currentSelectedPiece;
	std::vector<AbsChessPiece::PiecePosition> currentAllowedDestinations;
};
