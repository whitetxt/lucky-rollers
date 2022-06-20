#include "main.h"

void titlescreen() {
	u32 kDown = hidKeysDown();

	if (kDown && transition_timer == 0) {
		transitionState(GAME_STATE_MAIN_MENU, TRANSITION_TYPE_SWIPE_LEFT, seconds(2), EASE_IN_EXPO);
	}

	draw_text("Lucky Rollers", TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2 + sin(frame_num / 20.0f) * 8);

	float alpha = 150 + sin(180 + frame_num / 40.0f) * 200;
	alpha = alpha > 255 ? 255 : alpha;
	draw_text_colour("Press any button to start.", TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT - 32, C2D_Color32(0, 0, 0, alpha));
}