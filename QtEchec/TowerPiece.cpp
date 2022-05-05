
#include "TowerPiece.h"

namespace ChessPiecesData
{
	ChessPiecesData::TowerPiece::TowerPiece(PiecePosition initialPos, bool _isPlayer1) : AbsChessPiece(initialPos, _isPlayer1)
	{
	}

	std::vector<PiecePosition> TowerPiece::getPossibleDestinations(ChessPiecesHolder currentPieces)
	{
		std::vector<PiecePosition> output;

		bool continueCheckingUp = true;
		bool continueCheckingDown = true;
		bool continueCheckingLeft = true;
		bool continueCheckingRight = true;

		for (int i = 1; i < NB_OF_ROWS; i++) //we start i at 1 because we dont want to check ourselves
		{
			//Up
			if (continueCheckingUp)
				continueCheckingUp = checkIfContinueChecking(PiecePosition::NO_TRANSLATION, PiecePosition::UP * i, output, currentPieces);

			//Down
			if (continueCheckingDown)
				continueCheckingDown = checkIfContinueChecking(PiecePosition::NO_TRANSLATION, PiecePosition::DOWN * i, output, currentPieces);

			//Left
			if (continueCheckingLeft)
				continueCheckingLeft = checkIfContinueChecking(PiecePosition::LEFT * i, PiecePosition::NO_TRANSLATION, output, currentPieces);

			//Right
			if (continueCheckingRight)
				continueCheckingRight = checkIfContinueChecking(PiecePosition::RIGHT * i, PiecePosition::NO_TRANSLATION, output, currentPieces);
		}


		return output;
	}

	bool TowerPiece::checkIfContinueChecking(int x, int y, std::vector<PiecePosition>& output, ChessPiecesHolder currentPieces)
	{
		return checkValidPosDefault(PiecePosition(x, y) + getPiecePosition(), output, currentPieces) == FoundPieceAfterCheck::EmptySpace;
	}

	const PieceType TowerPiece::getPieceName()
	{
		return PieceType::Rook;
	}

}
