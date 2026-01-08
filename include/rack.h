#pragma once
#include <stdbool.h>
#include "tile.h"

#define RACK_START 14
#define RACK_MAX   30

typedef struct Rack_s{
    Tile_t tiles[RACK_MAX];
    int count;
} Rack_t;

void rack_init(Rack_t *);
void rack_add_tile(Rack_t *, Tile_t);
int rack_remove_tile_by_id(Rack_t *, int);
void rack_print(const Rack_t *);
int rack_find_tile_index(const Rack_t *, const Tile_t *);
bool rack_contains_tile(const Rack_t *, const Tile_t *);
bool rack_is_empty(const Rack_t *);
Tile_t rack_get_tile(const Rack_t *, int);
int rack_count(const Rack_t *);
