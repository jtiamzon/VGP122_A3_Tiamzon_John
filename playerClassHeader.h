
#ifndef PLAYERCLASSHEADER
#define PLAYERCLASSHEADER

#include <iostream>
#include <math.h>
#include <vector>
#include <time.h>

#include "DeckManagerClass.h"
#include <sdl_mixer.h>
/*
#ifndef DECKMANAGERHEADER

#include "DeckManagerClass.h"

#endif // !DECKMANAGERHEADER
*/

class Player {
    vector<Card*> hand;                 // These other hands are only used for splitting
    vector<Card*> hand2;
    int handVal = 0;
    int hand2Val = 0;
    int startingCredits = 10000;
    int credits = startingCredits;
    int numOfAces = 0;
    bool splitBool = false;
    bool doubleDownBool = false;
    bool surrenderBool = false;
    bool canSurrender = false;
    int betAmt;
    int bet2Amt;
public:
    int getHandVal() {
        return this->handVal;
    }
    int getHand2Val() {
        return this->hand2Val;
    }
    void setHand2Val(int newVal) {
        this->hand2Val = newVal;
    }
    int getStartingCredits() {
        return this->startingCredits;
    }

    int getCredits() {
        return this->credits;
    }
    void setCredits(int newVal) {
        this->credits = newVal;
    }

    int getBetAmount() {
        return this->betAmt;
    }
    void setBetAmount(int newVal) {
        this->betAmt = newVal;
    }
    int getBet2Amount() {
        return this->bet2Amt;
    }
    void setBet2Amount(int newVal) {
        this->bet2Amt = newVal;
    }

    vector<Card*> getPlayerHand() {
        return this->hand;
    }

    vector<Card*> getPlayerHand2() {
        return this->hand2;
    }

    void pushtoPlayerHand(Card* card) {
        this->hand.push_back(card);
    }

    void pushtoPlayerHand2(Card* card) {
        this->hand2.push_back(card);
    }
    int getNumOfAces() {
        return this->numOfAces;
    }
    void setNumOfAces(int newNum) {
        this->numOfAces = newNum;
    }
    bool getSplitBool() {
        return splitBool;
    }
    void setSplitBool(bool newBool) {
        this->splitBool = newBool;
    }

    bool getDoubleDownBool() {
        return doubleDownBool;
    }
    void setDoubleDownBool(bool newBool) {
        this->doubleDownBool = newBool;
    }

    bool setSurrenderBool() {
        return surrenderBool;
    }
    void setSurrenderBool(bool newBool) {
        this->surrenderBool = newBool;
    }
    bool getCanSurrenderBool() {
        return canSurrender;
    }
    void setCanSurrenderBool(bool newBool) {
        this->canSurrender = newBool;
    }

    void setBoolsToDefault() {
        numOfAces = 0;
        hand2Val = 0;
        bet2Amt = 0;
        splitBool = false;
        doubleDownBool = false;
        surrenderBool = false;
        canSurrender = false;
    }

    void totalCalc() {
        this->handVal = 0;
        for (Card* card : this->hand) {
            this->handVal += (*card).value;
        }
        if (this->handVal > 21) {
            this->handVal -= (10 * this->numOfAces);
        }
        return;
    }
    // returns true if the hand has a value of 21 or less
    bool totalCheck() {
        if (this->handVal > 21)
            return false;
        return true;
    }

    // pushes a card from the maindeck to the player's hand
    void hit(DeckManager& maindeck) {
        this->hand.push_back(maindeck.drawCardManager());
        if ((this->hand)[(this->hand).size() - 1]->type == "Ace")
            this->numOfAces += 1;
        return;
    }

    // almost no point in this function, I just wanted to take up processing time and resources
    void stand() {
        return;
    }

    // checks to see if the turn == 0, then if the cards in the players hand have the same value
    bool split(int turn) {
        if (turn == 0 && !(this->splitBool))
            if (this->hand[0]->value == this->hand[1]->value) {
                this->hand2.push_back(hand[1]);
                this->hand.pop_back();
                this->splitBool = true;
                return true;
            }
        return false;
    }

    // if the player has insufficient credits, returns false. otherwise executes and returns true
    bool doubleDown() {
        if (this->betAmt > this->credits)
            return false;
        this->credits -= betAmt;
        this->betAmt *= 2;
        this->doubleDownBool = true;
        return true;

    }

    // surrenders and returns true if it is the first turn, otherwise returns false
    bool surrender(int turn) {
        if (turn == 0 && !splitBool && canSurrender) {
            this->credits += this->betAmt / 2;
            return true;
        }
        return false;
    }

    // if the player's credits == 0, returns false. checks the players input to verify it, and returns true once executed
    bool bet() {
        if (this->credits == 0)
            return false;
        cout << "You have " << credits << " Credits." << endl;
        cout << "Enter a bet amount (0 to Quit): ";
        while (!(cin >> betAmt) || (betAmt > credits || betAmt < 1)) {
            if (betAmt == 0) {

                cout << "Thanks for Playing!" << endl << endl;

                cout << "Final Credit Score: " << this->getCredits() << endl;
                exit(0);
            }
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Please enter an amount greater than zero, and lower or equal to \nthe amount of credits you have " <<
                "(" << credits << ")" << endl << endl;
            cout << "Enter a bet amount: ";
        }
        credits -= betAmt;
        return true;
    }

    void giveInstructions()
    {
        cout << "\nBlackjack is a game also known as Twenty One (21). It is YOU, the player, against the AI, the dealer. The goal of the game is \n";
        cout << "to be higher than the dealer's cards to win. But never exceed over 21, or you lose.\n";
        cout << endl;
        cout << "Players are each dealt two cards, face up or down depending on the casino and the table. In the U.S., the dealer is also dealt two cards, \n";
        cout << "normally one up(exposed) and one down(hidden).In most other countries, the dealer only receives one card face up.The value of cards two through \n";
        cout << "ten is their pip value(2 through 10).Face cards(Jack, Queen, and King) are all worth ten.Aces can be worth one or eleven.A hand's value is the sum of the card values. \n";
        cout << "Players are allowed to draw additional cards to improve their hands. A hand with an ace valued as 11 is called SOFT, meaning that the hand will not bust by taking \n";
        cout << "an additional card.The value of the ace will become one to prevent the hand from exceeding 21. Otherwise, the hand is called HARD.\n";
        cout << endl;
        cout << "Once all the players have completed their hands, it is the dealer's turn. The dealer hand will not be completed if all players have either busted or received blackjacks. \n";
        cout << "The dealer then reveals the hidden cardand must hit until the cards total up to 17 points.At 17 points or higher the dealer must stay. (At most tables the dealer also hits \n";
        cout << "on a SOFT 17, (i.e. a hand containing an aceand one or more other cards totaling six.) You are betting that you have a better hand than the dealer.The better hand is the \n";
        cout << "hand where the sum of the card values is closer to 21 without exceeding 21. The detailed outcome of the hand follows : \n";
        cout << "       If the player is dealt an Ace and a ten-value card (called a BLACKJACK or NATURAL), and the dealer does not, the player wins and usually receives a bonus.\n";
        cout << "       If the player exceeds a sum of 21 (BUSTS), the player loses, even if the dealer also exceeds 21.\n";
        cout << "       If the dealer exceeds 21 (BUSTS) and the player does not, the player wins.\n";
        cout << "       If the player attains a final sum higher than the dealerand does not bust, the player wins.\n";
        cout << "       If both dealerand player receive a blackjack or any other hands with the same sum called a PUSH, no one wins.\n";
        cout << endl;
    }

    // Displays a list of all the commands (Except 'L')
    void commandList() {
        cout << endl;
        cout << "Valid Commands:" << endl;
        // Instructions
        cout << "Instructions: 'i'" << endl;
        // Hit
        cout << "Hit: 'H'" << endl;
        // Stand
        cout << "Stand: 'S'" << endl;
        // Split
        cout << "Split: 'P'" << endl;
        // Double Down
        cout << "Double Down: 'D'" << endl;
        // Surrender Hand
        cout << "Surrender Hand: 'F'" << endl;
        // Exit Game
        cout << "Exit Game: 'E'" << endl;
    }

    // calls the discardCardManager, then clears the hand, and returns nothing
    void discardHand(DeckManager& deck) {
        for (Card* card : this->hand) {
            deck.discardCardManager(card);
        }
        /*
        for (Card* card : hand2) {
            deck.discardCardManager(card);
        }
        hand2.clear();
        */
        this->hand.clear();
    }

    void discardHand2(DeckManager& deck) {
        for (Card* card : this->hand2) {
            deck.discardCardManager(card);
        }
        /*
        for (Card* card : hand2) {
            deck.discardCardManager(card);
        }
        hand2.clear();
        */
        this->hand2.clear();
    }

    // gets the player's starting hand. returns True if the hand total is 21, otherwise returns false
    bool getStartingHand(DeckManager& deck) {
        this->hand.push_back(deck.drawCardManager());
        this->hand.push_back(deck.drawCardManager());
        this->totalCalc();
        if (this->getHandVal() == 21)
            return true;
        return false;
    }
};


#endif
