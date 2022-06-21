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
void draw_text_raw(char *text, int x, int y, float x_scale, float y_scale, int flags, u32 colour) {
	Text *found = searchForCachedText(text);
	float w, h;
	if (!found) {
		C2D_TextBuf buf = C2D_TextBufNew(strlen(text));
		C2D_Text text_c2d;
		C2D_TextFontParse(&text_c2d, font, buf, text);
		C2D_TextGetDimensions(&text_c2d, x_scale, y_scale, &w, &h);
		C2D_TextOptimize(&text_c2d);
		C2D_DrawText(&text_c2d, C2D_WithColor | flags, x - w / 2, y - h / 2, 0, x_scale, y_scale, colour);
		cacheText(text_c2d, text);
	} else {
		C2D_TextGetDimensions(&found->t, x_scale, y_scale, &w, &h);
		C2D_DrawText(&found->t, C2D_WithColor | flags, x - w / 2, y - h / 2, 0, x_scale, y_scale, colour);
		cacheText(found->t, text);
	}
}