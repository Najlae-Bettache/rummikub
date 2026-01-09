#include <assert.h>
#include "../include/table.h"

int main(void) {
    Table_t t;
    table_init(&t);

    assert(table_is_empty(&t));

    Combination_t c;
    combination_init(&c);

    Tile_t t1 = tile_create(3, TILE_BLUE, 0, 3);
    combination_add_tile(&c, t1);

    assert(table_add_combination(&t, &c));
    assert(t.count == 1);

    Combination_t *stored = table_get_combination(&t, 0);
    assert(stored->count == 1);

    table_remove_combination(&t, 0);
    assert(table_is_empty(&t));

    return 0;
}
