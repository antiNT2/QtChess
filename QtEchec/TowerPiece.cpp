#include "TowerPiece.h"
#include <QDebug>
namespace ChessPiecesData
{
	ChessPiecesData::TowerPiece::TowerPiece(PiecePosition initialPos, bool _isPlayer1) : AbsChessPiece(initialPos, _isPlayer1)
	{
	}

	std::vector<PiecePosition> TowerPiece::getPossibleDestinations(ChessPiecesHolder currentPieces)
	{
		std::vector<PiecePosition> output;

		const int boardLength = 8;

		bool continueCheckingUp = true;
		bool continueCheckingDown = true;
		bool continueCheckingLeft = true;
		bool continueCheckingRight = true;

		for (int i = 1; i < boardLength; i++) //we start i at 1 because we dont want to check ourselves
		{
			//Up
			if (continueCheckingUp)
				continueCheckingUp = checkValidPosDefault(PiecePosition(0, -i) + getPiecePosition(), output, currentPieces);

			qDebug() << "Up " << continueCheckingUp << " | " << (PiecePosition(0, -i) + getPiecePosition()).print().c_str();

			//Down
			if (continueCheckingDown)
				continueCheckingDown = checkValidPosDefault(PiecePosition(0, i) + getPiecePosition(), output, currentPieces);

			//Left
			if (continueCheckingLeft)
				continueCheckingLeft = checkValidPosDefault(PiecePosition(-i, 0) + getPiecePosition(), output, currentPieces);

			//Right
			if (continueCheckingRight)
				continueCheckingRight = checkValidPosDefault(PiecePosition(i, 0) + getPiecePosition(), output, currentPieces);
		}


		return output;
	}

	const std::string TowerPiece::getPieceName()
	{
		return "Tower";
	}

}
