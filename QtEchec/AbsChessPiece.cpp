#include "AbsChessPiece.h"

AbsChessPiece::AbsChessPiece(PiecePosition initialPos, bool _isPlayer1) : position(initialPos), isPlayer1(_isPlayer1)
{
}

AbsChessPiece::PiecePosition AbsChessPiece::getPiecePosition()
{
	return position;
}

void AbsChessPiece::setPiecePosition(PiecePosition pos)
{
	position = pos;
}

const bool AbsChessPiece::isPlayer1Piece()
{
	return isPlayer1;
}
