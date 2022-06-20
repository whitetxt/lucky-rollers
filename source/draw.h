#define MAX_CACHED_TEXT 128

typedef struct Text {
	C2D_Text t;
	char *text;
} Text;

void draw_text(char *text, int x, int y);
void draw_text_colour(char *text, int x, int y, u32 colour);
void draw_titlescreen();
void draw_menu();