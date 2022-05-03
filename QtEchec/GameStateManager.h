#pragma once
#include "AbsChessPiece.h"
#include "UnitTestingChess.h"

#include <QObject>
using std::vector;
using std::shared_ptr;

using ChessPiecesData::AbsChessPiece;

class GameStateManager : public QObject
{
	Q_OBJECT

public:
	GameStateManager();
	~GameStateManager();

	enum class InitialBoardPiecesPosition
	{
		Default,
		MoreRooks,
		NoKnights
	};

	void instantiateInitialPieces(InitialBoardPiecesPosition initialBoardPiecesPosition = InitialBoardPiecesPosition::Default); //Tested
	void selectPiece(const shared_ptr<AbsChessPiece> pieceToSelect); //Tested
	void deselectCurrentPiece(); //Tested
	void moveCurrentPiece(ChessPiecesData::PiecePosition destination); //Tested
	void resetBoard(); //Tested
	void operator=(const GameStateManager& other);

	ChessPiecesData::ChessPiecesHolder piecesList;
	friend class UnitTestingChess;

private:
	bool isPlayer1Turn = true;
	void setCurrentAllowedDestinations(std::vector<ChessPiecesData::PiecePosition> allowedDestinations); //Tested
	bool isValidPiecePosition(ChessPiecesData::PiecePosition pos); //Tested
	bool isPositionIncludedInCurrentAllowedPos(ChessPiecesData::PiecePosition pos);  //Tested
	bool movePiece(const std::shared_ptr<AbsChessPiece> pieceToMove, ChessPiecesData::PiecePosition destination); //Tested
	void destroyPiece(const std::shared_ptr<AbsChessPiece> pieceToDestroy); //Tested
	template<typename T> void instantiatePieceForBothSides(ChessPiecesData::PiecePosition position); //Tested
	template<typename T> void instantiatePiece(ChessPiecesData::PiecePosition position, bool isPlayer1); //Tested
	bool isPlayerInCheckmate(bool player1); //Tested
	bool isKingInCheckWithBoardConfiguration(bool player1King, ChessPiecesData::ChessPiecesHolder configuration); //Tested
	void verifyCheckAndCheckmate(); //Tested
	void checkKingPieceCounter(shared_ptr<AbsChessPiece> pieceToCheck, bool remove); //Tested

	shared_ptr<AbsChessPiece> currentSelectedPiece;
	std::vector<ChessPiecesData::PiecePosition> currentAllowedDestinations;
	static int kingPieceCounter;



signals:
	void onNoPieceSelected();
	void onIllegalMoveChosen();
	void onInstantiatePiece(const std::shared_ptr<AbsChessPiece> newPiece);
	void onRemoveAllowedDestination(int x, int y);
	void onAddAllowedDestination(int x, int y);
	void onSelectPiece(int x, int y);
	void onDeselectPiece(int x, int y);
	void onPieceMoved(const std::shared_ptr<AbsChessPiece> pieceToMove, int gridX, int gridY);
	void onRemovedPiece(const std::shared_ptr<AbsChessPiece> removedPiece);
	void onChangePlayerTurn(bool isPlayer1Turn);
	void onVerifyKingInCheck(bool isPlayer1King);
	void onVerifyCheckmate(bool isPlayer1King);
	void onResetBoard();
	void onTooManyKings();
};
