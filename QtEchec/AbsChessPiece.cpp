#include "AbsChessPiece.h"
namespace ChessPiecesData
{

	AbsChessPiece::AbsChessPiece(PiecePosition initialPos, bool _isPlayer1) : position(initialPos), isPlayer1(_isPlayer1)
	{
	}

	const PiecePosition AbsChessPiece::getPiecePosition()
	{
		return position;
	}

	void AbsChessPiece::setPiecePosition(PiecePosition pos)
	{
		position = pos;
	}

	const bool AbsChessPiece::isPlayer1Piece()
	{
		return isPlayer1;
	}

	AbsChessPiece::FoundPieceAfterCheck AbsChessPiece::checkValidPosDefault(const PiecePosition inspectedPos, std::vector<PiecePosition>& outputToAddTo, const ChessPiecesHolder& allPieces)
	{
		auto inspectedPiece = allPieces.getPieceAtPosition(inspectedPos);

		if (inspectedPiece != nullptr)
		{
			if (inspectedPiece.get()->isPlayer1Piece() != isPlayer1Piece()) //we only eat enemy pieces
			{
				outputToAddTo.push_back(inspectedPos);
				return FoundPieceAfterCheck::EnemyPiece;
			}
			else
			{
				return FoundPieceAfterCheck::FriendlyPiece;
			}
		}
		else //we can move to empty spaces
		{
			outputToAddTo.push_back(inspectedPos);
			return FoundPieceAfterCheck::EmptySpace;
		}


	}

	const std::shared_ptr<AbsChessPiece> ChessPiecesHolder::getPieceAtPosition(const PiecePosition pos) const
	{
		if (pos.gridX > 8 || pos.gridY < 0) // check if out of bounds
			return nullptr;

		for (auto&& piece : allChessPieces)
		{
			if (piece.get()->getPiecePosition() == pos)
			{
				return piece;
			}
		}

		return nullptr;
	}

	void ChessPiecesHolder::removePiece(const std::shared_ptr<AbsChessPiece> pieceToRemove)
	{
		int foundIndex = -1;
		for (int i = 0; i < allChessPieces.size(); i++)
		{
			if (allChessPieces[i] == pieceToRemove)
			{
				foundIndex = i;
				break;
			}
		}

		allChessPieces.erase(allChessPieces.begin() + foundIndex);
	}
}
