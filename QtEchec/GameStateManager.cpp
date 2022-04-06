#include "GameStateManager.h"
#include "KingPiece.h"
#include "TowerPiece.h"

GameStateManager::GameStateManager()
{
}

GameStateManager::GameStateManager(DisplayManager* _displayManager) : displayManager(_displayManager)
{
}

void GameStateManager::instantiateInitialPieces()
{
	using namespace ChessPiecesData;

	//Summon p1
	instantiatePiece<KingPiece>(PiecePosition(4, 7), true);
	instantiatePiece<TowerPiece>(PiecePosition(0, 7), true);
	instantiatePiece<TowerPiece>(PiecePosition(7, 7), true);


	
	//Summon p2
	instantiatePiece<KingPiece>(PiecePosition(4, 0), false);
	instantiatePiece<TowerPiece>(PiecePosition(0, 0), false);
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
			deselectCurrentPiece(); //we deselect the current one just in case

			currentSelectedPiece = pieceToSelect;

			/*displayManager->displayMessage(QString("+Selected " +
				QString::fromStdString(pieceToSelect.get()->getPieceName()) + " at (%1, %2) \n").
				arg(pieceToSelect.get()->getPiecePosition().gridX).arg(pieceToSelect.get()->getPiecePosition().gridY));*/

			displayManager->setBackgroundColor(currentSelectedPiece, currentSelectedPiece.get()->getPiecePosition().gridX,
				currentSelectedPiece.get()->getPiecePosition().gridY, false);

			setCurrentAllowedDestinations(pieceToSelect.get()->getPossibleDestinations(piecesList));
		}

	}
	else
	{
		//we already selected our piece, and now we clicked on an enemy to eat it

		// Are we allowed to move there to eat the piece ?
		if (isPositionIncludedInCurrentAllowedPos(pieceToSelect.get()->getPiecePosition()))
		{
			//TODO: Eat the piece
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

	/*displayManager->displayMessage(QString("-Unselected " +
		QString::fromStdString(currentSelectedPiece.get()->getPieceName()) + " at (%1, %2) \n").
		arg(currentSelectedPiece.get()->getPiecePosition().gridX).arg(currentSelectedPiece.get()->getPiecePosition().gridY));*/

	displayManager->setBackgroundColor(currentSelectedPiece, currentSelectedPiece.get()->getPiecePosition().gridX,
		currentSelectedPiece.get()->getPiecePosition().gridY, true);

	currentSelectedPiece = nullptr;
	setCurrentAllowedDestinations(std::vector<ChessPiecesData::PiecePosition>());
}

void GameStateManager::moveCurrentPiece(ChessPiecesData::PiecePosition destination)
{
	if (currentSelectedPiece == nullptr)
	{
		displayManager->displayMessage(QString("Select a piece first! \n"));
		return;
	}
	if (isPositionIncludedInCurrentAllowedPos(destination) == false)
	{
		displayManager->displayMessage(QString("Illegal move! \n"));
		deselectCurrentPiece();
		return;
	}

	if (movePiece(currentSelectedPiece, destination))
	{
		deselectCurrentPiece();
		isPlayer1Turn = !isPlayer1Turn;
	}
}

void GameStateManager::setCurrentAllowedDestinations(std::vector<ChessPiecesData::PiecePosition> allowedDestinations)
{
	//First we hide the current placement indicators
	for (auto dest : currentAllowedDestinations)
	{
		if (isValidPiecePosition(dest))
		{
			displayManager->togglePlacementIndication(false, dest.gridX, dest.gridY);
		}
	}

	currentAllowedDestinations.clear();

	//Then we display the new ones
	for (auto dest : allowedDestinations)
	{
		if (isValidPiecePosition(dest))
		{
			currentAllowedDestinations.push_back(dest);
			//displayManager->displayMessage(QString("Allow (%1, %2) \n").arg(dest.gridX).arg(dest.gridY));
			displayManager->togglePlacementIndication(true, dest.gridX, dest.gridY);
		}
	}
}

bool GameStateManager::isValidPiecePosition(ChessPiecesData::PiecePosition pos)
{
	return (pos.gridX < 8 && pos.gridY < 8) && (pos.gridX >= 0 && pos.gridY >= 0);
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
	displayManager->movePieceToPosition(pieceToMove, destination.gridX, destination.gridY);

	return true;
}


template<typename T>
void GameStateManager::instantiatePiece(ChessPiecesData::PiecePosition position, bool isPlayer1)
{
	static_assert(std::is_base_of<AbsChessPiece, T>::value);

	shared_ptr<AbsChessPiece> newChessPiece = std::make_shared<T>(position, isPlayer1);
	piecesList.allChessPieces.push_back(newChessPiece);
	displayManager->summonPiece(newChessPiece);
}
