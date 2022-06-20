#include "main.h"

int mainMenuOptions = 3;
int selectedMenuOption = 0;
char *mainmenuitems[3] = {
	"Start Game",
	"Options Menu",
	"Quit to Main Menu",
};

void mainmenu() {
	u32 kDown = hidKeysDown();

	float x_offset = 0;

	if (kDown & KEY_A) {
		switch (selectedMenuOption) {
			case 0:
				break;
			case 1:
				break;
			case 2:
				transitionState(GAME_STATE_TITLE, TRANSITION_TYPE_CIRCLE, seconds(1), EASE_IN_CIRC);
				break;
			default:
				break;
		}
	}
	if ((kDown & KEY_DDOWN || kDown & KEY_CPAD_DOWN) && selectedMenuOption < mainMenuOptions - 1) {
		selectedMenuOption++;
	} else if ((kDown & KEY_DUP || kDown & KEY_CPAD_UP) && selectedMenuOption > 0) {
		selectedMenuOption--;
	}

	draw_text("Lucky Rollers", TOP_SCREEN_WIDTH / 2, 24);

	float offset = (TOP_SCREEN_HEIGHT - 64 - 48) / (mainMenuOptions - 1);
	for (int i = 0; i < mainMenuOptions; i++) {
		draw_text_colour(mainmenuitems[i], TOP_SCREEN_WIDTH / 2, 64 + offset * i, i == selectedMenuOption ? C2D_Color32(0x38, 0xa3, 0xa5, 0xff) : C2D_Color32(0x9d, 0x02, 0x08, 0xff));
	}
}