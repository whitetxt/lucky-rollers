#include "main.h"

int transition_timer = 0;
int transition_duration = 0;
bool transition_forwards = true;
game_state after_transition;
lerp_function transition_lerp = EASE_IN_EXPO;
transition_type trans_type = TRANSITION_TYPE_CIRCLE;

void transition() {
	if (transition_timer > 0) {
		switch (trans_type) {
			default:
			case TRANSITION_TYPE_CIRCLE:
				float percentage = (transition_timer + transition_duration * 0.05f) / (float)transition_duration;
				if (percentage > 1) {
					percentage = 1;
				}
				double scale = 7.5;
				percentage = lerp(percentage, transition_lerp);
				float offset = 32 * (scale - (percentage * scale));
				float scale_fac = scale - (percentage * scale);
				if (scale_fac < 0.1f) {
					scale_fac = 0.1f;
				}
				C2D_SpriteSetPos(get_sprite_ptr(SPRITE_FADE), TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2);
				C2D_SpriteSetScale(get_sprite_ptr(SPRITE_FADE), scale_fac, scale_fac);
				C2D_DrawSprite(get_sprite_ptr(SPRITE_FADE));
				C2D_DrawRectSolid(0, 0, 0, TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT / 2 - offset, colour_black);
				C2D_DrawRectSolid(0, TOP_SCREEN_HEIGHT / 2 + offset, 0, TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT / 2, colour_black);
				C2D_DrawRectSolid(0, 0, 0, TOP_SCREEN_WIDTH / 2 - offset, TOP_SCREEN_HEIGHT, colour_black);
				C2D_DrawRectSolid(TOP_SCREEN_WIDTH / 2 + offset, 0, 0, TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT, colour_black);
				if (transition_forwards) {
					transition_timer++;
				} else {
					transition_timer--;
				}
				if (transition_timer >= transition_duration) {
					transition_forwards = false;
					setState(after_transition);
				}
				break;
		}
	}
}