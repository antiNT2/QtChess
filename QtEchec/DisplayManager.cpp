#include "DisplayManager.h"
#include <QPropertyAnimation>
using namespace std;

#pragma region QStyle Definition
const QString redCaseStyle = QString::fromUtf8("QPushButton {\n"
	"	  border-image: url(:/QtEchec/redCase.png) 0 0 0 0 stretch stretch;\n"
	" }\n"
	"QPushButton:hover:!pressed {\n"
	"	border-image: url(:/QtEchec/selectedCase.png) 0 0 0 0 stretch stretch;\n"
	" }");

const QString yellowCaseStyle = QString::fromUtf8("QPushButton {\n"
	"	  border-image: url(:/QtEchec/yellowCase.png) 0 0 0 0 stretch stretch;\n"
	" }\n"
	"QPushButton:hover:!pressed {\n"
	"	border-image: url(:/QtEchec/selectedCase.png) 0 0 0 0 stretch stretch;\n"
	" }");
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
			newChessButton->setStyleSheet(((i + j) % 2 == 0 ? redCaseStyle : yellowCaseStyle));
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
	output->setIconSize(QSize(64, 64));
	output->setFlat(true);

	return output;
}
