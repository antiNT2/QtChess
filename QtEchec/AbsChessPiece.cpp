#include "AbsChessPiece.h"
namespace ChessPiecesData 
{

	AbsChessPiece::AbsChessPiece(PiecePosition initialPos, bool _isPlayer1) : position(initialPos), isPlayer1(_isPlayer1)
	{
	}

	PiecePosition AbsChessPiece::getPiecePosition()
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


	const std::shared_ptr<AbsChessPiece> ChessPiecesHolder::getPieceAtPosition(PiecePosition pos)
	{
		if (pos.gridX > 8 || pos.gridY < 0) // check if out of bounds
			return nullptr;

		for (auto&& piece : allChessPieces)
		{
			if (piece.get()->getPiecePosition() == pos)
			{
				return piece;
			}
		}

		return nullptr;
	}
}
