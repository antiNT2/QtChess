#include "QtEchec.h"
#include "DisplayManager.h"
#include "qscrollbar.h"
//#include "AbsChessPiece.h"
#include "GameStateManager.h"

//void clickChessCaseTest(DisplayManager& displayManager, Ui::QtEchecClass& ui, int x, int y)
//{
//	displayManager.movePieceToPosition(ui.piece, x, y);
//	displayManager.displayMessage(QString("Move : (x = %1, y = %2) \n").arg(x).arg(y));
//}

QtEchec::QtEchec(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	displayManager = DisplayManager(ui.chessFrame, &ui);
	displayManager.setUpChessUi();
	gameStateManager = GameStateManager(&displayManager);
	
	displayManager.displayMessage(QString("A great slam and then some! \n"));
	displayManager.displayMessage(QString("And begin! \n"));

	gameStateManager.instantiateInitialPieces();
	for (int i = 0; i < displayManager.getNumberOfRows(); i++)
	{
		for (int j = 0; j < displayManager.getNumberOfColumns(); j++)
		{
			connect(displayManager.getButtonAtPosition(i, j), &QPushButton::clicked, 
				[&, x = i, y = j]() {gameStateManager.moveCurrentPiece(ChessPiecesData::PiecePosition(x, y)); });
		}
	}

	

	for (auto&& spawnedPiece : displayManager.getSpawnedPieces())
	{
		connect(spawnedPiece.spawnedPieceVisual, &QPushButton::clicked,
			[&]() {gameStateManager.selectPiece(spawnedPiece.pieceData); });
	}

	//displayManager.togglePlacementIndication(true, 2, 0);

	//shared_ptr<AbsChessPiece> newPiece = std::make_shared<AbsChessPiece>(AbsChessPiece::PiecePosition(1, 5));
	//displayManager.summonPiece(newPiece);

	//
	////manager.selectPiece(displayManager.spawnedPieces[0].pieceData);

	//connect(displayManager.spawnedPieces[0].spawnedPieceVisual, &QPushButton::clicked,
	//	[&]() {gameStateManager.selectPiece(displayManager.spawnedPieces[0].pieceData); });
}
