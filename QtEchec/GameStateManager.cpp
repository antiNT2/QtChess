#include "GameStateManager.h"
#include "KingPiece.h"

GameStateManager::GameStateManager()
{
}

GameStateManager::GameStateManager(DisplayManager* _displayManager) : displayManager(_displayManager)
{
}

void GameStateManager::instantiateInitialPieces()
{
	//Summon kings
	instantiatePiece<KingPiece>(AbsChessPiece::PiecePosition(4, 7), true);
	instantiatePiece<KingPiece>(AbsChessPiece::PiecePosition(4, 0), false);
}

void GameStateManager::selectPiece(const shared_ptr<AbsChessPiece> pieceToSelect)
{
	if (currentSelectedPiece == nullptr)
	{
		if (pieceToSelect.get()->isPlayer1Piece() != isPlayer1Turn)
			return;
	}

	if (pieceToSelect.get()->isPlayer1Piece() == isPlayer1Turn)
	{
		//we allow selecting a piece as long as it is from the correct team
		currentSelectedPiece = pieceToSelect;
		displayManager->displayMessage(QString("Selected " +
			QString::fromStdString(pieceToSelect.get()->getPieceName()) + " at (%1, %2) \n").
			arg(pieceToSelect.get()->getPiecePosition().gridX).arg(pieceToSelect.get()->getPiecePosition().gridY));
	}
	else
	{
		//we already selected our piece, and now we clicked on an enemy one to eat it
		displayManager->displayMessage(QString("Trying to eat " + QString::fromStdString(pieceToSelect.get()->getPieceName() + "\n")));
	}
}

//std::vector<AbsChessPiece::PiecePosition> GameStateManager::computeAllowedDestinations(shared_ptr<AbsChessPiece> piece)
//{
//	std::vector<AbsChessPiece::Trajectory> allowedTrajectories = piece.get()->getPossibleDestinations();
//
//	for (auto&& trajectory : allowedTrajectories)
//	{
//		
//	}
//}

const shared_ptr<AbsChessPiece> GameStateManager::getPieceAtPosition(AbsChessPiece::PiecePosition pos)
{
	for (auto&& piece : allChessPieces)
	{
		if (piece.get()->getPiecePosition() == pos)
		{
			return piece;
		}
	}

	return nullptr;
}

template<typename T>
void GameStateManager::instantiatePiece(AbsChessPiece::PiecePosition position, bool isPlayer1)
{
	static_assert(std::is_base_of<AbsChessPiece, T>::value);

	shared_ptr<AbsChessPiece> newChessPiece = std::make_shared<T>(position, isPlayer1);
	allChessPieces.push_back(newChessPiece);
	displayManager->summonPiece(newChessPiece);
}
