#pragma once
#include <stddef.h>
#include <stdbool.h>
#include "tile.h"

#define COMBINATION_MAX_TILES 32

typedef struct Combinaison_s {
    Tile_t tiles[COMBINATION_MAX_TILES];
    size_t count;
} Combination_t;

void combination_init(Combination_t* combo);
bool combination_add_tile(Combination_t* combo, Tile_t tile);
bool combination_remove_tile_at(Combination_t* combo, size_t index);
Tile_t* combination_get_tile(Combination_t* combo, size_t index);
const Tile_t* combination_get_tile_const(const Combination_t* combo, size_t index);
bool combination_is_empty(const Combination_t* combo);
Combination_t combination_clone(const Combination_t* combo);
void combination_clear(Combination_t* combo);