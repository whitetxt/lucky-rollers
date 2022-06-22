#include "main.h"

bool initialised = false;

title_card cards[32];
int max_vel = 64;
int max_rot_vel = 100;

void titlescreen(bool top) {
	if (top) {
		if (!initialised) {
			for (int i = 0; i < 16; i++) {
				cards[i].suit = rand() % 4;
				cards[i].value = rand() % 14;
				cards[i].position.x = rand() % TOP_SCREEN_WIDTH;
				cards[i].position.y = rand() % TOP_SCREEN_HEIGHT;
				cards[i].rotational_vel = rand() % max_rot_vel - max_rot_vel / 2;
				cards[i].vel.x = rand() % max_vel - max_vel / 2;
				cards[i].vel.y = rand() % max_vel - max_vel / 2;
			}
			for (int i = 16; i < 32; i++) {
				cards[i].suit = rand() % 4;
				cards[i].value = rand() % 14;
				cards[i].position.x = rand() % BOTTOM_SCREEN_WIDTH;
				cards[i].position.y = rand() % BOTTOM_SCREEN_HEIGHT;
				cards[i].rotational_vel = rand() % max_rot_vel - max_rot_vel / 2;
				cards[i].vel.x = rand() % max_vel - max_vel / 2;
				cards[i].vel.y = rand() % max_vel - max_vel / 2;
			}
			initialised = true;
		}

		u32 kDown = hidKeysDown();

		if (kDown && transition_timer == 0) {
			transitionState(GAME_STATE_MAIN_MENU, TRANSITION_TYPE_CIRCLE, seconds(1.5), EASE_IN_CUBIC);
		}

		if (transition_timer == transition_duration - 1) {
			initialised = false;
		}

		draw_text("Lucky Rollers", TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2 + sin(frame_num / 20.0f) * 8);

		for (int i = 0; i < 16; i++) {
			C2D_SetTintMode(C2D_TintMult);
			C2D_ImageTint tint;
			C2D_AlphaImageTint(&tint, 0.05);
			C2D_Sprite *spr = getCardSprite(cards[i].suit, cards[i].value);
			C2D_SpriteSetPos(spr, cards[i].position.x, cards[i].position.y);
			C2D_SpriteRotateDegrees(spr, cards[i].rotational_vel * (1.0f / FRAMERATE));
			C2D_SpriteSetScale(spr, 0.5, 0.5);
			cards[i].position.x += cards[i].vel.x * (1.0f / FRAMERATE);
			cards[i].position.y += cards[i].vel.y * (1.0f / FRAMERATE);
			C2D_DrawSpriteTinted(spr, &tint);

			if (cards[i].position.y < 0) {
				cards[i].vel.y = rand() % max_vel / 2;
				cards[i].rotational_vel = rand() % max_rot_vel - max_rot_vel / 2;
			} else if (cards[i].position.y > TOP_SCREEN_HEIGHT) {
				cards[i].vel.y = rand() % max_vel / 2 - max_vel / 2;
				cards[i].rotational_vel = rand() % max_rot_vel - max_rot_vel / 2;
			}

			if (cards[i].position.x < 0) {
				cards[i].vel.x = rand() % max_vel / 2;
				cards[i].rotational_vel = rand() % max_rot_vel - max_rot_vel / 2;
			} else if (cards[i].position.x > TOP_SCREEN_WIDTH) {
				cards[i].vel.x = rand() % max_vel / 2 - max_vel / 2;
				cards[i].rotational_vel = rand() % max_rot_vel - max_rot_vel / 2;
			}
		}
	} else {
		for (int i = 16; i < 32; i++) {
			C2D_SetTintMode(C2D_TintMult);
			C2D_ImageTint tint;
			C2D_AlphaImageTint(&tint, 0.05);
			C2D_Sprite *spr = getCardSprite(cards[i].suit, cards[i].value);
			C2D_SpriteSetPos(spr, cards[i].position.x, cards[i].position.y);
			C2D_SpriteRotateDegrees(spr, cards[i].rotational_vel * (1.0f / FRAMERATE));
			C2D_SpriteSetScale(spr, 0.5, 0.5);
			cards[i].position.x += cards[i].vel.x * (1.0f / FRAMERATE);
			cards[i].position.y += cards[i].vel.y * (1.0f / FRAMERATE);
			C2D_DrawSpriteTinted(spr, &tint);

			if (cards[i].position.y < 0) {
				cards[i].vel.y = rand() % max_vel / 2;
				cards[i].rotational_vel = rand() % max_rot_vel - max_rot_vel / 2;
			} else if (cards[i].position.y > BOTTOM_SCREEN_HEIGHT) {
				cards[i].vel.y = rand() % max_vel / 2 - max_vel / 2;
				cards[i].rotational_vel = rand() % max_rot_vel - max_rot_vel / 2;
			}

			if (cards[i].position.x < 0) {
				cards[i].vel.x = rand() % max_vel / 2;
				cards[i].rotational_vel = rand() % max_rot_vel - max_rot_vel / 2;
			} else if (cards[i].position.x > BOTTOM_SCREEN_WIDTH) {
				cards[i].vel.x = rand() % max_vel / 2 - max_vel / 2;
				cards[i].rotational_vel = rand() % max_rot_vel - max_rot_vel / 2;
			}
		}

		float alpha = 150 + sin(180 + frame_num / 40.0f) * 200;
		alpha = alpha > 255 ? 255 : alpha;
		draw_text_colour("Press any button to start.", BOTTOM_SCREEN_WIDTH / 2, BOTTOM_SCREEN_HEIGHT / 2, C2D_Color32(0, 0, 0, alpha));
	}
}