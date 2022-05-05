/**
* Google unit tests for the chess model
* \file   UnitTestingChess.h | UnitTestingChess.cpp
* \author BEN RHOUMA Ahmed and KEZOUH Ryan
* \date   April 6th 2022
*/

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