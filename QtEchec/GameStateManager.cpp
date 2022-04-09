#include "GameStateManager.h"
#include "KingPiece.h"
#include "TowerPiece.h"
#include "KnightPiece.h"

#include<QDebug>

const int maxGridX = 7;
const int maxGridY = 7;

GameStateManager::GameStateManager()
{
}

//GameStateManager::GameStateManager(DisplayManager* _displayManager) /* : displayManager(_displayManager)*/
//{
//}

void GameStateManager::instantiateInitialPieces()
{
	using namespace ChessPiecesData;

	instantiatePieceForBothSides<KingPiece>(PiecePosition(4, 7));
	instantiatePieceForBothSides<TowerPiece>(PiecePosition(0, 7));
	instantiatePieceForBothSides<KnightPiece>(PiecePosition(1, 7));
	instantiatePieceForBothSides<KnightPiece>(PiecePosition(6, 7));
	instantiatePieceForBothSides<TowerPiece>(PiecePosition(7, 7));
}

void GameStateManager::selectPiece(const shared_ptr<AbsChessPiece> pieceToSelect)
{
	if (currentSelectedPiece == nullptr)
	{
		if (pieceToSelect.get()->isPlayer1Piece() != isPlayer1Turn)
			return;
	}

	//we allow selecting a piece as long as it is from the correct team
	if (pieceToSelect.get()->isPlayer1Piece() == isPlayer1Turn)
	{
		//if we already selected the piece to select, it's time to unselect it
		if (currentSelectedPiece != nullptr && currentSelectedPiece == pieceToSelect)
		{
			deselectCurrentPiece();
		}
		else
		{
			if (currentSelectedPiece != nullptr)
				deselectCurrentPiece(); //we deselect the current one just in case

			currentSelectedPiece = pieceToSelect;

			emit onSelectPiece(currentSelectedPiece.get()->getPiecePosition().gridX,
				currentSelectedPiece.get()->getPiecePosition().gridY);

			setCurrentAllowedDestinations(pieceToSelect.get()->getPossibleDestinations(piecesList));
		}

	}
	else
	{
		//we already selected our piece, and now we clicked on an enemy to eat it

		// Are we allowed to move there to eat the piece ?
		if (isPositionIncludedInCurrentAllowedPos(pieceToSelect.get()->getPiecePosition()))
		{
			//Eat the piece
			moveCurrentPiece(pieceToSelect.get()->getPiecePosition());
			destroyPiece(pieceToSelect);
		}
		else
		{
			deselectCurrentPiece();
		}

	}
}

void GameStateManager::deselectCurrentPiece()
{
	if (currentSelectedPiece == nullptr)
		return;

	emit onDeselectPiece(currentSelectedPiece.get()->getPiecePosition().gridX,
		currentSelectedPiece.get()->getPiecePosition().gridY);

	currentSelectedPiece = nullptr;
	setCurrentAllowedDestinations(std::vector<ChessPiecesData::PiecePosition>());
}

void GameStateManager::moveCurrentPiece(ChessPiecesData::PiecePosition destination)
{
	if (currentSelectedPiece == nullptr)
	{
		emit onNoPieceSelected();
		return;
	}
	if (isPositionIncludedInCurrentAllowedPos(destination) == false)
	{
		emit onIllegalMoveChosen();
		deselectCurrentPiece();
		return;
	}

	if (movePiece(currentSelectedPiece, destination))
	{
		deselectCurrentPiece();
		isPlayer1Turn = !isPlayer1Turn;
		emit onChangePlayerTurn(isPlayer1Turn);
	}
}

void GameStateManager::setCurrentAllowedDestinations(std::vector<ChessPiecesData::PiecePosition> allowedDestinations)
{
	//First we hide the current placement indicators
	for (auto dest : currentAllowedDestinations)
	{
		if (isValidPiecePosition(dest))
		{
			emit onRemoveAllowedDestination(dest.gridX, dest.gridY);
		}
	}

	currentAllowedDestinations.clear();

	//Then we display the new ones
	for (auto dest : allowedDestinations)
	{
		if (isValidPiecePosition(dest))
		{
			currentAllowedDestinations.push_back(dest);
			emit onAddAllowedDestination(dest.gridX, dest.gridY);
		}
	}
}

bool GameStateManager::isValidPiecePosition(ChessPiecesData::PiecePosition pos)
{
	return (pos.gridX <= maxGridX && pos.gridY <= maxGridY) && (pos.gridX >= 0 && pos.gridY >= 0);
}

bool GameStateManager::isPositionIncludedInCurrentAllowedPos(ChessPiecesData::PiecePosition pos)
{
	for (auto p : currentAllowedDestinations)
	{
		if (pos == p)
			return true;
	}

	return false;
}

bool GameStateManager::movePiece(const std::shared_ptr<AbsChessPiece> pieceToMove, ChessPiecesData::PiecePosition destination)
{
	if (pieceToMove == nullptr)
		return false;
	if (isValidPiecePosition(destination) == false)
		return false;

	pieceToMove.get()->setPiecePosition(destination);
	emit onPieceMoved(pieceToMove, destination.gridX, destination.gridY);

	return true;
}

void GameStateManager::destroyPiece(const std::shared_ptr<AbsChessPiece> pieceToDestroy)
{
	piecesList.removePiece(pieceToDestroy);

	emit onRemovedPiece(pieceToDestroy);
	emit onDeselectPiece(pieceToDestroy.get()->getPiecePosition().gridX, pieceToDestroy.get()->getPiecePosition().gridY);
}


template<typename T>
void GameStateManager::instantiatePieceForBothSides(ChessPiecesData::PiecePosition player1Position)
{
	using namespace ChessPiecesData;
	PiecePosition player2Position = PiecePosition(player1Position.gridX, maxGridY - player1Position.gridY);

	instantiatePiece<T>(player1Position, true);
	instantiatePiece<T>(player2Position, false);
}

template<typename T>
void GameStateManager::instantiatePiece(ChessPiecesData::PiecePosition position, bool isPlayer1)
{
	static_assert(std::is_base_of<AbsChessPiece, T>::value);

	shared_ptr<AbsChessPiece> newChessPiece = std::make_shared<T>(position, isPlayer1);
	piecesList.allChessPieces.push_back(newChessPiece);

	emit onInstantiatePiece(newChessPiece);
}
