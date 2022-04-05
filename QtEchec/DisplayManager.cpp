#include "DisplayManager.h"
#include <QPropertyAnimation>
#include <QLabel>
using namespace std;

#pragma region QStyle Definition
const QString redCaseFilePath = ":/QtEchec/images/redCase.png";
const QString redPlacementIndicatorFilePath = ":/QtEchec/images/redPlacementIndicator.png";
const QString yellowCaseFilePath = ":/QtEchec/images/yellowCase.png";
const QString yellowPlacementIndicatorFilePath = ":/QtEchec/images/yellowPlacementIndicator.png";
const QString selectedCaseFilePath = ":/QtEchec/images/selectedCase.png";
#pragma endregion

const int nbOfRows = 8;
const int nbOfColumns = 8;
const int chessCaseSize = 64;

DisplayManager::DisplayManager()
{
}

DisplayManager::DisplayManager(QFrame* chessFrameParent) : chessFrame(chessFrameParent)
{
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
			//newChessButton->setStyleSheet(((i + j) % 2 == 0 ? redCaseStyle : yellowCaseStyle));
			newChessButton->setStyleSheet(getButtonStyleSheet((i + j) % 2 == 0 ? true : false, false));
			allChessButtons.push_back(newChessButton);
		}
	}
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

void DisplayManager::togglePlacementIndication(bool show, int gridX, int gridY)
{
	getButtonAtPosition(gridX, gridY)->setStyleSheet(getButtonStyleSheet(((gridX + gridY) % 2 == 0 ? true : false), show));
}

QPushButton* DisplayManager::getButtonAtPosition(int gridX, int gridY)
{
	int countX = 0;
	int countY = 0;
	for (auto&& btn : getAllChessButtons())
	{
		if (countX == gridX && countY == gridY)
			return btn;

		countY++;
		countY = countY % 8;
		if (countY == 0)
			countX++;
	}
}

QString DisplayManager::getButtonStyleSheet(bool isRedCase, bool isPlacementIndicator)
{
	QString imagePath = redCaseFilePath;

	if (!isPlacementIndicator)
	{
		imagePath = (isRedCase ? redCaseFilePath : yellowCaseFilePath);
	}
	else 
	{
		imagePath = (isRedCase ? redPlacementIndicatorFilePath : yellowPlacementIndicatorFilePath);
	}

	QString output = QString("QPushButton {\n"
		"	  border-image: url(" + imagePath + ") 0 0 0 0 stretch stretch;\n"
		" }\n"
		"QPushButton:hover:!pressed {\n"
		"	border-image: url(" + selectedCaseFilePath + ") 0 0 0 0 stretch stretch;\n"
		" }");

	return output;
}

vector<QPushButton*>& DisplayManager::getAllChessButtons()
{
	return allChessButtons;
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
