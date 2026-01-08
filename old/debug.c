#include <stdio.h>
#include "../include/rummikub.h"

static const char *color_str(Color c) {
    switch (c) {
        case ROUGE: return "R";
        case BLEU:  return "B";
        case JAUNE: return "J";
        case NOIR:  return "N";
        default:    return "?";
    }
}

void tile_print(Tile t) {
    if (t.is_joker) {
        printf("[JOKER]");
    } else {
        printf("[%s%02d]", color_str(t.color), t.value);
    }
}

void rack_print(const Rack *r) {
    for (int i = 0; i < r->count; i++) {
        tile_print(r->tiles[i]);
        printf(" ");
    }
    printf("\n");
}
