typedef struct card {
	card_suit suit;
	card_value value;
	point position;
	int rotational_vel;
	point vel;
} card;

void titlescreen(bool top);