#include <stdlib.h>
#include <time.h>
#include "../include/rummikub.h"

static void swap_tiles(Tile *a, Tile *b) {
    Tile tmp = *a;
    *a = *b;
    *b = tmp;
}

/* DECK  */

void deck_generate(Deck *d) {
    d->count = 0;

    // 2 exemplaires de chaque tuile (4 couleurs x 1..13)
    for (int copy = 0; copy < 2; copy++) {
        for (int c = 0; c < 4; c++) {
            for (int v = 1; v <= 13; v++) {
                Tile t;
                t.value = v;
                t.color = (Color)c;
                t.is_joker = 0;
                d->tiles[d->count++] = t;
            }
        }
    }

    // 2 jokers
    for (int j = 0; j < 2; j++) {
        Tile joker;
        joker.value = 0;
        joker.color = ROUGE; // valeur arbitraire, ignorée si joker
        joker.is_joker = 1;
        d->tiles[d->count++] = joker;
    }
}

void deck_shuffle(Deck *d) {
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }

    // Fisher–Yates
    for (int i = d->count - 1; i > 0; i--) {
        int k = rand() % (i + 1);
        swap_tiles(&d->tiles[i], &d->tiles[k]);
    }
}

int deck_draw(Deck *d, Tile *out) {
    if (d->count <= 0) return 0;
    *out = d->tiles[--d->count];
    return 1;
}

/*  RACK  */

int rack_add(Rack *r, Tile t) {
    if (r->count >= RACK_MAX) return 0;
    r->tiles[r->count++] = t;
    return 1;
}

int rack_remove_at(Rack *r, int index, Tile *out) {
    if (index < 0 || index >= r->count) return 0;
    if (out) *out = r->tiles[index];

    for (int i = index; i < r->count - 1; i++) {
        r->tiles[i] = r->tiles[i + 1];
    }
    r->count--;
    return 1;
}

/* BOARD  */

int board_add_group(Board *b, const Tile *tiles, int n) {
    if (n < 3 || n > GROUP_MAX_TILES) return 0;
    if (b->count >= BOARD_MAX_GROUPS) return 0;

    Group *g = &b->groups[b->count++];
    g->count = n;
    for (int i = 0; i < n; i++) g->tiles[i] = tiles[i];
    return 1;
}
