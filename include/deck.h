#pragma once
#include "tile.h"
#include <stdbool.h>
#define MAX_DECK 106

typedef struct Deck_s{
    Tile_t tiles[MAX_DECK];
    int count;
} Deck_t;

void deck_create_full(Deck_t *);
void deck_shuffle(Deck_t *);
bool deck_is_empty(const Deck_t *);
Tile_t deck_draw(Deck_t *);
