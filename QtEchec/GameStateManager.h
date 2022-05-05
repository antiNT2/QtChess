/**
* Chess model that handles all the chess game logic
* \file   GameStateManager.h | GameStateManager.cpp
* \author BEN RHOUMA Ahmed and KEZOUH Ryan
* \date   April 6th 2022
*/

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

	void instantiateInitialPieces(const InitialBoardPiecesPosition initialBoardPiecesPosition = InitialBoardPiecesPosition::Default); 
	void selectPiece(const shared_ptr<AbsChessPiece> pieceToSelect); 
	void deselectCurrentPiece(); 
	void moveCurrentPiece(const ChessPiecesData::PiecePosition destination); 
	void resetBoard(); 
	void operator=(const GameStateManager& other); 

	ChessPiecesData::ChessPiecesHolder piecesList;
	friend class UnitTestingChess;

private:
	bool isPlayer1Turn = true;
	void setCurrentAllowedDestinations(const std::vector<ChessPiecesData::PiecePosition> allowedDestinations); 
	bool isValidPiecePosition(const ChessPiecesData::PiecePosition position); 
	bool isPositionIncludedInCurrentAllowedPos(const ChessPiecesData::PiecePosition position);  
	bool movePiece(const std::shared_ptr<AbsChessPiece> pieceToMove, const ChessPiecesData::PiecePosition destination); 
	void destroyPiece(const std::shared_ptr<AbsChessPiece> pieceToDestroy); 
	template<typename T> void instantiatePieceForBothSides(const ChessPiecesData::PiecePosition position); 
	template<typename T> void instantiatePiece(const ChessPiecesData::PiecePosition position, bool isPlayer1); 
	bool isPlayerInCheckmate(bool player1); 
	bool isKingInCheckWithBoardConfiguration(bool player1King, ChessPiecesData::ChessPiecesHolder configuration); 
	void verifyCheckAndCheckmate(); 
	void checkKingPieceCounter(const shared_ptr<AbsChessPiece> pieceToCheck, bool remove); 

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
