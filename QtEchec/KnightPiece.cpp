#include "KnightPiece.h"

namespace ChessPiecesData
{
	KnightPiece::KnightPiece(PiecePosition initialPos, bool _isPlayer1) : AbsChessPiece(initialPos, _isPlayer1)
	{
	}

	std::vector<PiecePosition> KnightPiece::getPossibleDestinations(ChessPiecesHolder currentPieces)
	{
		std::vector<PiecePosition> output;

		PiecePosition inspectedPos = getPiecePosition();
		const int KNIGHT_MOVE_FACTOR = 2;

		//Up Right
		checkValidPosDefault((inspectedPos.translated(PiecePosition::RIGHT, KNIGHT_MOVE_FACTOR * PiecePosition::UP)), output, currentPieces);
		checkValidPosDefault((inspectedPos.translated(KNIGHT_MOVE_FACTOR * PiecePosition::RIGHT, PiecePosition::UP)), output, currentPieces);

		//Up Left
		checkValidPosDefault((inspectedPos.translated(PiecePosition::LEFT, KNIGHT_MOVE_FACTOR * PiecePosition::UP)), output, currentPieces);
		checkValidPosDefault((inspectedPos.translated(KNIGHT_MOVE_FACTOR * PiecePosition::LEFT, PiecePosition::UP)), output, currentPieces);

		//Down Right
		checkValidPosDefault((inspectedPos.translated(PiecePosition::RIGHT, KNIGHT_MOVE_FACTOR * PiecePosition::DOWN)), output, currentPieces);
		checkValidPosDefault((inspectedPos.translated(KNIGHT_MOVE_FACTOR * PiecePosition::RIGHT, PiecePosition::DOWN)), output, currentPieces);

		//Down Left
		checkValidPosDefault((inspectedPos.translated(PiecePosition::LEFT, KNIGHT_MOVE_FACTOR * PiecePosition::DOWN)), output, currentPieces);
		checkValidPosDefault((inspectedPos.translated(KNIGHT_MOVE_FACTOR * PiecePosition::LEFT, PiecePosition::DOWN)), output, currentPieces);

		return output;
	}

	const PieceType KnightPiece::getPieceName()
	{
		return PieceType::Knight;
	}
}
