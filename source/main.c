#include "main.h"

/**
 * @brief Reads the state of the circle pad, and takes into account the dead zone.
 * 
 * @param dead_zone Values less than this will be considered as 0.
 * @return circlePosition 
 */
circlePosition readCStick(int dead_zone) {
	circlePosition pos;

	hidCircleRead(&pos);

	if (abs(pos.dx) < dead_zone) {
		pos.dx = 0;
	}
	if (abs(pos.dy) < dead_zone) {
		pos.dy = 0;
	}
	return pos;
}

float sign (point p1, point p2, point p3) {
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

/**
 * @brief Converts seconds into frames
 * 
 * @param secs Number of seconds
 * @return int
 */
int seconds(float secs) {
	return FRAMERATE * secs;
}

/**
 * @brief Create a bullet object
 * 
 * @param x Bullet X
 * @param y Bullet Y
 * @param from The type of enemy this came from
 * @param vel_x Starting X velocity
 * @param vel_y Starting Y velocity
 */
void create_bullet(float x, float y, enemy_type from, float vel_x, float vel_y) {
	if (num_bullets >= MAX_BULLETS) {
		return;
	}
	bullets[num_bullets].from = from;
	bullets[num_bullets].pos.x = x;
	bullets[num_bullets].pos.y = y;
	bullets[num_bullets].vel.x = vel_x;
	bullets[num_bullets].vel.y = vel_y;
	num_bullets++;
}

/**
 * @brief Deletes a bullet from the array.
 * 
 * @param idx The index to remove
 */
void delete_bullet(int idx) {
	if (idx >= num_bullets) {
		return;
	}
	for (int i = idx; i < num_bullets; i++) {
		bullets[i] = bullets[i + 1];
	}
	num_bullets--;
}

/**
 * @brief Loads the sprites from the sprite sheet
 * 
 */
void init_sprites() {
	sprite_sheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!sprite_sheet) {
		svcBreak(USERBREAK_PANIC);
	}
}

void init_level_text(C2D_Font font, C2D_TextBuf buf) {
	C2D_TextFontParse(&level_text[0], font, buf, "Introduction");
	C2D_TextFontParse(&level_text[1], font, buf, "The Beginning");
	C2D_TextFontParse(&level_text[2], font, buf, "Progging Land");
	C2D_TextFontParse(&level_text[3], font, buf, "The Intense Progger Arrives!");
	C2D_TextFontParse(&level_text[4], font, buf, "The End?");

	for (int i = 0; i < 5; i++) {
		C2D_TextOptimize(&level_text[i]);
	}

	C2D_TextFontParse(&unlocked_text, font, buf, "Debug Mode Active");
	C2D_TextOptimize(&unlocked_text);
}

void delay() {
	frame_num++;
	now_time = osGetTime();
	u64 frame_time = now_time - last_time;
	last_time = now_time;
	if (frame_time < MS_PER_FRAME) {
		svcSleepThread(MS_PER_FRAME - frame_time);
	}
}

bool inside_tri(point p, point tri_1, point tri_2, point tri_3) {
	float d1 = sign(p, tri_1, tri_2);
	float d2 = sign(p, tri_2, tri_3);
	float d3 = sign(p, tri_3, tri_1);

	bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
	return !(has_neg && has_pos);
}

bool inside_rect(point p, point top_left, int w, int h) {
	return 	(p.x < top_left.x + w && p.x > top_left.x) &&
			(p.y > top_left.y && p.y < top_left.y + h);
}

circlePosition pos;

C3D_RenderTarget *top;
C3D_RenderTarget *bottom;

void spriteInit() {
	size_t numImages = C2D_SpriteSheetCount(sprite_sheet);
	for (size_t i = 0; i < numImages; i++) {
		C2D_SpriteFromSheet(&boss_sprites[i], sprite_sheet, i);
		C2D_SpriteSetCenter(&boss_sprites[i], 0.5f, 0.5f);
		C2D_SpriteSetPos(&boss_sprites[i], 0, 0);
		C2D_SpriteSetRotation(&boss_sprites[i], 0);
	}
}

inline void setState(game_state new_state) {
	state = new_state;
	since_state_change = 0;
}

inline float dist(point a, point b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

inline float dist_sq(point a, point b) {
	return pow(a.x - b.x, 2) + pow(a.y - b.y, 2);
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	srvInit(); // Init services
	aptInit(); // Init applets
	hidInit(); // Init input
	romfsInit(); // Init romfs
	gfxInitDefault(); // Init graphics
	gfxSet3D(false); // Set 3D off

	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	//sprite_sheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	//if (!sprite_sheet) svcBreak(USERBREAK_PANIC);

	spriteInit();

	init_sprites();
	// Main loop
	while (aptMainLoop()) {
		hidScanInput();
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		if ((kHeld & KEY_L) && (kHeld & KEY_R)) { // L + R to quit
			break; // break in order to return to hbmenu
		}

		if (kDown & KEY_START) {
			paused = !paused;
			selected_option = 0;
		}

		if (paused) {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, colour_clear);
			C2D_SceneBegin(top);

			C2D_DrawText(&paused_text, 0, TOP_SCREEN_WIDTH / 2 - paused_text.width / 2, TOP_SCREEN_HEIGHT / 2, 0.0f, 1, 1);

			C3D_FrameEnd(0);
			delay();
			continue;
		}

		if (state == STATE_DEAD) {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, colour_clear);
			C2D_SceneBegin(top);

			C2D_DrawText(&dead_text, C2D_AtBaseline, TOP_SCREEN_WIDTH / 2 - dead_text.width / 2, TOP_SCREEN_HEIGHT / 2, 0.0f, 1, 1);

			if (since_state_change > seconds(2.5f)) {
				while (num_enemies > 0) {
					remove_enemy(0);
				}
				while (num_bullets > 0) {
					delete_bullet(0);
				}
				setState(STATE_MAP);
			}
		} else if (state == STATE_LOSE_TIME) {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, colour_clear);
			C2D_SceneBegin(top);

			C2D_DrawText(&time_lose_text, C2D_AtBaseline, TOP_SCREEN_WIDTH / 2 - time_lose_text.width / 2, TOP_SCREEN_HEIGHT / 2, 0.0f, 1, 1);

			if (since_state_change > seconds(2.5f)) {
				while (num_enemies > 0) {
					remove_enemy(0);
				}
				while (num_bullets > 0) {
					delete_bullet(0);
				}
				score = 0;
				setState(STATE_MAP);
			}
		} else if (state == STATE_MAP) {
			pos = readCStick(CSTICK_DEAD_ZONE);
			if (since_map_change > seconds(1)) {
				if (kDown & KEY_CPAD_RIGHT) {
					selected_map++;
					if (selected_map >= num_levels) {
						selected_map--;
					} else {
						selected_map_changed = true;
						last_selected_map = selected_map - 1;
						since_map_change = 0;
					}
				}
				if (kDown & KEY_CPAD_LEFT) {
					selected_map--;
					if (selected_map < 0) {
						selected_map++;
					} else {
						selected_map_changed = true;
						last_selected_map = selected_map + 1;
						since_map_change = 0;
					}
				}
			}
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, colour_clear);
			C2D_SceneBegin(top);
			draw_map(unlocked);
			if (kHeld & KEY_L && kHeld & KEY_X && kHeld & KEY_Y) {
				unlocked = true;
			} else if (kDown & KEY_A) { // Press A to select a level
				if (unlocked || 
					((selected_map == 0 || won_levels[selected_map - 1]) && !won_levels[selected_map])) {
					setState(STATE_LEVEL);
					next_level = selected_map + 1;
					last_wave_spawn = 0;
					next_wave = 0;
				}
			}
			since_map_change++;
		} else if (state == STATE_LEVEL) {

			// Read C Stick
			pos = readCStick(CSTICK_DEAD_ZONE);

			// Invert y axis
			pos.dy *= -1;

			// Start of draw routine
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, colour_clear);
			C2D_SceneBegin(top);

			C2D_DrawCircleSolid(10, 10, 0, 4, C2D_Color32(255, 255, 255, 255));

		} else if (state == STATE_LEVEL_WIN) {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, colour_clear);
			C2D_SceneBegin(top);

			C2D_DrawCircleSolid(10, 10, 0, 4, C2D_Color32(0, 255, 255, 255));

		}

		C2D_TargetClear(bottom, colour_clear);
		C2D_SceneBegin(bottom);
		C3D_FrameDrawOn(bottom);

		C3D_FrameEnd(0);
		delay();
	}

	C2D_Fini();
	C3D_Fini();
	hidExit();
	aptExit();
	srvExit();
	gfxExit();
	romfsExit();
	return 0;
}

void draw_text(char *text, int x, int y, int scalex, int scaley, float *width, float *height) {
	C2D_Text text_obj;
	C2D_TextBuf buf = C2D_TextBufNew(strlen(text));
	C2D_TextFontParse(&text_obj, font, buf, text);
	C2D_TextOptimize(&text_obj);
	float w;
	if (!width) width = &w;
	C2D_TextGetDimensions(&text_obj, scalex, scaley, width, height);
	C2D_DrawText(&text_obj, 0, x - (w / 2.0f), y, 0, scalex, scaley);
	C2D_TextBufDelete(buf);
}

void set_bonus_active(char *name, bool active) {
	for (int i = 0; i < num_bonuses; i++) {
		if (strcmp(name, bonuses[i].name) == 0) {
			bonuses[i].active = active;
			return;
		}
	}
}