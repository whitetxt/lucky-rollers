#pragma once

#define TOP_SCREEN_WIDTH  400
#define TOP_SCREEN_HEIGHT 240

#define BOTTOM_SCREEN_WIDTH 320
#define BOTTOM_SCREEN_HEIGHT 240

#define CSTICK_DEAD_ZONE 12

#define FRAMERATE 60
#define MS_PER_FRAME 1000 / FRAMERATE

#define MAX_SPRITES 768

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <citro2d.h>
#include <time.h>
#include <math.h>

#include "colours.h"
#include "math.h"
#include "draw.h"

#include "logic.h"

typedef enum sprite {
	SPRITE_FADE = 0,
} sprite;

typedef enum game_state {
	GAME_STATE_TITLE = 0,
	GAME_STATE_MAIN_MENU = 1,
} game_state;

typedef struct point {
	float x, y;
} point;

typedef struct line {
	point start, end;
	float m, c;
} line;

extern C3D_RenderTarget *top;
extern C3D_RenderTarget *bottom;
extern C2D_Font font;
extern C2D_SpriteSheet sprite_sheet;

extern game_state state;

extern int frame_num;
extern int since_state_change;

extern bool paused;

C2D_Sprite get_sprite(int idx);
C2D_Sprite *get_sprite_ptr(int idx);

bool inside_tri(point p, point tri_1, point tri_2, point tri_3);
bool inside_rect(point p, point top_left, int w, int h);

circlePosition readCStick(int dead_zone);
float sign(point p1, point p2, point p3);
int seconds(float secs);
void setState(game_state new_state, bool transition, int frames);
float dist(point a, point b);
float dist_sq(point a, point b);