#define MAX_CACHED_TEXT 128

typedef struct Text {
	C2D_Text t;
	char *text;
} Text;

extern int selectedMenuOption;
extern int mainMenuOptions;

void draw_text_raw(char *text, int x, int y, float x_scale, float y_scale, int flags, u32 colour);

#define draw_text(text, x, y) \
	draw_text_raw(text, x, y, 1, 1, 0, colour_black);

#define draw_text_colour(text, x, y, colour) \
	draw_text_raw(text, x, y, 1, 1, 0, colour);

#define draw_text_scale(text, x, y, scale_x, scale_y) \
	draw_text_raw(text, x, y, scale_x, scale_y, 0, colour_black);

#define draw_text_colour_scale(text, x, y, scale_x, scale_y, colour) \
	draw_text_raw(text, x, y, scale_x, scale_y, 0, colour);

void draw_titlescreen();
void draw_mainmenu();