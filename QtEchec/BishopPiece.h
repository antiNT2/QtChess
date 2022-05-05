/**
* Bishop piece behaviour
* \file   BishopPiece.h | BishopPiece.cpp
* \author BEN RHOUMA Ahmed and KEZOUH Ryan
* \date   April 6th 2022
*/

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

