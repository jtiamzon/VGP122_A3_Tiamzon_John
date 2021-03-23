#ifndef DECKMANAGERHEADER
#define DECKMANAGERHEADER

#include <iostream>
#include <math.h>
#include <vector>
#include <time.h>
#include <sdl_mixer.h>

using namespace std;

// A function to convert a string to an integral type so that it can be used in a switch statement (because why not)
constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

// struct for cards
struct Card
{
public:
    string suit, type;
    int value;
};

// A class to manage deck creation and cards. There should only be one deck manager, but nothing should break if there are more.
class DeckManager {
    const int DECKMAX = 49;
    const int PLAYERMAX = 8;
    // number of decks
    int numberOfDecks = 1;

    int numberOfPlayers = 1;

    // The Suits of the cards
    const char* suits[4] = { "Clubs", "Spades", "Diamonds", "Hearts" };

    // The types of the cards eg king, ace, 10
    const char* types[13] = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };

    // a vector to hold the unique card structs of possible cards
    vector<Card> deck;

    // a vector to hold the pointers of the deck of cards
    vector<Card*> maindeck;

    // a vector to hold the pointers of discarded cards
    vector<Card*> discardDeck;

    // A function to initialize every card and put it into a deck
    void initializeCards() {
        // clears the vector deck when the funtion is called so that the game can be run recursively
        deck.clear();
        for (const char* suit : suits)
        {
            for (const char* type : types)
            {
                Card card;
                card.suit = suit;
                card.type = type;

                // a switch statement to assign a value to 
                switch (str2int(type)) {
                case str2int("Ace"):
                    card.value = 11;
                    break;
                case str2int("2"):
                    card.value = 2;
                    break;
                case str2int("3"):
                    card.value = 3;
                    break;
                case str2int("4"):
                    card.value = 4;
                    break;
                case str2int("5"):
                    card.value = 5;
                    break;
                case str2int("6"):
                    card.value = 6;
                    break;
                case str2int("7"):
                    card.value = 7;
                    break;
                case str2int("8"):
                    card.value = 8;
                    break;
                case str2int("9"):
                    card.value = 9;
                    break;
                default:
                    card.value = 10;
                    break;
                }

                this->deck.push_back(card);
            }
        }
    }

public:
    DeckManager(int numDecks = 1, int numPlayers = 1) {
        if (numDecks > 0 && numDecks <= DECKMAX)
            numberOfDecks = numDecks;
        if (numPlayers > 0 && numPlayers <= PLAYERMAX)
            numberOfPlayers = numPlayers;
        this->initializeCards();
        this->initializeDeck();
        this->shuffleDeck();
    }
    void setNumberOfPlayers(int newNumber) {
        if (newNumber > 0 && newNumber <= PLAYERMAX)
            numberOfPlayers = newNumber;
    }
    int getNumberOfPlayers() {
        return numberOfPlayers;
    }
    void setNumberOfDecks(int newNumber) {
        if (newNumber > 0 && newNumber <= DECKMAX)
            numberOfDecks = newNumber;
    }
    int getNumberOfDecks() {
        return numberOfDecks;
    }
    vector<Card*> getMainDeck() {
        return this->maindeck;
    }

    // checks if the main deck is empty before drawing. if it is, initialize and shuffle the deck. then remove a card from the deck and return that card.
    Card* drawCardManager() {
        if (maindeck.empty()) {
            this->initializeDeck();
            this->shuffleDeck();
        }
        Card* card = maindeck[maindeck.size() - 1];
        this->maindeck.pop_back();
        return card;
    }

    void discardCardManager(Card* tempCard) {
        this->discardDeck.push_back(tempCard);
    }

    // puts pointers to the unique cards stored in deck into the vector maindeck.
    void initializeDeck() {
        // clears the vectors before pointers are put into the decks
        this->maindeck.clear();
        this->discardDeck.clear();
        for (int i = 0; i < numberOfDecks; i++) {
            int x = 0;
            // I couldn't ficure out why I couldn't use the pointer for card and store it in maindeck, until I realized card was a temporary copy so &card didn't point to deck[x]
            for (Card card : deck) {
                this->maindeck.push_back(&(deck[x]));
                x++;
            }
        }
    }

    // shuffle boi (kinda based on the durstenfeld shuffle, I just used the js version as a reference and wrote a version for c++)
    void shuffleDeck() {
        // Mix_PlayChannel(-1, cardShuffleSnd, 0);

        srand(time(NULL));
        int i = (maindeck.size()) - 1;
        for (Card* card : maindeck) {
            int j = floor((rand() * (i + 1)) % maindeck.size());
            // cout << j << endl;
            Card* temp = maindeck[i];
            maindeck[i] = maindeck[j];
            maindeck[j] = temp;
            i--;
        }
    }

};

#endif

