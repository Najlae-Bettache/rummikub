#include "../../include/combination.h"

void combination_init(Combination_t* combo) {
    combo->count = 0;
}

bool combination_add_tile(Combination_t* combo, Tile_t tile) {
    if (combo->count >= COMBINATION_MAX_TILES) {
        return false;
    }
    combo->tiles[combo->count++] = tile;
    return true;
}

bool combination_remove_tile_at(Combination_t* combo, size_t index) {
    if (index >= combo->count) {
        return false;
    }
    for (size_t i = index; i < combo->count - 1; i++) {
        combo->tiles[i] = combo->tiles[i + 1];
    }
    combo->count--;
    return true;
}

Tile_t* combination_get_tile(Combination_t* combo, size_t index) {
    if (index >= combo->count) {
        return NULL;
    }
    return &combo->tiles[index];
}

const Tile_t* combination_get_tile_const(const Combination_t* combo, size_t index) {
    if (index >= combo->count) {
        return NULL;
    }
    return &combo->tiles[index];
}

bool combination_is_empty(const Combination_t* combo) {
    return combo->count == 0;
}

Combination_t combination_clone(const Combination_t* combo) {
    Combination_t new_combo;
    combination_init(&new_combo);
    for (size_t i = 0; i < combo->count; i++) {
        combination_add_tile(&new_combo, combo->tiles[i]);
    }
    return new_combo;
}

void combination_clear(Combination_t* combo) {
    combo->count = 0;
}
