#include "GameStateManager.h"
#include "KingPiece.h"
#include "TowerPiece.h"
#include "KnightPiece.h"
#include "BishopPiece.h"

#include<QDebug>
#include <stdexcept>

const int MAX_GRID_X = 7;
const int MIN_GRID_X = 0;

const int MAX_GRID_Y = 7;
const int MIN_GRID_Y = 0;

const int INITIAL_KING_PIECE_COUNTER = 0;

int GameStateManager::kingPieceCounter = 0;

GameStateManager::GameStateManager()
{
}

GameStateManager::~GameStateManager()
{
	resetBoard();
}

void GameStateManager::instantiateInitialPieces(InitialBoardPiecesPosition initialBoardPiecesPosition)
{
	using namespace ChessPiecesData;

	instantiatePieceForBothSides<KingPiece>(PiecePosition(4, 7));

	switch (initialBoardPiecesPosition)
	{
	case GameStateManager::InitialBoardPiecesPosition::Default:
		instantiatePieceForBothSides<TowerPiece>(PiecePosition(0, 7));
		instantiatePieceForBothSides<KnightPiece>(PiecePosition(1, 7));
		instantiatePieceForBothSides<KnightPiece>(PiecePosition(6, 7));
		instantiatePieceForBothSides<TowerPiece>(PiecePosition(7, 7));
		instantiatePieceForBothSides<BishopPiece>(PiecePosition(2, 7));
		instantiatePieceForBothSides<BishopPiece>(PiecePosition(5, 7));
		break;
	case GameStateManager::InitialBoardPiecesPosition::MoreRooks:
		instantiatePieceForBothSides<TowerPiece>(PiecePosition(0, 7));
		instantiatePieceForBothSides<TowerPiece>(PiecePosition(7, 7));

		instantiatePieceForBothSides<TowerPiece>(PiecePosition(2, 7));
		instantiatePieceForBothSides<TowerPiece>(PiecePosition(5, 7));
		instantiatePieceForBothSides<KnightPiece>(PiecePosition(1, 7));
		instantiatePieceForBothSides<KnightPiece>(PiecePosition(6, 7));
		break;
	case GameStateManager::InitialBoardPiecesPosition::NoKnights:
		instantiatePieceForBothSides<TowerPiece>(PiecePosition(0, 7));
		instantiatePieceForBothSides<TowerPiece>(PiecePosition(7, 7));
		instantiatePieceForBothSides<BishopPiece>(PiecePosition(2, 7));
		instantiatePieceForBothSides<BishopPiece>(PiecePosition(5, 7));
		break;
	default:
		break;
	}
			


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
	kingPieceCounter = INITIAL_KING_PIECE_COUNTER;
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
	return (pos.gridX <= MAX_GRID_X && pos.gridY <= MAX_GRID_Y) && (pos.gridX >= MIN_GRID_X && pos.gridY >= MIN_GRID_Y);
}

bool GameStateManager::isPositionIncludedInCurrentAllowedPos(ChessPiecesData::PiecePosition pos)
{
	if (currentAllowedDestinations.size() == 0)
		return false;

	for (auto p : currentAllowedDestinations)
	{
		if (pos == p)
			return true;
	}

	return false;
}

bool GameStateManager::movePiece(const std::shared_ptr<AbsChessPiece> pieceToMove, ChessPiecesData::PiecePosition destination)
{
	if (isValidPiecePosition(destination) == false || pieceToMove == nullptr)
		return false;

	pieceToMove.get()->setPiecePosition(destination);
	emit onPieceMoved(pieceToMove, destination.gridX, destination.gridY);

	verifyCheckAndCheckmate();

	return true;
}

void GameStateManager::destroyPiece(const std::shared_ptr<AbsChessPiece> pieceToDestroy)
{
	piecesList.removePiece(pieceToDestroy);
	checkKingPieceCounter(pieceToDestroy, true);

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

void GameStateManager::checkKingPieceCounter(shared_ptr<AbsChessPiece> pieceToCheck, bool remove)
{
	if (pieceToCheck.get()->getPieceName() == ChessPiecesData::PieceType::King)
	{
		if (remove)
			kingPieceCounter--;
		else
			kingPieceCounter++;

	}

	if (kingPieceCounter > 2)
		emit onTooManyKings();

}


template<typename T>
void GameStateManager::instantiatePieceForBothSides(ChessPiecesData::PiecePosition player1Position)
{
	using namespace ChessPiecesData;
	PiecePosition player2Position = PiecePosition(player1Position.gridX, MAX_GRID_Y - player1Position.gridY);

	instantiatePiece<T>(player1Position, true);
	instantiatePiece<T>(player2Position, false);
}

template<typename T>
void GameStateManager::instantiatePiece(ChessPiecesData::PiecePosition position, bool isPlayer1)
{
	static_assert(std::is_base_of<AbsChessPiece, T>::value);

	shared_ptr<AbsChessPiece> newChessPiece = std::make_shared<T>(position, isPlayer1);
	piecesList.allChessPieces.push_back(newChessPiece);

	checkKingPieceCounter(newChessPiece, false);

	emit onInstantiatePiece(newChessPiece);
}
