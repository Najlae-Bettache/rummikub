#include "../../include/deck.h"
#include <stdlib.h>
#include <stdio.h>

void deck_create_full(Deck_t *deck)
{
    int index = 0;
    for (int color = TILE_RED; color <= TILE_BLACK; color++) {
        for (int value = 1; value <= 13; value++) {
            deck->tiles[index] = tile_create(value, (TileColor)color, false, index);
            index++;
            deck->tiles[index] = tile_create(value, (TileColor)color, false, index);
            index++;
        }
    }
    deck->tiles[index] = tile_create(0, TILE_RED, true, index);
    index++;
    deck->tiles[index] = tile_create(0, TILE_BLUE, true, index);
    index++;
    deck->count = index;
}

void deck_shuffle(Deck_t *deck)
{
    for (int i = deck->count - 1; i >= 0; i--) {
        int j = rand() % (i + 1);
        Tile_t temp = deck->tiles[i];
        deck->tiles[i] = deck->tiles[j];
        deck->tiles[j] = temp;
    }
}

bool deck_is_empty(const Deck_t *deck)
{
    return deck->count == 0;
}

Tile_t deck_draw(Deck_t *deck)
{
    if (deck_is_empty(deck)) {
        fprintf(stderr, "Error: Attempt to draw from an empty deck.\n");
        exit(EXIT_FAILURE);
    }
    return deck->tiles[--deck->count];
}
