#include "main.h"

int mainMenuOptions = 3;
int selectedMenuOption = 0;
char *mainmenuitems[3] = {
	"Start Game",
	"Options Menu",
	"Quit to Main Menu",
};

void mainmenu(bool top) {
	if (!top) return;
	u32 kDown = hidKeysDown();

	if (kDown & KEY_A) {
		switch (selectedMenuOption) {
			case 0:
				break;
			case 1:
				transitionState(GAME_STATE_OPTIONS, TRANSITION_TYPE_SWIPE_LEFT, seconds(1), EASE_IN_EXPO);
				break;
			case 2:
				transitionState(GAME_STATE_TITLE, TRANSITION_TYPE_CIRCLE, seconds(1.5), EASE_IN_CIRC);
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
	float xoffset = 0;
	float scale = 1;
	if (transition_timer != 0) {
		if (previous_state == GAME_STATE_OPTIONS || after_transition == GAME_STATE_OPTIONS) {
			xoffset = (TOP_SCREEN_WIDTH / 10) * lerp((transition_timer / (float)transition_duration), EASE_IN_EXPO);
		} else if (previous_state == GAME_STATE_TITLE || after_transition == GAME_STATE_TITLE) {
			scale = 1.05 - lerp((transition_timer / (float)transition_duration), EASE_IN_EXPO);
			scale = scale > 1 ? 1 : scale;
		}
	}

	draw_text_scale("Lucky Rollers", TOP_SCREEN_WIDTH / 2 - xoffset, 24, scale, scale);

	float offset = (TOP_SCREEN_HEIGHT - 64 - 48) / (mainMenuOptions - 1);
	for (int i = 0; i < mainMenuOptions; i++) {
		draw_text_colour_scale(mainmenuitems[i], TOP_SCREEN_WIDTH / 2 - xoffset, 64 + offset * i, scale, scale, i == selectedMenuOption ? C2D_Color32(0x38, 0xa3, 0xa5, 0xff) : C2D_Color32(0x9d, 0x02, 0x08, 0xff));
	}
}