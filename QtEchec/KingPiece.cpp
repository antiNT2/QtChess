#include "KingPiece.h"
namespace ChessPiecesData
{
	KingPiece::KingPiece(PiecePosition initialPos, bool _isPlayer1) : AbsChessPiece(initialPos, _isPlayer1)
	{
	}

	std::vector<PiecePosition> KingPiece::getPossibleDestinations(ChessPiecesHolder currentPieces)
	{
		std::vector<PiecePosition> output;

		const int NB_OF_POS = 8; //the king can move to 8 different positions, (no relation to board size or anything)


		PiecePosition positions[NB_OF_POS] = { 
			PiecePosition(PiecePosition::NO_TRANSLATION, PiecePosition::UP),
			PiecePosition(PiecePosition::NO_TRANSLATION,PiecePosition::DOWN), 
			PiecePosition(PiecePosition::RIGHT, PiecePosition::NO_TRANSLATION),
			PiecePosition(PiecePosition::LEFT, PiecePosition::NO_TRANSLATION),
			PiecePosition(PiecePosition::RIGHT, PiecePosition::UP),
			PiecePosition(PiecePosition::RIGHT, PiecePosition::DOWN),
			PiecePosition(PiecePosition::LEFT, PiecePosition::DOWN), 
			PiecePosition(PiecePosition::LEFT, PiecePosition::UP) 
		};

		for (int i = 0; i < NB_OF_POS; i++)
		{
			PiecePosition inspectedPos = positions[i] + getPiecePosition();
			checkValidPosDefault(inspectedPos, output, currentPieces);
		}

		return output;
	}

	const PieceType KingPiece::getPieceName()
	{
		return PieceType::King;
	}
}
