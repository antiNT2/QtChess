#pragma once
#include "AbsChessPiece.h"

class KingPiece :
	public AbsChessPiece
{
public:
	KingPiece(PiecePosition initialPos, bool _isPlayer1);
	virtual std::vector<Trajectory> getPossibleDestinations() override;
	virtual const std::string getPieceName() override;
};

