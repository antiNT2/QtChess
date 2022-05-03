#include "UnitTestingChess.h"
#include "KingPiece.h"
#include "TowerPiece.h"
#include "KnightPiece.h"
#include "BishopPiece.h"

#if __has_include("gtest/gtest.h")
#include "gtest/gtest.h"
#endif

#ifdef TEST

using namespace ChessPiecesData;
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

TEST(Chess, TestCheckmate)
{
	EXPECT_EQ(UnitTestingChess::testPlayerCheckmate(), true);
}

TEST(Chess, TestVerifyCheckAndCheckmate)
{
	EXPECT_EQ(UnitTestingChess::testVerifyCheckAndCheckmate(), true);
}

TEST(Chess, TestSetCurrentAllowedDestinations)
{
	EXPECT_EQ(UnitTestingChess::testSetCurrentAllowedDestinations(), true);
}

TEST(Chess, TestIsPositionIncludedInCurrentAllowedPos)
{
	EXPECT_EQ(UnitTestingChess::testIsPositionIncludedInCurrentAllowedPos(), true);
}

TEST(Chess, TestCheckKingPieceCounter)
{
	EXPECT_EQ(UnitTestingChess::testCheckKingPieceCounter(), true);
}

TEST(Chess, TestDestroyPiece)
{
	EXPECT_EQ(UnitTestingChess::testDestroyPiece(), true);
}

TEST(Chess, TestEqualsOperator)
{
	EXPECT_EQ(UnitTestingChess::testEqualsOperator(), true);
}

bool UnitTestingChess::testIsValidPiecePosition()
{
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

bool UnitTestingChess::testPlayerCheckmate()
{
	GameStateManager gameStateManager = GameStateManager();

	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(4, 7), true);

	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(4, 0), false);

	if (gameStateManager.isPlayerInCheckmate(true))
		return false;

	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(4, 5), false);

	if (gameStateManager.isPlayerInCheckmate(true))
		return false;

	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(3, 5), false);
	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(5, 5), false);

	if (gameStateManager.isPlayerInCheckmate(false))
		return false;

	if (!gameStateManager.isPlayerInCheckmate(true))
		return false;

	return true;
}

bool UnitTestingChess::testSetCurrentAllowedDestinations()
{
	GameStateManager gameStateManager = GameStateManager();

	// Case 1: A piece (bishop) that can move freely wherever it wants on the boards
	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(2, 2), true);
	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(7, 7), false);
	gameStateManager.instantiatePiece<BishopPiece>(PiecePosition(2, 3), true);

	gameStateManager.selectPiece(gameStateManager.piecesList.getPieceAtPosition(PiecePosition(2, 3)));

	gameStateManager.setCurrentAllowedDestinations(gameStateManager.piecesList.getPieceAtPosition(PiecePosition(2, 3))->getPossibleDestinations(gameStateManager.piecesList));

	if (gameStateManager.currentAllowedDestinations.size() != 11)
		return false;

	gameStateManager.deselectCurrentPiece();
	gameStateManager.resetBoard();

	// Case 2: A piece (knight) that has certain possible destinations blocked by allied pieces and that cannot go out of bounds
	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(0, 7), true);
	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(7, 7), false);
	gameStateManager.instantiatePiece<KnightPiece>(PiecePosition(1, 3), true);
	gameStateManager.instantiatePiece<BishopPiece>(PiecePosition(0, 1), true);
	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(2, 5), false);

	gameStateManager.selectPiece(gameStateManager.piecesList.getPieceAtPosition(PiecePosition(1, 3)));

	gameStateManager.setCurrentAllowedDestinations(gameStateManager.piecesList.getPieceAtPosition(PiecePosition(1, 3))->getPossibleDestinations(gameStateManager.piecesList));

	if (gameStateManager.currentAllowedDestinations.size() != 5)
		return false;

	gameStateManager.deselectCurrentPiece();
	gameStateManager.resetBoard();

	// Case 3: A piece (tower) that has certain possible destinations blocked because of discovery check
	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(2, 2), true);
	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(7, 7), false);
	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(2, 3), true);
	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(2, 7), false);

	gameStateManager.selectPiece(gameStateManager.piecesList.getPieceAtPosition(PiecePosition(2, 3)));

	gameStateManager.setCurrentAllowedDestinations(gameStateManager.piecesList.getPieceAtPosition(PiecePosition(2, 3))->getPossibleDestinations(gameStateManager.piecesList));

	if (gameStateManager.currentAllowedDestinations.size() != 3)
		return false;

	gameStateManager.deselectCurrentPiece();
	gameStateManager.resetBoard();

	// Case 4: There is no piece currently selected
	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(2, 2), true);
	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(7, 7), false);

	gameStateManager.setCurrentAllowedDestinations(gameStateManager.piecesList.getPieceAtPosition(PiecePosition(2, 2))->getPossibleDestinations(gameStateManager.piecesList));

	if (gameStateManager.currentAllowedDestinations.size() != 0)
		return false;

	gameStateManager.resetBoard();

	return true;
}

bool UnitTestingChess::testVerifyCheckAndCheckmate()
{
	GameStateManager gameStateManager = GameStateManager();

	gameStateManager.instantiatePieceForBothSides<KingPiece>(PiecePosition(4, 7));

	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(4, 5), false);
	gameStateManager.verifyCheckAndCheckmate();

	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(4, 2), true);
	gameStateManager.verifyCheckAndCheckmate();

	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(3, 5), false);
	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(5, 5), false);
	gameStateManager.verifyCheckAndCheckmate();

	return true;
}

bool UnitTestingChess::testCheckKingPieceCounter()
{
	GameStateManager gameStateManager = GameStateManager();

	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(4, 5), true);
	auto king1 = gameStateManager.piecesList.getPieceAtPosition(PiecePosition(4, 5));

	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(4, 7), false);
	auto king2 = gameStateManager.piecesList.getPieceAtPosition(PiecePosition(4, 7));

	gameStateManager.checkKingPieceCounter(king2, false);
	gameStateManager.checkKingPieceCounter(king1, true);

	if (gameStateManager.kingPieceCounter != 2)
		return false;

	return true;
}

bool UnitTestingChess::testInstantiateInitialPieces()
{
	GameStateManager gameStateManager = GameStateManager();

	gameStateManager.instantiateInitialPieces(GameStateManager::InitialBoardPiecesPosition::Default);

	if(gameStateManager.piecesList.allChessPieces.size() != 14)
		return false;

	gameStateManager.resetBoard();
	gameStateManager.instantiateInitialPieces(GameStateManager::InitialBoardPiecesPosition::MoreRooks);
	
	if(gameStateManager.piecesList.allChessPieces.size() != 14)
		return false;

	gameStateManager.resetBoard();
	gameStateManager.instantiateInitialPieces(GameStateManager::InitialBoardPiecesPosition::NoKnights);
	
	if(gameStateManager.piecesList.allChessPieces.size() != 12)
		return false;

	return true;
}

bool UnitTestingChess::testIsPositionIncludedInCurrentAllowedPos()
{
	GameStateManager gameStateManager = GameStateManager();

	if (gameStateManager.isPositionIncludedInCurrentAllowedPos(PiecePosition(1, 1)))
		return false;

	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(2, 2), true);
	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(7, 7), false);

	gameStateManager.selectPiece(gameStateManager.piecesList.getPieceAtPosition(PiecePosition(2, 2)));

	gameStateManager.currentAllowedDestinations.push_back(PiecePosition(2, 1));

	if (!gameStateManager.isPositionIncludedInCurrentAllowedPos(PiecePosition(2, 1)))
		return false;

	return true;
}

bool UnitTestingChess::testDestroyPiece()
{
	GameStateManager gameStateManager = GameStateManager();
	
	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(2, 2), true);
	gameStateManager.instantiatePiece<KingPiece>(PiecePosition(7, 7), false);
	gameStateManager.instantiatePiece<TowerPiece>(PiecePosition(0, 0), true);

	gameStateManager.destroyPiece(gameStateManager.piecesList.getPieceAtPosition(PiecePosition(0, 0)));

	if (gameStateManager.piecesList.allChessPieces.size() != 2
		|| gameStateManager.piecesList.getPieceAtPosition(PiecePosition(0, 0)) != nullptr)
		return false;



	return true;
}

bool UnitTestingChess::testEqualsOperator()
{
	GameStateManager gameStateManager1 = GameStateManager();

	gameStateManager1.isPlayer1Turn = false;
	gameStateManager1.instantiatePiece<KnightPiece>(PiecePosition(0, 0), false);

	GameStateManager gameStateManager2 = GameStateManager();
	gameStateManager2 = gameStateManager1;

	if (gameStateManager2.isPlayer1Turn != false)
		return false;
	if (gameStateManager2.piecesList.getPieceAtPosition(PiecePosition(0, 0)) == nullptr)
		return false;

	return true;
}

#endif
