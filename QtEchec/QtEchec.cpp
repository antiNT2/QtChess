#include "QtEchec.h"
#include "DisplayManager.h"
#include "qscrollbar.h"

QtEchec::QtEchec(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	displayManager = DisplayManager(ui.chessFrame);
	displayManager.setUpChessUi();
	ui.debugInfoDisplay->insertPlainText(QString::fromUtf8("Hello \n"));

	int countX = 0;
	int countY = 0;
	for (auto&& btn : displayManager.getAllChessButtons())
	{
		connect(btn, &QPushButton::clicked, [&, x = countX, y = countY]() {
			ui.debugInfoDisplay->insertPlainText(QString("Move to x= %1 y= %2 \n").arg(x).arg(y));
			displayManager.movePieceToPosition(ui.piece, x, y);
			ui.debugInfoDisplay->verticalScrollBar()->setValue(ui.debugInfoDisplay->verticalScrollBar()->maximum());
			});
		countY++;
		countY = countY % 8;
		if (countY == 0)
			countX++;
	}



	//ui.debugInfoDisplay->insertPlainText(QString("chess case 1: x= %1 y= %2").arg(displayManager.));
}
