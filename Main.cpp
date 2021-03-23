
#include <iostream>
#include <math.h>
#include <vector>
#include <time.h>

#include "deckManagerClass.h"
#include "playerClassHeader.h"
#include "gameLoops.h"
#include "Defs.h"

using namespace std;

DeckManager deckmanager;            // DeckManager class instance
Player userPlayer;                  // Player1 class instance
Player dealer;                      // Dealer class instance
vector<Player*> playerList;



int main(int argv, char** argc)
{  
    if (SDL_Init(SDL_INIT_AUDIO) != 0)
    {
        /** Display error message */
        cerr << "\nUnable to initialize SDL: " << SDL_GetError() << endl;

        /** End main() */
        return 1;
    }
    atexit(SDL_Quit);
    
    /* Open Audio */
    int Mix_OpenAudio(int freq, Uint16 format, int channels, int chunk);

    music = Mix_LoadMUS("BlackjackSounds/lobby_music.wav");
    /* If audio file was opened */
    if (!music)
        cerr << "lobby_music Mix_LoadMUS Error: " << Mix_GetError() << endl;
    Mix_PlayMusic(music, -1);

    cardShuffleSnd = Mix_LoadWAV("BlackjackSounds/cardShuffle.wav");
        if (!cardShuffleSnd)
            cerr << "cardShuffleSnd Mix_LoadWAV Error: " << Mix_GetError() << endl;
        Mix_PlayChannel(-1, cardShuffleSnd, 0);
    
    if (count(playerList.begin(), playerList.end(), &userPlayer) == 0)             // If player is not on player list, it will add player to game; else it will continue
    {
        playerList.push_back(&userPlayer);
        userPlayer.commandList();
        cout << endl << endl;
    }


    if (!userPlayer.bet())                                                 // If player does bet money
    {
        char cont;
        gameFinishSnd = Mix_LoadWAV("BlackjackSounds/loser.wav");
        if (!gameFinishSnd)
            cerr << "gameFinishSnd Mix_LoadWAV Error: " << Mix_GetError() << endl;
        Mix_PlayChannel(4, gameFinishSnd, 0);
        cout << "You're out of credits!" << endl;
        cout << "Would you like to play again? (Y/N)" << endl;
        while (!(cin >> cont)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please enter Y or N: ";
            cout << endl;
        }
        if (cont == 'N') {
            cout << "Game Over! Thanks for Playing!" << endl;
            exit(0);
        }
        else
        {
            userPlayer.setCredits(userPlayer.getStartingCredits());     // Player takes the set credits
        }

    }

    if (deckmanager.getMainDeck().size() <= 31)
    {
        deckmanager.initializeDeck();
    }

    dealer.getStartingHand(deckmanager);
    bool dealerBlackjack = (dealer.getHandVal() == 21);
    vector<Card*> dealerHand = (dealer.getPlayerHand());
    bool surrender = (((dealerHand)[0])->type == "Ace");
    userPlayer.setCanSurrenderBool(surrender);

    cout << endl << "Dealer's Up Card: " << ((dealerHand)[0])->type << " of " << ((dealerHand)[0])->suit << endl;

    for (int i = 0; i < playerList.size(); i++)                                     // start of hand for player
    {
        playerList[i]->getStartingHand(deckmanager);
        for (Card* card : playerList[i]->getPlayerHand())
        {
            if (card->type == "Ace")
                playerList[i]->setNumOfAces(playerList[i]->getNumOfAces() + 1);
        }
    }


    for (int i = 0; i < playerList.size(); i++)
    {
        Player* playerRef = playerList[i];
        int playerHandVal = playerRef->getHandVal();
        bool playerBlackJack = (playerHandVal == 21);
        if (playerRef == &userPlayer && (playerBlackJack || dealerBlackjack))
        {

            vector<Card*> playerHand = userPlayer.getPlayerHand();

            cout << endl;

            cout << "Your Hand: " << endl;

            for (Card* card : playerHand)
            {
                cout << card->type << " of " << card->suit << endl;
            }

            cout << "Your Total Hand Value: " << userPlayer.getHandVal() << endl;

        }
        if (playerBlackJack && !dealerBlackjack)
        {
            blackjackSnd = Mix_LoadWAV("BlackjackSounds/gotcha-bitch.wav");
            if (!blackjackSnd)
                cerr << "blackjackSnd Mix_LoadWAV Error: " << Mix_GetError() << endl;
            Mix_PlayChannel(0, cardShuffleSnd, 0);
            cout << "You got a Blackjack! The dealer does not have a blackjack, so you win!" << endl;
            playerRef->setCredits(playerRef->getBetAmount() * (1.5) + playerRef->getCredits());
            playerRef->setBetAmount(0);
        }
        else if (playerBlackJack && dealerBlackjack)
        {
            tieSnd = Mix_LoadWAV("BlackjackSounds/gasp_crowd.wav");
            if (!tieSnd)
                cerr << "tieSnd Mix_LoadWAV Error: " << Mix_GetError() << endl;
            Mix_PlayChannel(1, tieSnd, 0);
            
            cout << "You and the Dealer both have a Blackjack. You tie with the dealer." << endl;
            playerRef->setCredits(playerRef->getBetAmount() + playerRef->getCredits());
            playerRef->setBetAmount(0);
        }
        else if (!playerBlackJack && dealerBlackjack)
        {
            noBlackjackSnd = Mix_LoadWAV("BlackjackSounds/bust.wav");
            if (!noBlackjackSnd)
                cerr << "noBlackjackSnd Mix_LoadWAV Error: " << Mix_GetError() << endl;
            Mix_PlayChannel(2, noBlackjackSnd, 0);
            
            cout << "The Dealer got a Blackjack... You do not have a Blackjack, so you lose your bet." << endl;
            playerRef->setBetAmount(0);
            playerRef->discardHand(deckmanager);
        }
        if (playerBlackJack)
        {
            playerRef->discardHand(deckmanager);
        }
    }

    if (!dealerBlackjack && (userPlayer.getHandVal() != 21))
    {
        if (playerTurn(userPlayer, deckmanager))
        {
            cout << "Dealer's Hand: " << endl;

            for (Card* card : (dealer.getPlayerHand()))
            {
                cout << card->type << " of " << card->suit << endl;
            }

            while (dealer.getHandVal() <= 16)
            {
                dealer.hit(deckmanager);

                dealer.totalCalc();
                cout << "Dealer Drew: " << ((dealer.getPlayerHand()))[(dealer.getPlayerHand()).size() - 1]->type
                    << " of " << ((dealer.getPlayerHand())[(dealer.getPlayerHand()).size() - 1])->suit << endl;
            }
            cout << "Dealer's Total Hand Value: " << dealer.getHandVal() << endl;

            if (dealer.getHandVal() > userPlayer.getHandVal() && dealer.getHandVal() <= 21)
            {
                playerLoseSnd = Mix_LoadWAV("BlackjackSounds/gasp_crowd.wav");
                if (!playerLoseSnd)
                    cerr << "playerLoseSnd Mix_LoadWAV Error: " << Mix_GetError() << endl;

                Mix_PlayChannel(3, playerLoseSnd, 0);
                cout << "The Dealer Won..." << endl << "You lost " << userPlayer.getBetAmount() << " Credits." << endl;
                userPlayer.setBetAmount(0);
            }

            else if (dealer.getHandVal() == userPlayer.getHandVal())
            {
               tieRegSnd = Mix_LoadWAV("BlackjackSounds/damnit.wav");
                if (!tieRegSnd)
                    cerr << "tieRegSnd Mix_LoadWAV Error: " << Mix_GetError() << endl;

                Mix_PlayChannel(5, tieRegSnd, 0);
                
                cout << "You tied with the Dealer. Your bet of " << userPlayer.getBetAmount() << " Credits was returned to you." << endl;
                userPlayer.setCredits(userPlayer.getCredits() + userPlayer.getBetAmount());
                userPlayer.setBetAmount(0);
            }

            else if (dealer.getHandVal() < userPlayer.getHandVal()) {
                cout << "You Defeated the Dealer. You received " << userPlayer.getBetAmount() * 1.5 << " Credits." << endl;
                userPlayer.setCredits(userPlayer.getCredits() + userPlayer.getBetAmount() * 1.5);
                userPlayer.setBetAmount(0);
            }
            cout << endl;
        }
        else if (!userPlayer.setSurrenderBool()) {

            cout << ((userPlayer.getPlayerHand())[(userPlayer.getPlayerHand()).size() - 1])->type
                << " of " << ((userPlayer.getPlayerHand())[(userPlayer.getPlayerHand()).size() - 1])->suit << endl;
            playerBustSnd = Mix_LoadWAV("BlackjackSounds/damnit_loud.wav");
            if (!playerBustSnd)
                cerr << "playerBustSnd Mix_LoadWAV Error: " << Mix_GetError() << endl;

            Mix_PlayChannel(6, playerBustSnd, 0);

            cout << "You went over 21! You lost your bet of " << userPlayer.getBetAmount() << " Credits." << endl;
            cout << "Dealer's Hand: " << endl;

            for (Card* card : (dealer.getPlayerHand())) {
                cout << card->type << " of " << card->suit << endl;
            }

            cout << "Dealer's Total Hand Value: " << dealer.getHandVal() << endl;
            cout << endl;
        }
    }
    if (!dealerBlackjack && (userPlayer.getHand2Val() > 0)) {
        if (dealer.getHandVal() > userPlayer.getHand2Val()) {
            playerLoseSnd = Mix_LoadWAV("BlackjackSounds/gasp_crowd.wav");
            if (!playerLoseSnd)
                cerr << "playerLoseSnd Mix_LoadWAV Error: " << Mix_GetError() << endl;

            Mix_PlayChannel(3, playerLoseSnd, 0);
            cout << "The Dealer Won..." << endl << "You lost " << userPlayer.getBet2Amount() << " Credits." << endl;
            userPlayer.setBetAmount(0);
        }

        else if (dealer.getHandVal() == userPlayer.getHand2Val()) {
            tieRegSnd = Mix_LoadWAV("BlackjackSounds/damnit.wav");
            if (!tieRegSnd)
                cerr << "tieRegSnd Mix_LoadWAV Error: " << Mix_GetError() << endl;

            Mix_PlayChannel(5, tieRegSnd, 0);
            cout << "You tied with the Dealer. Your bet of " << userPlayer.getBet2Amount() << " Credits was returned to you." << endl;
            userPlayer.setCredits(userPlayer.getCredits() + userPlayer.getBet2Amount());
            userPlayer.setBetAmount(0);
        }

        else if (dealer.getHandVal() < userPlayer.getHand2Val()) {
            playerWinSnd = Mix_LoadWAV("BlackjackSounds/winner.wav");
            if (!playerWinSnd)
                cerr << "playerWinSnd Mix_LoadWAV Error: " << Mix_GetError() << endl;

            Mix_PlayChannel(6, playerWinSnd, 0);

            cout << "You Defeated the Dealer. You received " << userPlayer.getBet2Amount() * 1.5 << " Credits." << endl;
            userPlayer.setCredits(userPlayer.getCredits() + userPlayer.getBet2Amount() * 1.5);
            userPlayer.setBetAmount(0);
        }
        cout << endl;
    }

    /*if (dealerBlackjack) {

        // cout << "The Dealer got BlackJack!" << endl;

        dealer.discardHand(deckmanager);
    }*/

    for (Player* player : playerList) {
        player->discardHand(deckmanager);
        player->discardHand2(deckmanager);
        player->setBoolsToDefault();
    }

    dealer.discardHand(deckmanager);

    cout << endl;

    main(argv, argc);


}