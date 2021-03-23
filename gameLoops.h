#ifndef GAMELOOPSHEADER
#define GAMELOOPSHEADER

#include "playerclassHeader.h"
#include "DeckManagerClass.h"
//#include "Defs.h"
/*
#ifndef DECKMANAGERHEADER

#ifndef PLAYERCLASSHEADER

#include "DeckManagerClass.h"
#include "playerclassHeader.h"

#endif // !PLAYERCLASSHEADER
#endif // !DECKMANAGERHEADER
*/
bool isValidInput(char);

bool playerTurn(Player& player, DeckManager& deck, int turn = 0);

#endif // !GAMELOOPSHEADER



