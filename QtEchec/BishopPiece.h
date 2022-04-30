#pragma once
#include "AbsChessPiece.h"
namespace ChessPiecesData
{
	class BishopPiece :
		public AbsChessPiece
	{
	public:
		BishopPiece(PiecePosition initialPos, bool _isPlayer1);
		virtual std::vector<PiecePosition> getPossibleDestinations(ChessPiecesHolder currentPieces) override;
		virtual const PieceType getPieceName() override;
	};
}

