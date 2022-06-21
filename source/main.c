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
	now_time = osGetTime();
	u64 frame_time = now_time - last_time;
	last_time = now_time;
	if (frame_time < MS_PER_FRAME) {
		svcSleepThread(MS_PER_FRAME - frame_time);
	}
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

inline void setState(game_state new_state) {
	state = new_state;
	since_state_change = 0;
}

inline void transitionState(game_state new_state, transition_type type, int frames, lerp_function trans_lerp) {
	transition_timer = 1;
	transition_duration = frames;
	after_transition = new_state;
	transition_forwards = true;
	transition_lerp = trans_lerp;
	trans_type = type;
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
				titlescreen();
				break;
			case GAME_STATE_MAIN_MENU:
				mainmenu();
				break;
			default:
				break;
		}

		transition();

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