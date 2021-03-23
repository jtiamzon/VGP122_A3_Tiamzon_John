#include "playerclassHeader.h"
#include <sdl_mixer.h>

// #include "Defs.h"
using namespace std;

bool isValidInput(char input) {
    char validInputs[] = { 'H', 'S' , 'P', 'D', 'F', 'L', 'E', 'i' };
    for (char letter : validInputs) {
        if (input == letter) {
            return true;
        }
    }
    return false;
}


// Returns true if the player's hand total value exceeds 21
bool playerTurn(Player& player, DeckManager& deck, int turn = 0) {
    player.totalCalc();

    vector<Card*> playerHand = player.getPlayerHand();

    cout << endl;

    cout << "Your Hand: " << endl;

    for (Card* card : playerHand) {
        cout << card->type << " of " << card->suit << endl;
    }

    cout << "Your Total Hand Value: " << player.getHandVal() << endl;

    cout << "Enter a command: ";
    char input = ' ';
    while (!(cin >> input) || !isValidInput(input)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter a valid command (you can view a list with the command 'L')" << endl;
        cout << "Enter a command: ";
    }
    switch (input) {
    case 'H':
        player.hit(deck);
        break;
    case 'S':
        player.stand();
        return true;
        break;
    case 'P':
        if (player.split(turn)) {
            Player tempPlayer;
            tempPlayer.setBetAmount(player.getBetAmount());
            tempPlayer.pushtoPlayerHand(player.getPlayerHand2()[0]);
            tempPlayer.hit(deck);
            player.hit(deck);
            tempPlayer.setSplitBool(true);
            if (playerTurn(tempPlayer, deck)) {
                vector<Card*> tempPlayerHand = tempPlayer.getPlayerHand();
                for (int i = 1; i < tempPlayerHand.size(); i++) {
                    player.pushtoPlayerHand2(tempPlayerHand[i]);
                }
                player.setHand2Val(tempPlayer.getHandVal());
                player.setBet2Amount(tempPlayer.getBetAmount());
                player.setCredits(player.getCredits() - tempPlayer.getBetAmount());
            }
            else {
                cout << (tempPlayer.getPlayerHand())[(tempPlayer.getPlayerHand()).size() - 1]->type <<
                    " of " << (tempPlayer.getPlayerHand())[(tempPlayer.getPlayerHand()).size() - 1]->suit << endl;

                cout << "This hand went over 21..." << endl;

                cout << "You lost " << tempPlayer.getBetAmount() << " Credits." << endl;

                player.setCredits(player.getCredits() - tempPlayer.getBetAmount());
            }
        }
        else {
            cout << "You can't split this hand!" << endl;
        }
        turn--;
        break;
    case 'D':
        if (!player.doubleDown()) {
            cout << "You don't have enough credits to double down!" << endl;
            turn--;
        }
        else
            player.hit(deck);
        break;
    case 'F':
        if (player.surrender(turn)) {
            cout << "You Surrendered your hand!" << endl;
            player.setSurrenderBool(true);
            return false;
        }
        else
            cout << "You cannot surrender!" << endl;
        turn--;
        break;
    case 'L':
        player.commandList();
        turn--;
        break;
    case 'E':
        cout << "See You Next Time!" << endl << endl;

        cout << "Final Credit Score: " << player.getCredits() << endl;

        atexit(Mix_CloseAudio);

        exit(0);
        break;
    default:
        // playerTurn(player, deck, turn);
        turn--;
        break;
    case 'i':               // instructions
        player.giveInstructions();
        turn--;
        break;
    }
    player.totalCalc();
    // cout << endl;

    Mix_Chunk* gotBlackjackSnd = nullptr;
    
    if (player.getHandVal() == 21) {
        cout << (player.getPlayerHand())[(player.getPlayerHand()).size() - 1]->type <<
            " of " << (player.getPlayerHand())[(player.getPlayerHand()).size() - 1]->suit << endl;
        cout << "You got a Blackjack!" << endl << endl;
        gotBlackjackSnd = Mix_LoadWAV("BlackjackSounds/shit_loud.wav");
        if (!gotBlackjackSnd)
            cerr << "gotBlackjackSnd Mix_LoadWAV Error: " << Mix_GetError() << endl;

        Mix_PlayChannel(7, gotBlackjackSnd, 0);
        return true;
    }
    if (player.totalCheck()) {
        if (!(player.getDoubleDownBool()))
            return playerTurn(player, deck, turn++);
        else
            return true;
    }
    else if (!player.totalCheck())
        return false;

}