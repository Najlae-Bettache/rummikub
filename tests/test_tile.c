#include <assert.h>
#include "../include/tile.h"

int main(void) {
    Tile_t t1 = tile_create(5, TILE_RED, 0, 1);
    Tile_t t2 = tile_create(5, TILE_RED, 0, 1);
    Tile_t joker = tile_create(0, TILE_BLACK, 1, 99);

    assert(tile_equals(&t1, &t2));
    assert(!tile_is_joker(&t1));
    assert(tile_is_joker(&joker));

    return 0;
}
