#include "data.h"
#include <stdlib.h>

Deck data_generate_deck(void) {
    Deck d;
    d.count = 0;
    return d;
}

void data_shuffle_deck(Deck *d) {
    // mélange du deck
}

Tile data_draw_tile(Deck *d) {
    Tile t = {0};
    if (d->count > 0)
        t = d->tiles[--d->count];
    return t;
}

void data_add_tile_to_board(Board *b, Tile t) {
    // ajout logique
}

void data_remove_tile_from_rack(Rack *r, int index) {
    for (int i = index; i < r->count - 1; i++)
        r->tiles[i] = r->tiles[i + 1];
    r->count--;
}
