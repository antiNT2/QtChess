#include "KingPiece.h"

KingPiece::KingPiece(PiecePosition initialPos, bool _isPlayer1) : AbsChessPiece(initialPos, _isPlayer1)
{
}

std::vector<AbsChessPiece::Trajectory> KingPiece::getPossibleDestinations()
{
	Trajectory up = Trajectory(getPiecePosition(), 0, -1);
	Trajectory down = Trajectory(getPiecePosition(), 0, 1);
	Trajectory left = Trajectory(getPiecePosition(), -1, 0);
	Trajectory right = Trajectory(getPiecePosition(), 1, 0);

	Trajectory bottomLeft = Trajectory(getPiecePosition(), -1, 1);
	Trajectory bottomRight = Trajectory(getPiecePosition(), 1, 1);
	Trajectory upLeft = Trajectory(getPiecePosition(), -1, -1);
	Trajectory upRight = Trajectory(getPiecePosition(), 1, -1);

	std::vector<Trajectory> output;

	output.push_back(up);
	output.push_back(down);
	output.push_back(left);
	output.push_back(right);

	output.push_back(bottomLeft);
	output.push_back(bottomRight);
	output.push_back(upLeft);
	output.push_back(upRight);

	return output;
}

const std::string KingPiece::getPieceName()
{
	return "King";
}
