#include <assert.h>
#include "../include/rack.h"

int main(void) {
    Rack_t r;
    rack_init(&r);

    Tile_t t1 = tile_create(7, TILE_YELLOW, 0, 10);
    Tile_t t2 = tile_create(8, TILE_YELLOW, 0, 11);

    rack_add_tile(&r, t1);
    rack_add_tile(&r, t2);

    assert(rack_contains_tile(&r, &t1));
    assert(rack_count(&r) == 2);

    rack_remove_tile_by_id(&r, 10);
    assert(!rack_contains_tile(&r, &t1));
    assert(rack_count(&r) == 1);

    return 0;
}
