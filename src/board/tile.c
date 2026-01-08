#include "../../include/tile.h"
#include <stdio.h>

Tile_t tile_create(int value, TileColor color, bool is_joker, int id)
{
    Tile_t tile;

    tile.value = value;
    tile.color = color;
    tile.is_joker = is_joker;
    tile.id = id;
    return tile;
}

void tile_print(const Tile_t *tile)
{
    if (tile->is_joker) {
        printf("Joker (ID: %d)", tile->id);
    } else {
        const char* color_str;
        switch (tile->color) {
            case TILE_RED: color_str = "Red"; break;
            case TILE_BLUE: color_str = "Blue"; break;
            case TILE_YELLOW: color_str = "Yellow"; break;
            case TILE_BLACK: color_str = "Black"; break;
            default: color_str = "Unknown"; break;
        }
        printf("Tile(ID: %d, Color: %s, Value: %d)", tile->id, color_str, tile->value);
    }
}

bool tile_equals(const Tile_t *t1, const Tile_t *t2) {
    return (t1->id == t2->id);
}

bool tile_is_joker(const Tile_t *tile) {
    return tile->is_joker;
}
