#pragma once
#include "GameStateManager.h"

class UnitTestingChess
{
public:
	static bool testIsValidPiecePosition();
	static bool testInstantiatePiece();
	static bool testInstantiatePieceForBothSides();
	static bool testResetBoard();
	static bool testSelectPiece();
	static bool testMoveCurrentPiece();
	static bool testKingInCheck();
	static bool testPlayerCheckmate();
	static bool testSetCurrentAllowedDestinations();
	static bool testVerifyCheckAndCheckmate();
	static bool testCheckKingPieceCounter();
	static bool testInstantiateInitialPieces();
	static bool testIsPositionIncludedInCurrentAllowedPos();
	static bool testDestroyPiece();
	static bool testEqualsOperator();
};