#include "main.h"

circlePosition pos;

C3D_RenderTarget *top;
C3D_RenderTarget *bottom;
C2D_Font font;
C2D_SpriteSheet sprite_sheet;

C2D_Sprite sprites[MAX_SPRITES];

game_state state = GAME_STATE_TITLE;

int frame_num = 0;
int since_state_change = 0;

int transition_timer = 0;
int transition_duration = 0;
bool transition_forwards = true;
game_state after_transition;

u64 now_time, last_time;

bool paused;

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

float sign(point p1, point p2, point p3) {
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
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
 * @brief Loads the sprites from the sprite sheet
 */
void init_sprites() {
	sprite_sheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!sprite_sheet) svcBreak(USERBREAK_PANIC);
}

void delay() {
	frame_num++;
	/*now_time = osGetTime();
	u64 frame_time = now_time - last_time;
	last_time = now_time;
	if (frame_time < MS_PER_FRAME) {
		svcSleepThread(MS_PER_FRAME - frame_time);
	}*/
}

void spriteInit() {
	size_t num_images = C2D_SpriteSheetCount(sprite_sheet);
	for (int i = 0; i < num_images; i++) {
		C2D_SpriteFromSheet(&sprites[i], sprite_sheet, 0);
		C2D_SpriteSetCenter(&sprites[i], 0.5, 0.5);
		C2D_SpriteSetPos(&sprites[i], 0, 0);
		C2D_SpriteSetRotation(&sprites[i], 0);
		C2D_SpriteSetScale(&sprites[i], 1, 1);
	}
}

inline void setState(game_state new_state, bool transition, int frames) {
	if (!transition) {
		state = new_state;
		since_state_change = 0;
	} else {
		transition_timer = 1;
		transition_duration = frames;
		after_transition = new_state;
	}
}

inline C2D_Sprite get_sprite(int idx) {
	return sprites[idx];
}

inline C2D_Sprite *get_sprite_ptr(int idx) {
	return &sprites[idx];
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
	font = C2D_FontLoadSystem(CFG_REGION_EUR);

	init_sprites();
	spriteInit();

	
	// Main loop
	while (aptMainLoop()) {
		hidScanInput();
		u32 kDown = hidKeysDown();
		if ((kDown & KEY_L) && (kDown & KEY_R)) { // L + R to quit
			break; // break in order to return to hbmenu
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, colour_clear);
		C2D_SceneBegin(top);

		switch (state) {
			case GAME_STATE_TITLE:
				draw_titlescreen();
				titlescreen_logic();
				break;
			case GAME_STATE_MAIN_MENU:
				draw_menu();
				menu_logic();
				break;
			default:
				break;
		}

		if (transition_timer > 0) {
			float percentage = (transition_timer + transition_duration * 0.05f) / (float)transition_duration;
			if (percentage > 1) {
				percentage = 1;
			}
			double scale = 7.5;
			percentage = lerp(percentage, EASE_IN_EXPO);
			float offset = 32 * (scale - (percentage * scale));
			C2D_SpriteSetPos(get_sprite_ptr(SPRITE_FADE), TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2);
			C2D_SpriteSetScale(get_sprite_ptr(SPRITE_FADE), scale - (percentage * scale), scale - (percentage * scale));
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
				setState(after_transition, false, 0);
			}
		}

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