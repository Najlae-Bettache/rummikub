#include <assert.h>
#include "../include/deck.h"

int main(void) {
    Deck_t d;
    deck_create_full(&d);

    assert(d.count == MAX_DECK);

    deck_shuffle(&d);

    for (int i = 0; i < MAX_DECK; i++) {
        Tile_t t = deck_draw(&d);
        assert(t.id >= 0);
    }

    assert(deck_is_empty(&d));
    return 0;
}
