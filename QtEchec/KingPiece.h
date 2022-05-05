/**
* King piece behaviour
* \file   KingPiece.h | KingPiece.cpp
* \author BEN RHOUMA Ahmed and KEZOUH Ryan
* \date   April 6th 2022
*/

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
		virtual const PieceType getPieceName() override;
	};
}

