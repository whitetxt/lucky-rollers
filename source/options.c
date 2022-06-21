#include "main.h"

int optionsOptions = 3;
int selectedOptionOption = 0;
char *optionsitems[3] = {
	"Force Save Data",
	"Reload Save Data",
	"Go Back",
};

void optionsmenu() {
	u32 kDown = hidKeysDown();

	if (kDown & KEY_A) {
		switch (selectedOptionOption) {
			case 0:
				break;
			case 1:
				break;
			case 2:
				transitionState(GAME_STATE_MAIN_MENU, TRANSITION_TYPE_SWIPE_RIGHT, seconds(1), EASE_IN_EXPO);
				break;
			default:
				break;
		}
	}
	if ((kDown & KEY_DDOWN || kDown & KEY_CPAD_DOWN) && selectedOptionOption < optionsOptions - 1) {
		selectedOptionOption++;
	} else if ((kDown & KEY_DUP || kDown & KEY_CPAD_UP) && selectedOptionOption > 0) {
		selectedOptionOption--;
	}

	draw_text("Lucky Rollers", TOP_SCREEN_WIDTH / 2, 24);

	float offset = (TOP_SCREEN_HEIGHT - 64 - 48) / (optionsOptions - 1);
	for (int i = 0; i < optionsOptions; i++) {
		draw_text_colour(optionsitems[i], TOP_SCREEN_WIDTH / 2, 64 + offset * i, i == selectedOptionOption ? C2D_Color32(0x38, 0xa3, 0xa5, 0xff) : C2D_Color32(0x9d, 0x02, 0x08, 0xff));
	}
}