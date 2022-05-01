#include "QtEchec.h"
#include "DisplayManager.h"
#include "qscrollbar.h"
#include "GameStateManager.h"
#include <QMessageBox>
#include <QInputDialog>

QtEchec::QtEchec(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	initializeGame(false);
	connect(ui.resetButton, &QPushButton::clicked,
		this, [&]() {resetGame(); });
}

void QtEchec::resetGame()
{
	for (auto conn : allConnections)
	{
		QObject::disconnect(conn);
	}

	allConnections.clear();

	gameStateManager.resetBoard();

	initializeGame(true);
}

void QtEchec::showWelcomeWindow()
{
	QStringList items;
	items << tr("Default") << tr("More rooks") << tr("No knights");

	bool ok;
	QString item = QInputDialog::getItem(this, tr("Start a new game"),
		tr("Welcome to QtChess! Select a starting position."), items, 0, false, &ok);

	int indexSelected = items.indexOf(item);

	if (ok && !item.isEmpty())
		selectedDefaultConfiguration = GameStateManager::InitialBoardPiecesPosition(indexSelected);
}

void QtEchec::initializeGame(bool reset = false)
{
	using namespace std;
	using ChessDisplay::DisplayManager;

	if (reset == false)
	{
		gameStateManager = GameStateManager();

		displayManager = DisplayManager(ui.chessFrame, &ui);

		displayManager.setUpChessUi();
	}

	showWelcomeWindow();

	ui.debugInfoDisplay->clear();
	// Greet the players with a cuphead reference
	displayManager.displayMessage("This match will get red hot! \n");


	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onInstantiatePiece,
		&displayManager, &DisplayManager::summonPiece));

	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onRemovedPiece,
		&displayManager, &DisplayManager::removePiece));

	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onNoPieceSelected,
		[&]() {displayManager.displayMessage("Select a piece first! \n"); }));

	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onIllegalMoveChosen,
		[&]() {displayManager.displayMessage("Illegal move! \n"); }));

	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onRemoveAllowedDestination,
		[&](int x, int y) {displayManager.togglePlacementIndication(false, x, y); }));

	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onAddAllowedDestination,
		[&](int x, int y) {displayManager.togglePlacementIndication(true, x, y); }));

	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onSelectPiece,
		[&](int x, int y) { displayManager.setBackgroundColor(x, y, false); }));

	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onDeselectPiece,
		[&](int x, int y) { displayManager.setBackgroundColor(x, y, true); }));


	for (int i = 0; i < displayManager.getNumberOfRows(); i++)
	{
		for (int j = 0; j < displayManager.getNumberOfColumns(); j++)
		{
			allConnections.push_back(connect(displayManager.getButtonAtPosition(i, j), &QPushButton::clicked,
				[&, x = i, y = j]() {gameStateManager.moveCurrentPiece(ChessPiecesData::PiecePosition(x, y)); }));
		}
	}

	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onChangePlayerTurn, &displayManager,
		&DisplayManager::setPlayerTurnIndicator));

	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onVerifyKingInCheck,
		[&](bool isKing1InCheck) {displayManager.
		displayMessage(isKing1InCheck ? "PLAYER 1's KING IN CHECK \n" : "Player 2's KING IN CHECK \n"); }));

	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onVerifyCheckmate,
		[&](bool isPlayer1Checkmate) {displayManager.
		displayDialogueBox(isPlayer1Checkmate ? "PLAYER 2 WINS \n" : "PLAYER 1 WINS \n"); resetGame(); }));

	connect(&gameStateManager, &GameStateManager::onResetBoard, &displayManager, &DisplayManager::deleteAllPieces);
	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onTooManyKings, [&]() {displayManager.displayMessage("TOO MANY KINGS! \n"); }));

	gameStateManager.instantiateInitialPieces(selectedDefaultConfiguration);

	for (auto&& spawnedPiece : displayManager.getSpawnedPieces())
	{
		allConnections.push_back(connect(spawnedPiece.spawnedPieceVisual, &QPushButton::clicked,
			[&, p = spawnedPiece.pieceData]() {gameStateManager.selectPiece(p); }));
	}

	allConnections.push_back(connect(&gameStateManager, &GameStateManager::onPieceMoved,
		[&](const std::shared_ptr<AbsChessPiece> piece, int gridX, int gridY)
		{ displayManager.movePieceToPosition(piece, gridX, gridY); }));
}

