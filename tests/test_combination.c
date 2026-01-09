#include <assert.h>
#include "../include/combination.h"

int main(void) {
    Combination_t c;
    combination_init(&c);

    assert(combination_is_empty(&c));

    Tile_t t1 = tile_create(1, TILE_RED, 0, 1);
    Tile_t t2 = tile_create(2, TILE_RED, 0, 2);

    assert(combination_add_tile(&c, t1));
    assert(combination_add_tile(&c, t2));
    assert(c.count == 2);

    Combination_t copy = combination_clone(&c);
    assert(copy.count == 2);

    combination_remove_tile_at(&c, 0);
    assert(c.count == 1);
    assert(copy.count == 2);

    return 0;
}
