#include "main.h"

Player pl = {
	.initialised = false,
	.maxHealth = 20,
};

Deck generateDeck() {
	Deck deck;
	deck.numCards = 54;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			deck.cards[i * 13 + j].suit = i;
			deck.cards[i * 13 + j].value = j;
		}
	}
	deck.cards[52].suit = 0;
	deck.cards[53].suit = 0;
	deck.cards[52].value = -1;
	deck.cards[53].value = -1;
	return deck;
}

Card drawCardFromDeck(Deck *deck) {
	int idx = rand() % deck->numCards;
	Card final = deck->cards[idx];
	for (int i = idx; i < deck->numCards; i++) {
		deck->cards[i] = deck->cards[i + 1];
	}
	deck->numCards--;
	return final;
}

void initialisePlayer(Player *player) {
	player->health = player->maxHealth;
	player->deck = generateDeck();
	for (int i = 0; i < 8; i++) {
		//player->activeCards[i] = drawCardFromDeck(&player->deck);
		//player->numActiveCards++;
	}
	player->initialised = true;
}

void game(bool top) {
	if (!pl.initialised) initialisePlayer(&pl);

	if (top) {

	} else {
		for (int i = 0; i < pl.deck.numActionCards; i++) {
			
		}
		/*for (int i = 0; i < pl.numActiveCards; i++) {
			C2D_Sprite *spr = getCardSprite(suitToTitle(pl.activeCards[i].suit), valueToTitle(pl.activeCards[i].value));
			int yoffset = (i / 8) * 38;
			int xoffset = (i % 8) * (BOTTOM_SCREEN_WIDTH / 9);
			C2D_SpriteSetScale(spr, 0.5, 0.5);
			C2D_SpriteSetPos(spr, xoffset + 16, TOP_SCREEN_HEIGHT - yoffset - 24);
			C2D_SpriteSetRotation(spr, 0);
			C2D_DrawSprite(spr);
		}*/
		for (int i = 0; i < pl.deck.numCards; i++) {
			C2D_Sprite *spr = getCardSprite(suitToTitle(pl.deck.cards[i].suit), valueToTitle(pl.deck.cards[i].value));
			int yoffset = (i / 8) * 32;
			int xoffset = (i % 8) * (BOTTOM_SCREEN_WIDTH / 9) + 16;
			C2D_SpriteSetScale(spr, 0.5, 0.5);
			C2D_SpriteSetPos(spr, xoffset, yoffset + 24);
			C2D_SpriteSetRotation(spr, 0);
			C2D_DrawSprite(spr);
		}
		C2D_SpriteSetScale(blankCard, 0.5, 0.5);
		C2D_SpriteSetRotation(blankCard, 0);
		C2D_SpriteSetPos(blankCard, BOTTOM_SCREEN_WIDTH - 24, BOTTOM_SCREEN_HEIGHT - 48);
		for (int i = 0; i < pl.deck.numCards; i++) {
			C2D_DrawSprite(blankCard);
			C2D_SpriteMove(blankCard, 0, -2);
		}
		char num[16];
		sprintf(num, "%d", pl.deck.numCards);
		draw_text(num, BOTTOM_SCREEN_WIDTH - 24, BOTTOM_SCREEN_HEIGHT - 24);
	}
}