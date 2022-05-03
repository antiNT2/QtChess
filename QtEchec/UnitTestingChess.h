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
};

