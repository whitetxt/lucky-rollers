typedef struct TitleCard {
	CardSuit suit;
	CardValue value;
	point position;
	int rotational_vel;
	point vel;
} TitleCard;

void titlescreen(bool top);