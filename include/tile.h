#pragma once
#include <stdbool.h>

typedef enum {
    TILE_RED,
    TILE_BLUE,
    TILE_YELLOW,
    TILE_BLACK
} TileColor;

typedef struct Tile_s{
    TileColor color;
    int value;
    bool is_joker;
    int id;
} Tile_t;

Tile_t tile_create(int, TileColor, bool, int);
void tile_print(const Tile_t *);
bool tile_equals(const Tile_t *, const Tile_t *);
bool tile_is_joker(const Tile_t *);