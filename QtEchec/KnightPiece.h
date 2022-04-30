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

