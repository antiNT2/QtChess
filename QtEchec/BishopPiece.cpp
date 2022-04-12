#include "BishopPiece.h"
namespace ChessPiecesData
{
	BishopPiece::BishopPiece(PiecePosition initialPos, bool _isPlayer1) : AbsChessPiece(initialPos, _isPlayer1)
	{
	}

	std::vector<PiecePosition> BishopPiece::getPossibleDestinations(ChessPiecesHolder currentPieces)
	{
		std::vector<PiecePosition> output;

		PiecePosition inspectedPos = getPiecePosition();

		const int boardLength = 8;

		bool continueCheckingUpRight = true;
		bool continueCheckingDownRight = true;
		bool continueCheckingUpLeft = true;
		bool continueCheckingDownLeft = true;

		for (int i = 1; i < boardLength; i++) //we start i at 1 because we dont want to check ourselves
		{
			//Up Right
			if (continueCheckingUpRight)
				continueCheckingUpRight = checkValidPosDefault(PiecePosition(i, -i) + getPiecePosition(), output, currentPieces) == FoundPieceAfterCheck::EmptySpace;

			//Down Right
			if (continueCheckingDownRight)
				continueCheckingDownRight = checkValidPosDefault(PiecePosition(i, i) + getPiecePosition(), output, currentPieces) == FoundPieceAfterCheck::EmptySpace;

			//Up Left
			if (continueCheckingUpLeft)
				continueCheckingUpLeft = checkValidPosDefault(PiecePosition(-i, -i) + getPiecePosition(), output, currentPieces) == FoundPieceAfterCheck::EmptySpace;

			//Down Left
			if (continueCheckingDownLeft)
				continueCheckingDownLeft = checkValidPosDefault(PiecePosition(-i, i) + getPiecePosition(), output, currentPieces) == FoundPieceAfterCheck::EmptySpace;
		}

		return output;
	}

	const std::string BishopPiece::getPieceName()
	{
		return "Bishop";
	}
}
