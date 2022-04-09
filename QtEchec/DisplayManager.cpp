#include "DisplayManager.h"
#include <QPropertyAnimation>
#include <QLabel>
#include <QScrollbar>
#include <stdexcept>
using namespace std;

#pragma region QStyle Definition
const QString redCaseFilePath = ":/QtEchec/images/redCase.png";
const QString redPlacementIndicatorFilePath = ":/QtEchec/images/redPlacementIndicator.png";
const QString yellowCaseFilePath = ":/QtEchec/images/yellowCase.png";
const QString yellowPlacementIndicatorFilePath = ":/QtEchec/images/yellowPlacementIndicator.png";
const QString selectedCaseFilePath = ":/QtEchec/images/selectedCase.png";
const QString deathCaseFilePath = ":/QtEchec/images/deathIndicator.png";

const std::string selectedPieceBackgroundColor = "background-color: rgb(85, 170, 127);";
const std::string unselectedPieceBackgroundColor = "background-color: rgba(0, 0, 0, 0);";
#pragma endregion

#pragma region QStyle Chess Piece Display Definition
const QString basePieceFilePath = ":/QtEchec/images/";
#pragma endregion

const int nbOfRows = 8;
const int nbOfColumns = 8;
const int chessCaseSize = 64;

DisplayManager::DisplayManager()
{
}

DisplayManager::DisplayManager(QFrame* chessFrameParent, Ui::QtEchecClass* _ui) : chessFrame(chessFrameParent), ui(_ui)
{
}

DisplayManager::~DisplayManager()
{
	deleteAllPieces();
}

void DisplayManager::setUpChessUi()
{
	for (size_t i = 0; i < nbOfRows; i++)
	{
		for (size_t j = 0; j < nbOfColumns; j++)
		{
			QPushButton* newChessButton = createChessCase(chessFrame);
			newChessButton->setObjectName(QString("c%1%2").arg(i, j));
			newChessButton->setGeometry(QRect(i * chessCaseSize, j * chessCaseSize, chessCaseSize, chessCaseSize));
			newChessButton->setStyleSheet(getButtonStyleSheet((i + j) % 2 == 0 ? true : false, CaseType::Normal));
			allChessCasesButtons.push_back(newChessButton);
		}
	}

	setPlayerTurnIndicator(true);
}

void DisplayManager::operator=(const DisplayManager& other)
{
	chessFrame = other.chessFrame;
	ui = other.ui;
}

void DisplayManager::movePieceToPosition(QWidget* piece, int gridX, int gridY)
{
	QPropertyAnimation* animation = new QPropertyAnimation(piece, "geometry");
	animation->setDuration(350);

	QPoint positionReference = chessFrame->geometry().topLeft();
	QRect destinationRect = QRect(positionReference, QSize(chessCaseSize, chessCaseSize))
		.translated(gridX * chessCaseSize, gridY * chessCaseSize);

	animation->setEndValue(destinationRect);
	animation->setEasingCurve(QEasingCurve::OutExpo);

	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void DisplayManager::setBackgroundColor(QWidget* piece, bool transparent)
{
	std::string newStyleSheet = piece->styleSheet().toStdString();

	auto replace = [&](std::string& str, const std::string& from, const std::string& to)
	{
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	};

	if (newStyleSheet.find(selectedPieceBackgroundColor) == std::string::npos &&
		newStyleSheet.find(unselectedPieceBackgroundColor) == std::string::npos)
	{
		newStyleSheet.append(transparent ? unselectedPieceBackgroundColor : selectedPieceBackgroundColor);
	}

	replace(newStyleSheet, transparent ? selectedPieceBackgroundColor : unselectedPieceBackgroundColor,
		transparent ? unselectedPieceBackgroundColor : selectedPieceBackgroundColor);

	piece->setStyleSheet(QString::fromStdString(newStyleSheet));
}

const DisplayManager::SpawnedPiece DisplayManager::getSpawnedPiece(const const std::shared_ptr<AbsChessPiece> piece)
{
	for (auto&& sp : spawnedPieces)
	{
		if (sp.pieceData == piece)
			return sp;
	}

	throw std::logic_error("Piece not found");
}

const DisplayManager::SpawnedPiece& DisplayManager::getSpawnedPiece(int gridX, int gridY)
{
	for (auto&& sp : spawnedPieces)
	{
		if (sp.pieceData.get()->getPiecePosition() == ChessPiecesData::PiecePosition(gridX, gridY))
			return sp;
	}

	throw std::logic_error("Piece not found");
}

const bool DisplayManager::pieceExistInPosition(int gridX, int gridY)
{
	for (auto&& sp : spawnedPieces)
	{
		if (sp.pieceData.get()->getPiecePosition() == ChessPiecesData::PiecePosition(gridX, gridY))
			return true;
	}

	return false;
}

void DisplayManager::togglePlacementIndication(bool show, int gridX, int gridY)
{
	CaseType caseToShow = CaseType::Normal;
	if (show)
	{
		caseToShow = (pieceExistInPosition(gridX, gridY)) ? CaseType::DeathCase : CaseType::PlacementIndicator;
	}
	getButtonAtPosition(gridX, gridY)->setStyleSheet(getButtonStyleSheet(((gridX + gridY) % 2 == 0 ? true : false), caseToShow));
}

QPushButton* DisplayManager::getButtonAtPosition(int gridX, int gridY)
{
	int countX = 0;
	int countY = 0;
	for (auto&& btn : allChessCasesButtons)
	{
		if (countX == gridX && countY == gridY)
			return btn;

		countY++;
		countY = countY % 8;
		if (countY == 0)
			countX++;
	}
}

const vector<DisplayManager::SpawnedPiece>& DisplayManager::getSpawnedPieces()
{
	return spawnedPieces;
}

void DisplayManager::summonPiece(std::shared_ptr<AbsChessPiece> pieceData)
{
	SpawnedPiece newSpawnedPiece = SpawnedPiece(pieceData, createPieceVisual(pieceData));
	spawnedPieces.push_back(newSpawnedPiece);

	qDebug() << "Summoned " << pieceData.get()->getPieceName().c_str() << "\n";
}

void DisplayManager::removePiece(std::shared_ptr<AbsChessPiece> pieceToRemove)
{
	SpawnedPiece spawnedPieceToRemove = getSpawnedPiece(pieceToRemove);

	int foundIndex = -1;
	for (int i = 0; i < spawnedPieces.size(); i++)
	{
		if (spawnedPieces[i].pieceData == spawnedPieceToRemove.pieceData)
		{
			foundIndex = i;
			break;
		}
	}

	spawnedPieceToRemove.spawnedPieceVisual->disconnect();
	delete spawnedPieceToRemove.spawnedPieceVisual;
	spawnedPieces.erase(spawnedPieces.begin() + foundIndex);
}

void DisplayManager::movePieceToPosition(const std::shared_ptr<AbsChessPiece> piece, int gridX, int gridY)
{
	movePieceToPosition(getSpawnedPiece(piece).spawnedPieceVisual, gridX, gridY);
}

void DisplayManager::setPlayerTurnIndicator(bool isPlayer1Turn)
{
	ui->playerTurnLabel->setText(QString(isPlayer1Turn ? "Player 1's turn" : "Player 2's turn"));
}

void DisplayManager::deleteAllPieces()
{
	for (auto&& spawnedPiece : spawnedPieces)
	{
		delete spawnedPiece.spawnedPieceVisual;
	}
	spawnedPieces.clear();

	qDebug() << "Cleared all pieces visual \n";
}

void DisplayManager::setBackgroundColor(int gridX, int gridY, bool transparent)
{
	setBackgroundColor(getSpawnedPiece(gridX, gridY).spawnedPieceVisual, transparent);
}

void DisplayManager::displayMessage(QString messageToShow)
{
	ui->debugInfoDisplay->insertPlainText(messageToShow);
	ui->debugInfoDisplay->verticalScrollBar()->setValue(ui->debugInfoDisplay->verticalScrollBar()->maximum());
}

QString DisplayManager::getButtonStyleSheet(bool isRedCase, CaseType caseType)
{
	QString imagePath = redCaseFilePath;

	if (caseType == CaseType::Normal)
	{
		imagePath = (isRedCase ? redCaseFilePath : yellowCaseFilePath);
	}
	else if (caseType == CaseType::PlacementIndicator)
	{
		imagePath = (isRedCase ? redPlacementIndicatorFilePath : yellowPlacementIndicatorFilePath);
	}
	else
	{
		imagePath = deathCaseFilePath;
	}

	QString output = QString("QPushButton {\n"
		"	  border-image: url(" + imagePath + ") 0 0 0 0 stretch stretch;\n"
		" }\n");

	return output;
}

const int DisplayManager::getNumberOfRows()
{
	return nbOfRows;
}

const int DisplayManager::getNumberOfColumns()
{
	return nbOfColumns;
}

QPushButton* DisplayManager::createChessCase(QWidget* parent)
{
	QPushButton* output = new QPushButton(parent);

	output->setBaseSize(QSize(0, 0));
	output->setMouseTracking(true);
	output->setAutoFillBackground(false);
	output->setIconSize(QSize(chessCaseSize, chessCaseSize));
	output->setFlat(true);

	return output;
}

// Instantiate a QWidget with the appropriate Chess Piece visual on the board
QPushButton* DisplayManager::createPieceVisual(std::shared_ptr<AbsChessPiece> pieceData)
{
	QPushButton* piece = new QPushButton(chessFrame->parentWidget());

	int posX = pieceData.get()->getPiecePosition().gridX * chessCaseSize + chessFrame->geometry().x();
	int posY = pieceData.get()->getPiecePosition().gridY * chessCaseSize + chessFrame->geometry().y();

	piece->setObjectName(QString::fromUtf8("piece"));
	piece->setGeometry(QRect(posX, posY, chessCaseSize, chessCaseSize));
	piece->setCursor(QCursor(Qt::PointingHandCursor));
	piece->setFocusPolicy(Qt::NoFocus);
	piece->setIconSize(QSize(chessCaseSize, chessCaseSize));
	piece->setFlat(true);

	QString pieceImage = basePieceFilePath + QString::fromStdString(pieceData.get()->getPieceName());

	pieceImage = pieceData.get()->isPlayer1Piece() ? pieceImage : pieceImage + QString("2");
	pieceImage += QString(".png");

	piece->setStyleSheet(QString("border-image: url(" + pieceImage + ") 0 0 0 0 stretch stretch;\n"));

	piece->show();

	qDebug() << "Summoned " << pieceData->getPieceName().c_str() << " at " << posX << " " << posY;

	return piece;
}
