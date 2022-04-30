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
        virtual const PieceType getPieceName() override;
    private:
        bool checkIfContinueChecking(int x, int y, std::vector<PiecePosition>& output, ChessPiecesHolder currentPieces);
    };
}

