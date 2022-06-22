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
	deck.cards[52].suit = CARD_SJOKER;
	deck.cards[53].suit = CARD_SJOKER;
	deck.cards[52].value = CARD_JOKER;
	deck.cards[53].value = CARD_JOKER;
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
	for (int i = 0; i < 6; i++) {
		player->activeCards[i] = drawCardFromDeck(&player->deck);
		player->numActiveCards++;
	}
	player->initialised = true;
}

void game(bool top) {
	if (!pl.initialised) initialisePlayer(&pl);

	if (top) {

	} else {
		for (int i = 0; i < pl.deck.numActionCards; i++) {
			
		}
		for (int i = 0; i < pl.numActiveCards; i++) {
			C2D_Sprite *spr = getCardSprite(pl.activeCards[i].suit, pl.activeCards[i].value);
			int yoffset = (i / 6) * 32;
			int xoffset = (i % 6) * (BOTTOM_SCREEN_WIDTH / 7.0f) + 24;
			C2D_SpriteSetScale(spr, 0.65, 0.65);
			C2D_SpriteSetPos(spr, xoffset, TOP_SCREEN_HEIGHT - yoffset - 32);
			C2D_SpriteSetRotation(spr, 0);
			C2D_DrawSprite(spr);
		}

		C2D_SpriteSetScale(backCard, 0.65, 0.65);
		C2D_SpriteSetRotation(backCard, 0);
		C2D_SpriteSetPos(backCard, BOTTOM_SCREEN_WIDTH - 24, BOTTOM_SCREEN_HEIGHT - 64);
		for (int i = 0; i < pl.deck.numCards; i++) {
			C2D_DrawSprite(backCard);
			C2D_SpriteMove(backCard, 0, -2);
		}
		char num[16];
		sprintf(num, " %d ", pl.deck.numCards);
		draw_text_scale(num, BOTTOM_SCREEN_WIDTH - 24, BOTTOM_SCREEN_HEIGHT - 16, 0.9, 0.9);
	}
}