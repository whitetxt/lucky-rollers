typedef enum title_card_suit {
	CLUBS = 1,
	DIAMONDS = 2,
	HEARTS = 3,
	SPADES = 4,
} title_card_suit;

typedef enum title_card_value {
	TITLE_CARD_10 = 0,
	TITLE_CARD_2 = 1,
	TITLE_CARD_3 = 2,
	TITLE_CARD_4 = 3,
	TITLE_CARD_5 = 4,
	TITLE_CARD_6 = 5,
	TITLE_CARD_7 = 6,
	TITLE_CARD_8 = 7,
	TITLE_CARD_9 = 8,
	TITLE_CARD_ACE = 9,
	TITLE_CARD_JACK = 10,
	TITLE_CARD_KING = 11,
	TITLE_CARD_QUEEN = 12,
	TITLE_CARD_JOKER = 13,
} title_card_value;

typedef struct title_card {
	title_card_suit suit;
	title_card_value value;
	point position;
	int rotational_vel;
	point vel;
} title_card;

void titlescreen(bool top);