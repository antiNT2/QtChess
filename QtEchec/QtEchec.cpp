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
	: QMainWindow(parent), gameStateManager(GameStateManager())
{
	ui.setupUi(this);
	displayManager = DisplayManager(ui.chessFrame, &ui);
	displayManager.setUpChessUi();
	//gameStateManager = GameStateManager(&displayManager);

	displayManager.displayMessage(QString("A great slam and then some! \n"));
	displayManager.displayMessage(QString("And begin! \n"));


	connect(&gameStateManager, &GameStateManager::onInstantiatePiece,
		&displayManager, &DisplayManager::summonPiece);

	connect(&gameStateManager, &GameStateManager::onRemovedPiece,
		&displayManager, &DisplayManager::removePiece);

	connect(&gameStateManager, &GameStateManager::onNoPieceSelected,
		[&]() {displayManager.displayMessage("Select a piece first! \n"); });

	connect(&gameStateManager, &GameStateManager::onIllegalMoveChosen,
		[&]() {displayManager.displayMessage("Illegal move! \n"); });

	connect(&gameStateManager, &GameStateManager::onRemoveAllowedDestination,
		[&](int x, int y) {displayManager.togglePlacementIndication(false, x, y); });

	connect(&gameStateManager, &GameStateManager::onAddAllowedDestination,
		[&](int x, int y) {displayManager.togglePlacementIndication(true, x, y); });

	connect(&gameStateManager, &GameStateManager::onSelectPiece,
		[&](int x, int y)	{ displayManager.setBackgroundColor(x, y, false);});

	connect(&gameStateManager, &GameStateManager::onDeselectPiece,
		[&](int x, int y) { displayManager.setBackgroundColor(x, y, true); });

	/*displayManager->setBackgroundColor(currentSelectedPiece, currentSelectedPiece.get()->getPiecePosition().gridX,
		currentSelectedPiece.get()->getPiecePosition().gridY, true);*/

	for (int i = 0; i < displayManager.getNumberOfRows(); i++)
	{
		for (int j = 0; j < displayManager.getNumberOfColumns(); j++)
		{
			connect(displayManager.getButtonAtPosition(i, j), &QPushButton::clicked,
				[&, x = i, y = j]() {gameStateManager.moveCurrentPiece(ChessPiecesData::PiecePosition(x, y)); });
		}
	}

	gameStateManager.instantiateInitialPieces();

	for (auto&& spawnedPiece : displayManager.getSpawnedPieces())
	{
		connect(spawnedPiece.spawnedPieceVisual, &QPushButton::clicked,
			[&, p = spawnedPiece.pieceData]() {gameStateManager.selectPiece(p); });
	}

	connect(&gameStateManager, &GameStateManager::onPieceMoved,
		[&](const std::shared_ptr<AbsChessPiece> piece, int gridX, int gridY) 
		{ displayManager.movePieceToPosition(piece, gridX, gridY); });
	
}
