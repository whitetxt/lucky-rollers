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

typedef enum sprite {
	SPRITE_FADE = 0,
} sprite;

typedef enum game_state {
	GAME_STATE_TITLE,
	GAME_STATE_MAIN_MENU,
	GAME_STATE_OPTIONS,
	GAME_STATE_IN_GAME,
} game_state;

typedef enum card_suit {
	CLUBS = 1,
	DIAMONDS = 2,
	HEARTS = 3,
	SPADES = 4,
} card_suit;

typedef enum card_value {
	CARD_10 = 0,
	CARD_2 = 1,
	CARD_3 = 2,
	CARD_4 = 3,
	CARD_5 = 4,
	CARD_6 = 5,
	CARD_7 = 6,
	CARD_8 = 7,
	CARD_9 = 8,
	CARD_ACE = 9,
	CARD_JACK = 10,
	CARD_KING = 11,
	CARD_QUEEN = 12,
	CARD_JOKER = 13,
} card_value;

typedef struct point {
	float x, y;
} point;

typedef struct line {
	point start, end;
	float m, c;
} line;

#include "colours.h"
#include "math.h"
#include "draw.h"
#include "transitions.h"

#include "game.h"

#include "title.h"
#include "mainmenu.h"
#include "options.h"

extern C3D_RenderTarget *top;
extern C3D_RenderTarget *bottom;
extern C2D_Font font;
extern C2D_SpriteSheet sprite_sheet;

extern game_state state;
extern game_state previous_state;

extern int frame_num;
extern int since_state_change;

extern int transition_timer;

extern bool paused;

C2D_Sprite get_sprite(int idx);
C2D_Sprite *get_sprite_ptr(int idx);
C2D_Sprite *getCardSprite(card_suit suit, card_value value);

bool inside_tri(point p, point tri_1, point tri_2, point tri_3);
bool inside_rect(point p, point top_left, int w, int h);

circlePosition readCStick(int dead_zone);
float sign(point p1, point p2, point p3);
int seconds(float secs);
void setState(game_state new_state);
void transitionState(game_state new_state, transition_type type, int frames, lerp_function trans_lerp);
float dist(point a, point b);
float dist_sq(point a, point b);