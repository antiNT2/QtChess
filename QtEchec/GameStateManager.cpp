#include "GameStateManager.h"
#include "KingPiece.h"
#include "TowerPiece.h"
#include "KnightPiece.h"

#include<QDebug>
#include <stdexcept>

const int maxGridX = 7;
const int maxGridY = 7;

GameStateManager::GameStateManager()
{
}

GameStateManager::~GameStateManager()
{
	//resetBoard();
	//qDebug() << "Destroyed board \n";
}

void GameStateManager::instantiateInitialPieces()
{
	using namespace ChessPiecesData;

	/*instantiatePieceForBothSides<KingPiece>(PiecePosition(4, 7));
	instantiatePieceForBothSides<TowerPiece>(PiecePosition(0, 7));
	instantiatePieceForBothSides<KnightPiece>(PiecePosition(1, 7));
	instantiatePieceForBothSides<KnightPiece>(PiecePosition(6, 7));
	instantiatePieceForBothSides<TowerPiece>(PiecePosition(7, 7));*/

	instantiatePiece<KingPiece>(PiecePosition(7, 3), false);

	instantiatePiece<KingPiece>(PiecePosition(5, 3), true);
	instantiatePiece<TowerPiece>(PiecePosition(6, 7), true);

	qDebug() << "Finished instantiating new pieces \n";
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

void GameStateManager::resetBoard()
{
	piecesList.allChessPieces.clear();
	isPlayer1Turn = true;
	onResetBoard();
}

void GameStateManager::operator=(const GameStateManager& other)
{
	isPlayer1Turn = other.isPlayer1Turn;
	piecesList = other.piecesList;
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

	if (currentSelectedPiece == nullptr)
		return;

	//Then we display the new ones
	ChessPiecesData::PiecePosition originalPos = currentSelectedPiece->getPiecePosition();
	for (auto dest : allowedDestinations)
	{
		if (isValidPiecePosition(dest))
		{
			//make sure moving there doesn;t places us in check
			currentSelectedPiece->setPiecePosition(dest);
			if (!isKingInCheckWithBoardConfiguration(isPlayer1Turn, piecesList))
			{
				//Restore the og position
				currentSelectedPiece->setPiecePosition(originalPos);

				currentAllowedDestinations.push_back(dest);
				emit onAddAllowedDestination(dest.gridX, dest.gridY);
			}

			//Restore the og position
			currentSelectedPiece->setPiecePosition(originalPos);
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

	verifyCheckAndCheckmate();

	return true;
}

void GameStateManager::destroyPiece(const std::shared_ptr<AbsChessPiece> pieceToDestroy)
{
	piecesList.removePiece(pieceToDestroy);

	emit onRemovedPiece(pieceToDestroy);
	emit onDeselectPiece(pieceToDestroy.get()->getPiecePosition().gridX, pieceToDestroy.get()->getPiecePosition().gridY);
}

bool GameStateManager::isPlayerInCheckmate(bool player1)
{
	using namespace ChessPiecesData;
	bool output = true;

	if (isKingInCheckWithBoardConfiguration(player1, piecesList))
	{
		std::vector possibleEscapes = piecesList.getKingPiece(player1).get()->getPossibleDestinations(piecesList);
		PiecePosition initialPos = piecesList.getKingPiece(player1).get()->getPiecePosition();

		for (auto&& escapeDestination : possibleEscapes)
		{
			if (isValidPiecePosition(escapeDestination) == false)
				continue; //dont check invalid positions


			ChessPiecesHolder possiblePiecesConfiguration = piecesList;
			possiblePiecesConfiguration.getKingPiece(player1).get()->setPiecePosition(escapeDestination);

			if (!isKingInCheckWithBoardConfiguration(player1, possiblePiecesConfiguration))
			{
				//There is one configuration where we're not in check -> no checkmate :)
				qDebug() << "We can escape at " << escapeDestination.print().c_str() << (player1 ? " PLAYER 1" : " PLAYER 2");
				output = false;
				break;
			}
			else
			{
				qDebug() << "Can't escape at " << escapeDestination.print().c_str() << (player1 ? " PLAYER 1" : " PLAYER 2");
			}
		}

		piecesList.getKingPiece(player1).get()->setPiecePosition(initialPos);
	}
	else
	{
		output = false;
	}

	return output;

}

bool GameStateManager::isKingInCheckWithBoardConfiguration(bool player1King, ChessPiecesData::ChessPiecesHolder configuration)
{
	using namespace ChessPiecesData;
	PiecePosition kingPos = configuration.getKingPiece(player1King).get()->getPiecePosition();

	// We check if any of the allowed movements of all the board pieces is on the king's position
	for (auto&& piece : configuration.allChessPieces)
	{
		std::vector allowedDestinations = piece.get()->getPossibleDestinations(configuration);
		for (auto&& destination : allowedDestinations)
		{
			if (isValidPiecePosition(destination))
			{
				if (destination == kingPos)
					return true;
			}
		}
	}

	return false;
}

void GameStateManager::verifyCheckAndCheckmate()
{
	if (isPlayerInCheckmate(!isPlayer1Turn))
	{
		emit onVerifyCheckmate(!isPlayer1Turn);
		return;
	}
	else if (isPlayerInCheckmate(isPlayer1Turn))
	{
		emit onVerifyCheckmate(isPlayer1Turn);
		return;
	}

	if (isKingInCheckWithBoardConfiguration(!isPlayer1Turn, piecesList))
		emit onVerifyKingInCheck(!isPlayer1Turn);
	else if (isKingInCheckWithBoardConfiguration(isPlayer1Turn, piecesList))
		emit onVerifyKingInCheck(isPlayer1Turn);
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
