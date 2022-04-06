#pragma once
#include "AbsChessPiece.h"

namespace ChessPiecesData
{
	class KingPiece :
		public AbsChessPiece
	{
	public:
		KingPiece(PiecePosition initialPos, bool _isPlayer1);
		virtual std::vector<PiecePosition> getPossibleDestinations(ChessPiecesHolder currentPieces) override;
		virtual const std::string getPieceName() override;
	};
}

