#include "KingPiece.h"
namespace ChessPiecesData
{
	KingPiece::KingPiece(PiecePosition initialPos, bool _isPlayer1) : AbsChessPiece(initialPos, _isPlayer1)
	{
	}

	std::vector<PiecePosition> KingPiece::getPossibleDestinations(ChessPiecesHolder currentPieces)
	{
		std::vector<PiecePosition> output;

		const int numberOfPos = 8;
		PiecePosition positions[numberOfPos] = { PiecePosition(0,-1), PiecePosition(0, 1), PiecePosition(1, 0)
		,PiecePosition(-1, 0), PiecePosition(1,-1), PiecePosition(1, 1), PiecePosition(-1, 1), PiecePosition(-1,-1) };

		for (int i = 0; i < numberOfPos; i++)
		{
			PiecePosition inspectedPos = positions[i] + getPiecePosition();
			checkValidPosDefault(inspectedPos, output, currentPieces);
			//auto inspectedPiece = currentPieces.getPieceAtPosition(inspectedPos);
			//if (inspectedPiece != nullptr)
			//{
			//	if (inspectedPiece.get()->isPlayer1Piece() != isPlayer1Piece()) //we only eat enemy pieces
			//	{
			//		output.push_back(inspectedPos);
			//	}
			//}
			//else //we can move to empty spaces
			//{
			//	output.push_back(inspectedPos);
			//}
		}

		return output;
	}

	const std::string KingPiece::getPieceName()
	{
		return "King";
	}
}
