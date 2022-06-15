#pragma once

#define TOP_SCREEN_WIDTH  400
#define TOP_SCREEN_HEIGHT 240

#define BOTTOM_SCREEN_WIDTH 320
#define BOTTOM_SCREEN_HEIGHT 240

#define CSTICK_DEAD_ZONE 12

#define FRAMERATE 60
#define MS_PER_FRAME 1000 / FRAMERATE

#define MAX_ENEMIES 512
#define MAX_BULLETS 1024
#define MAX_SPRITES 768

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <citro2d.h>
#include <time.h>

typedef enum game_state {
	STATE_MAP = 0,
	STATE_LEVEL = 1,
	STATE_GAME_OVER = 2,
	STATE_LEVEL_WIN = 3,
	STATE_BOSS_LEVEL = 4,
	STATE_DEAD = 5,
	STATE_LOSE_TIME = 6,
} game_state;

typedef struct point {
	float x, y;
} point;

typedef struct line {
	point start, end;
	float m, c;
} line;

bool inside_tri(point p, point tri_1, point tri_2, point tri_3);
bool inside_rect(point p, point top_left, int w, int h);

circlePosition readCStick(int dead_zone);
float sign (point p1, point p2, point p3);
void create_bullet(float x, float y, enemy_type from, float vel_x, float vel_y);
void delete_bullet(int idx);
int seconds(float secs);
void setState(game_state new_state);
float dist(point a, point b);
float dist_sq(point a, point b);

extern circlePosition pos;

extern C3D_RenderTarget *top;
extern C3D_RenderTarget *bottom;

void draw_text(char *text, int x, int y, int scalex, int scaley, float *width, float *height);