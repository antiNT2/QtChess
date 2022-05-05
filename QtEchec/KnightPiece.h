/**
* Knight piece behaviour
* \file   KnightPiece.h | KnightPiece.cpp
* \author BEN RHOUMA Ahmed and KEZOUH Ryan
* \date   April 6th 2022
*/

#pragma once
#include "AbsChessPiece.h"

namespace ChessPiecesData
{
    class KnightPiece :
        public AbsChessPiece
    {
    public:
        KnightPiece(PiecePosition initialPos, bool _isPlayer1);
        virtual std::vector<PiecePosition> getPossibleDestinations(ChessPiecesHolder currentPieces) override;
        virtual const PieceType getPieceName() override;
    };
}

