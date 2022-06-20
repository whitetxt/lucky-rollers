#include "main.h"

void titlescreen_logic() {
	u32 kDown = hidKeysDown();

	if (kDown) {
		setState(GAME_STATE_MAIN_MENU, true, seconds(2));
	}
}