#include "QtEchec.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtEchec w;
    w.show();
    return a.exec();
}
