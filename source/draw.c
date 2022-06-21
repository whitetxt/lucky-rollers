#include "main.h"

Text cached[MAX_CACHED_TEXT];
int num_cached = 0;

/**
 * @brief Searches for a text string that has been cached.
 * 
 * @param text Text to search for
 * @return Text* on success, NULL on failure.
 */
Text *searchForCachedText(char *text) {
	for (int i = 0; i < num_cached; i++) {
		if (strcmp(cached[i].text, text) == 0) {
			return &cached[i];
		}
	}
	return NULL;
}

/**
 * @brief Push text to the cache.
 * 
 * @param t Text object to push
 * @param text Text that the object says.
 */
void cacheText(C2D_Text t, char *text) {
	int found = -1;
	for (int i = 0; i < num_cached; i++) {
		if (strcmp(cached[i].text, text) == 0) {
			found = i;
		}
	}
	if (found == -1) {
		int position = num_cached >= MAX_CACHED_TEXT ? MAX_CACHED_TEXT - 1 : num_cached;

		C2D_TextBufDelete(cached[position].t.buf);
		cached[position].t = t;
		free(cached[position].text);
		cached[position].text = strdup(text);

		num_cached++;
		if (num_cached > MAX_CACHED_TEXT) {
			num_cached = MAX_CACHED_TEXT;
		}
	} else if (found != 0) {
		for (int i = found; i > 0; i--) {
			cached[i] = cached[i - 1];
		}
		cached[0].t = t;
		free(cached[0].text);
		cached[0].text = strdup(text);
	}
}

/**
 * @brief Draws text to the screen at the specified position.
 * Position used is for the centre of the text.
 * 
 * @param text Text to draw
 * @param x X Position
 * @param y Y Position
 */
void draw_text(char *text, int x, int y) {
	Text *found = searchForCachedText(text);
	float w, h;
	if (!found) {
		C2D_TextBuf buf = C2D_TextBufNew(strlen(text));
		C2D_Text text_c2d;
		C2D_TextFontParse(&text_c2d, font, buf, text);
		C2D_TextGetDimensions(&text_c2d, 1, 1, &w, &h);
		C2D_TextOptimize(&text_c2d);
		C2D_DrawText(&text_c2d, 0, x - w / 2, y - h / 2, 0, 1, 1);
		cacheText(text_c2d, text);
	} else {
		C2D_TextGetDimensions(&found->t, 1, 1, &w, &h);
		C2D_DrawText(&found->t, 0, x - w / 2, y - h / 2, 0, 1, 1);
		cacheText(found->t, text);
	}
}

/**
 * @brief Draws text with a specified colour.
 * 
 * @param text Text to draw
 * @param x X Position
 * @param y Y Position
 * @param colour Colour to draw as
 */
void draw_text_colour(char *text, int x, int y, u32 colour) {
	float w, h;
	C2D_TextBuf buf = C2D_TextBufNew(strlen(text));
	C2D_Text text_c2d;
	C2D_TextFontParse(&text_c2d, font, buf, text);
	C2D_TextGetDimensions(&text_c2d, 1, 1, &w, &h);
	C2D_TextOptimize(&text_c2d);
	C2D_DrawText(&text_c2d, C2D_WithColor, x - w / 2, y - h / 2, 0, 1, 1, colour);
	C2D_TextBufDelete(buf);
}