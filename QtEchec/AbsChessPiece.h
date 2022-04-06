#pragma once

#include <vector>
#include <string>
#include <memory>

namespace ChessPiecesData
{
	struct PiecePosition
	{
		PiecePosition(int _gridX = 0, int _gridY = 0)
		{
			gridX = _gridX;
			gridY = _gridY;
		}

		PiecePosition translated(int dx, int dy)
		{
			return PiecePosition(gridX + dx, gridY + dy);
		}

		bool operator==(const PiecePosition& other)
		{
			return gridX == other.gridX && gridY == other.gridY;
		}

		bool operator!=(const PiecePosition& other)
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

	class ChessPiecesHolder;

	class AbsChessPiece
	{
	public:
		AbsChessPiece(PiecePosition initialPos, bool isPlayer1);
		PiecePosition getPiecePosition();
		void setPiecePosition(PiecePosition pos);
		const bool isPlayer1Piece();

		virtual const std::string getPieceName() = 0;
		//virtual std::vector<Trajectory> getPossibleDestinations() = 0;
		virtual std::vector<PiecePosition> getPossibleDestinations(ChessPiecesHolder currentPieces) = 0;

	private:
		PiecePosition position;
		bool isPlayer1;
	};

	class ChessPiecesHolder
	{
	public:
		const std::shared_ptr<AbsChessPiece> getPieceAtPosition(PiecePosition pos);
		std::vector<std::shared_ptr<AbsChessPiece>> allChessPieces;
	};
}



