#include "main.h"

int transition_timer = 0;
int transition_duration = 0;
bool transition_forwards = true;
game_state after_transition;
lerp_function transition_lerp = EASE_IN_EXPO;
transition_type trans_type = TRANSITION_TYPE_CIRCLE;

void transition(bool top) {
	int width, height;
	if (top) {
		width = TOP_SCREEN_WIDTH;
		height = TOP_SCREEN_HEIGHT;
	} else {
		width = BOTTOM_SCREEN_WIDTH;
		height = BOTTOM_SCREEN_HEIGHT;
	}
	if (transition_timer > 0) {
		float percentage = (transition_timer + transition_duration * 0.05f) / (float)transition_duration;
		if (percentage > 1) {
			percentage = 1;
		}
		double scale;
		float offset, scale_fac;
		switch (trans_type) {
			default:
			case TRANSITION_TYPE_CIRCLE:
				scale = 7.5;
				percentage = lerp(percentage, transition_lerp);
				offset = 32 * (scale - (percentage * scale));
				scale_fac = scale - (percentage * scale);
				if (scale_fac < 0.1f) {
					scale_fac = 0.1f;
				}
				C2D_SpriteSetPos(get_sprite_ptr(SPRITE_FADE), width / 2, height / 2);
				C2D_SpriteSetScale(get_sprite_ptr(SPRITE_FADE), scale_fac, scale_fac);
				C2D_DrawSprite(get_sprite_ptr(SPRITE_FADE));
				C2D_DrawRectSolid(0, 0, 0, width, height / 2 - offset, colour_black);
				C2D_DrawRectSolid(0, height / 2 + offset, 0, width, height / 2, colour_black);
				C2D_DrawRectSolid(0, 0, 0, width / 2 - offset, height, colour_black);
				C2D_DrawRectSolid(width / 2 + offset, 0, 0, width / 2, height, colour_black);
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
			case TRANSITION_TYPE_SWIPE_LEFT:
				scale = 5;
				percentage = lerp(percentage, transition_lerp);
				C2D_DrawRectSolid(width - (percentage) * width, 0, 0, width, height, colour_black);
				if (transition_forwards) {
					transition_timer++;
				} else {
					transition_timer--;
				}
				if (transition_timer >= transition_duration) {
					transition_forwards = false;
					trans_type = TRANSITION_TYPE_SWIPE_RIGHT;
					setState(after_transition);
				}
				break;
			case TRANSITION_TYPE_SWIPE_RIGHT:
				scale = 5;
				percentage = lerp(percentage, transition_lerp);
				C2D_DrawRectSolid(0, 0, 0, (percentage) * width, height, colour_black);
				if (transition_forwards) {
					transition_timer++;
				} else {
					transition_timer--;
				}
				if (transition_timer >= transition_duration) {
					transition_forwards = false;
					trans_type = TRANSITION_TYPE_SWIPE_LEFT;
					setState(after_transition);
				}
				break;
			case TRANSITION_TYPE_SWIPE_UP:
				scale = 5;
				percentage = lerp(percentage, transition_lerp);
				C2D_DrawRectSolid(0, height - percentage * height, 0, width, height, colour_black);
				if (transition_forwards) {
					transition_timer++;
				} else {
					transition_timer--;
				}
				if (transition_timer >= transition_duration) {
					transition_forwards = false;
					trans_type = TRANSITION_TYPE_SWIPE_DOWN;
					setState(after_transition);
				}
				break;
			case TRANSITION_TYPE_SWIPE_DOWN:
				scale = 5;
				percentage = lerp(percentage, transition_lerp);
				C2D_DrawRectSolid(0, 0, 0, width, percentage * height, colour_black);
				if (transition_forwards) {
					transition_timer++;
				} else {
					transition_timer--;
				}
				if (transition_timer >= transition_duration) {
					transition_forwards = false;
					trans_type = TRANSITION_TYPE_SWIPE_UP;
					setState(after_transition);
				}
				break;
		}
	}
}