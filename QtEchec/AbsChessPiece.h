#pragma once

#include <vector>
#include <string>
#include <memory>

namespace ChessPiecesData
{
	struct PiecePosition
	{
		static const int UP = -1;
		static const int DOWN = 1;

		static const int LEFT = -1;
		static const int RIGHT = 1;

		static const int NO_TRANSLATION = 0;

		PiecePosition(int _gridX = 0, int _gridY = 0)
		{
			gridX = _gridX;
			gridY = _gridY;
		}

		PiecePosition translated(int dx, int dy)
		{
			return PiecePosition(gridX + dx, gridY + dy);
		}

		const bool operator==(const PiecePosition& other) const
		{
			return gridX == other.gridX && gridY == other.gridY;
		}

		const bool operator!=(const PiecePosition& other) const
		{
			return !(*this == other);
		}

		PiecePosition operator+(const PiecePosition& other)
		{
			return PiecePosition(gridX + other.gridX, gridY + other.gridY);
		}

		std::string print()
		{
			return ("x: " + std::to_string(gridX) + " y :" + std::to_string(gridY));
		}

		int gridX;
		int gridY;
	};

	enum class PieceType
	{
		None,
		King,
		Bishop,
		Knight,
		Rook
	};

	const int NB_OF_ROWS = 8;
	const int NB_OF_COLUMNS = 8;

	class ChessPiecesHolder;

	class AbsChessPiece
	{
	public:
		AbsChessPiece(PiecePosition initialPos, bool isPlayer1);
		const PiecePosition getPiecePosition();
		void setPiecePosition(PiecePosition pos);
		const bool isPlayer1Piece();

		virtual const PieceType getPieceName() = 0;
		virtual std::vector<PiecePosition> getPossibleDestinations(ChessPiecesHolder currentPieces) = 0;

	private:
		PiecePosition position;
		bool isPlayer1;
	protected:
		enum class FoundPieceAfterCheck
		{
			EnemyPiece,
			FriendlyPiece,
			EmptySpace
		};
		FoundPieceAfterCheck checkValidPosDefault(const PiecePosition inspectedPos, std::vector<PiecePosition>& outputToAddTo, const ChessPiecesHolder& allPieces);
	};

	class ChessPiecesHolder
	{
	public:
		const std::shared_ptr<AbsChessPiece> getPieceAtPosition(const PiecePosition pos) const;
		const std::shared_ptr<AbsChessPiece> getKingPiece(bool player1King);
		void removePiece(const std::shared_ptr<AbsChessPiece> pieceToRemove);
		std::vector<std::shared_ptr<AbsChessPiece>> allChessPieces;
	};
}



