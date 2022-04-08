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

		//Up Right
		checkValidPosDefault((inspectedPos.translated(1,-2)), output, currentPieces);
		checkValidPosDefault((inspectedPos.translated(2,-1)), output, currentPieces);

		//Up Left
		checkValidPosDefault((inspectedPos.translated(-1, -2)), output, currentPieces);
		checkValidPosDefault((inspectedPos.translated(-2, -1)), output, currentPieces);

		//Down Right
		checkValidPosDefault((inspectedPos.translated(1, 2)), output, currentPieces);
		checkValidPosDefault((inspectedPos.translated(2, 1)), output, currentPieces);

		//Down Left
		checkValidPosDefault((inspectedPos.translated(-1, 2)), output, currentPieces);
		checkValidPosDefault((inspectedPos.translated(-2, 1)), output, currentPieces);

		return output;
	}

	const std::string KnightPiece::getPieceName()
	{
		return "Knight";
	}
}
