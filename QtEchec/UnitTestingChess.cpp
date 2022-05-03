#include "UnitTestingChess.h"
#include "KingPiece.h"
#include "TowerPiece.h"
#include "KnightPiece.h"
#include "BishopPiece.h"

#if __has_include("gtest/gtest.h")
#include "gtest/gtest.h"
#endif

#ifdef TEST

//TEST(Chess, InstantiateInitialPieces)
//{
//	GameStateManager gameStateManager = GameStateManager();
//	gameStateManager.instantiateInitialPieces(GameStateManager::InitialBoardPiecesPosition::Default);
//
//	EXPECT_GT(gameStateManager.piecesList.allChessPieces.size(), 0);
//}

//TEST(Chess, InstantiateSinglePiece)
//{
//
//	GameStateManager gameStateManager = GameStateManager();
//	//	gameStateManager.
//
//		//EXPECT_GT(gameStateManager.piecesList.allChessPieces.size(), 0);
//}

TEST(Chess, TestValidPosition)
{
	EXPECT_EQ(UnitTestingChess::testIsValidPiecePosition(), true);
}

TEST(Chess, TestInstantiatePiece)
{
	EXPECT_EQ(UnitTestingChess::testInstantiatePiece(), true);
}

TEST(Chess, TestInstantiatePieceForBothSides)
{
	EXPECT_EQ(UnitTestingChess::testInstantiatePieceForBothSides(), true);
}

TEST(Chess, TestResetBoard)
{
	EXPECT_EQ(UnitTestingChess::testResetBoard(), true);
}

TEST(Chess, TestSelectPiece)
{
	EXPECT_EQ(UnitTestingChess::testSelectPiece(), true);
}

TEST(Chess, TestMoveCurrentPiece)
{
	EXPECT_EQ(UnitTestingChess::testMoveCurrentPiece(), true);
}

TEST(Chess, TestKingCheck)
{
	EXPECT_EQ(UnitTestingChess::testKingInCheck(), true);
}

bool UnitTestingChess::testIsValidPiecePosition()
{
	using namespace ChessPiecesData;
	GameStateManager gameStateManager = GameStateManager();

	PiecePosition validPosition = PiecePosition(4, 4);

	if (gameStateManager.isValidPiecePosition(validPosition) == false)
		return false;

	PiecePosition invalidPosition = PiecePosition(15, -2);

	if (gameStateManager.isValidPiecePosition(invalidPosition) == true)
		return false;


	return true;
}

bool UnitTestingChess::testInstantiatePiece()
{
	using namespace ChessPiecesData;
	GameStateManager gameStateManager = GameStateManager();

	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(2, 2), true);

	shared_ptr<AbsChessPiece> evaluatedPiece = gameStateManager.piecesList.getPieceAtPosition(PiecePosition(2, 2));
	if (evaluatedPiece != nullptr)
	{
		if (evaluatedPiece->isPlayer1Piece() && evaluatedPiece->getPieceName() == PieceType::King)
			return true;
	}

	return false;
}

bool UnitTestingChess::testInstantiatePieceForBothSides()
{
	using namespace ChessPiecesData;
	GameStateManager gameStateManager = GameStateManager();

	gameStateManager.instantiatePieceForBothSides<KingPiece>(PiecePosition(4, 7));

	if (gameStateManager.piecesList.getPieceAtPosition(PiecePosition(4, 7))->getPieceName() != PieceType::King)
		return false;

	if (gameStateManager.piecesList.getPieceAtPosition(PiecePosition(4, 0))->getPieceName() != PieceType::King)
		return false;

	return true;
}

bool UnitTestingChess::testResetBoard()
{
	using namespace ChessPiecesData;
	GameStateManager gameStateManager = GameStateManager();

	gameStateManager.instantiateInitialPieces();
	gameStateManager.resetBoard();

	if (gameStateManager.piecesList.allChessPieces.size() != 0)
		return false;

	if (gameStateManager.isPlayer1Turn == false)
		return false;

	if (gameStateManager.kingPieceCounter != 0)
		return false;

	return true;
}

bool UnitTestingChess::testSelectPiece()
{
	using namespace ChessPiecesData;
	GameStateManager gameStateManager = GameStateManager();

	PiecePosition ourInitialPos = PiecePosition(2, 2);
	gameStateManager.instantiatePiece<KingPiece>(ourInitialPos, true);
	auto ourPiece = gameStateManager.piecesList.getPieceAtPosition(ourInitialPos);

	PiecePosition enemyInitialPos = PiecePosition(2, 1);
	gameStateManager.instantiatePiece<KingPiece>(enemyInitialPos, false);
	auto enemyPiece = gameStateManager.piecesList.getPieceAtPosition(enemyInitialPos);

	gameStateManager.instantiatePiece<BishopPiece>(PiecePosition(5, 5), false);
	auto farAwayPiece = gameStateManager.piecesList.getPieceAtPosition(PiecePosition(5, 5));

	//Test selecting a piece even though its not our turn
	gameStateManager.selectPiece(enemyPiece);
	if (gameStateManager.currentSelectedPiece != nullptr)
		return false;

	gameStateManager.selectPiece(ourPiece);

	if (gameStateManager.currentSelectedPiece != ourPiece) //test select
		return false;

	gameStateManager.selectPiece(ourPiece); //we deselect it

	if (gameStateManager.currentSelectedPiece != nullptr)
		return false;

	gameStateManager.selectPiece(ourPiece); // we select it again to try to eat a faraway piece

	gameStateManager.selectPiece(farAwayPiece);

	if (gameStateManager.currentSelectedPiece != nullptr || ourPiece->getPiecePosition() != ourInitialPos)
		return false;

	gameStateManager.selectPiece(ourPiece); // we select it again to try to eat a close piece

	gameStateManager.selectPiece(enemyPiece);

	if (gameStateManager.currentSelectedPiece != nullptr || ourPiece->getPiecePosition() != enemyInitialPos)
		return false;

	return true;
}

bool UnitTestingChess::testMoveCurrentPiece()
{
	using namespace ChessPiecesData;
	GameStateManager gameStateManager = GameStateManager();

	PiecePosition initialPos = PiecePosition(2, 2);

	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(0, 0), false);
	gameStateManager.instantiatePiece<KingPiece>(initialPos, true);

	auto spawnedPiece = gameStateManager.piecesList.getPieceAtPosition(initialPos);

	gameStateManager.selectPiece(spawnedPiece);
	gameStateManager.moveCurrentPiece(PiecePosition(10, 10));

	if (spawnedPiece->getPiecePosition() != initialPos)
		return false;

	gameStateManager.deselectCurrentPiece();
	gameStateManager.moveCurrentPiece(PiecePosition(3, 3));

	if (spawnedPiece->getPiecePosition() != initialPos)
		return false;

	gameStateManager.selectPiece(spawnedPiece);
	gameStateManager.moveCurrentPiece(PiecePosition(3, 3));

	if (spawnedPiece->getPiecePosition() != PiecePosition(3, 3))
		return false;

	return true;
}

bool UnitTestingChess::testKingInCheck()
{
	using namespace ChessPiecesData;
	GameStateManager gameStateManager = GameStateManager();

	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(4, 4), true);
	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(7, 7), false);

	if (gameStateManager.isKingInCheckWithBoardConfiguration(true, gameStateManager.piecesList))
		return false;

	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(2, 4), false);
	
	if (!gameStateManager.isKingInCheckWithBoardConfiguration(true, gameStateManager.piecesList))
		return false;

	return true;
}

#endif
