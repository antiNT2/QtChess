#pragma once
#include "AbsChessPiece.h"

namespace ChessPiecesData
{
    class TowerPiece :
        public AbsChessPiece
    {
    public:
        TowerPiece(PiecePosition initialPos, bool _isPlayer1);
        virtual std::vector<PiecePosition> getPossibleDestinations(ChessPiecesHolder currentPieces) override;
        virtual const std::string getPieceName() override;
    };
}

