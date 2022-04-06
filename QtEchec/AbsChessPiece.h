#pragma once
#include <vector>
#include <string>
#include<math.h>  

class AbsChessPiece
{
public:
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

		bool operator==(const PiecePosition& rhs)
		{
			return gridX == rhs.gridX && gridY == rhs.gridY;
		}

		int gridX;
		int gridY;
	};
	class Trajectory
	{
	public:
		Trajectory(PiecePosition _start, PiecePosition _finish)
		{
			start = _start;
			finish = _finish;

			directionX = finish.gridX - start.gridX;
			directionX = (directionX > 0) - (directionX < 0); //cpp magic to get the sign of the number

			directionY = finish.gridY - start.gridY;
			directionY = (directionY > 0) - (directionY < 0); //cpp magic to get the sign of the number
		}
		Trajectory(PiecePosition _start, int translationFromStartX, int translationFromStartY)
		{
			*this = Trajectory(_start, _start.translated(translationFromStartX, translationFromStartY));
		}

		const PiecePosition getStart()
		{
			return start;
		}

		const PiecePosition getFinish()
		{
			return finish;
		}

		const int getDirectionX()
		{
			return directionX;
		}

		const int getDirectionY()
		{
			return directionX;
		}

	private:
		PiecePosition start;
		PiecePosition finish;
		int directionX = 0;
		int directionY = 0;
	};
	AbsChessPiece(PiecePosition initialPos, bool isPlayer1);
	PiecePosition getPiecePosition();
	void setPiecePosition(PiecePosition pos);
	const bool isPlayer1Piece();

	virtual const std::string getPieceName() = 0;
	virtual std::vector<Trajectory> getPossibleDestinations() = 0;

private:
	PiecePosition position;
	bool isPlayer1;
};



