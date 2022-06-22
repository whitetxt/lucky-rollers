typedef enum CardValue {
	CARD_ACE = 0,
	CARD_2 = 1,
	CARD_3 = 2,
	CARD_4 = 3,
	CARD_5 = 4,
	CARD_6 = 5,
	CARD_7 = 6,
	CARD_8 = 7,
	CARD_9 = 8,
	CARD_10 = 9,
	CARD_JACK = 10,
	CARD_QUEEN = 11,
	CARD_KING = 12,
	CARD_JOKER = -1,
} CardValue;

typedef enum CardSuit {
	CARD_SPADES = 0,
	CARD_HEARTS = 1,
	CARD_DIAMONDS = 2,
	CARD_CLUBS = 3,
	CARD_SJOKER = -1,
} CardSuit;

typedef struct ActionCard {
	bool restrictSuit;
	bool suits[4];
	bool restrictValue;
	bool values[13];
	char *description;
	char *name;
} ActionCard;

typedef struct Card {
	CardValue value;
	CardSuit suit;
} Card;

typedef struct Deck {
	ActionCard actionCards[64];
	int numActionCards;
	Card cards[54];
	int numCards;
} Deck;

typedef struct Player {
	bool initialised;
	Deck deck;
	Card activeCards[54];
	int numActiveCards;
	int health;
	int maxHealth;
} Player;

Deck generateDeck();
void initialisePlayer(Player *player);
void game(bool top);