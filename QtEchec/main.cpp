/**
* Program that allows 2 players to play chess on the same computer
* \file   UnitTestingChess.h | UnitTestingChess.cpp
* \author BEN RHOUMA Ahmed and KEZOUH Ryan
* \date   April 6th 2022
*/

#include "QtEchec.h"
#include <QtWidgets/QApplication>

#if __has_include("bibliotheque_cours.hpp")
#include "bibliotheque_cours.hpp"
#define BIBLIOTHEQUE_COURS_INCLUS
using bibliotheque_cours::cdbg;
#else
auto& cdbg = clog;
#endif

#if __has_include("verification_allocation.hpp")
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp" 
#endif

void initialiserBibliothequeCours([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
#ifdef BIBLIOTHEQUE_COURS_INCLUS
	bibliotheque_cours::activerCouleursAnsi();


    bibliotheque_cours::executerGoogleTest(argc, argv);
#endif
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initialiserBibliothequeCours(argc, argv);

    QtEchec w;
    w.show();
    return a.exec();
}
