#include "../../include/rack.h"
#include <stdio.h>

void rack_init(Rack_t *rack)
{
    rack->count = 0;
}

void rack_add_tile(Rack_t *rack, Tile_t tile)
{
    if (rack_contains_tile(rack, &tile)) {
        fprintf(stderr, "Warning: Tile already in rack, not adding duplicate.\n");
        return;
    }
    if (rack->count < RACK_MAX) {
        rack->tiles[rack->count++] = tile;
    } else {
        fprintf(stderr, "Error: Rack is full, cannot add more tiles.\n");
    }
}

int rack_remove_tile_by_id(Rack_t *rack, int id)
{
    for (int i = 0; i < rack->count; i++) {
        if (rack->tiles[i].id == id) {
            for (int j = i; j < rack->count - 1; j++) {
                rack->tiles[j] = rack->tiles[j + 1];
            }
            rack->count--;
            return 1;
        }
    }
    return 0;
}

void rack_print(const Rack_t *rack)
{
    printf("Rack (Count: %d): ", rack->count);
    for (int i = 0; i < rack->count; i++) {
        tile_print(&rack->tiles[i]);
        if (i < rack->count - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

int rack_find_tile_index(const Rack_t *rack, const Tile_t *tile)
{
    for (int i = 0; i < rack->count; i++) {
        if (tile_equals(&rack->tiles[i], tile)) {
            return i;
        }
    }
    return -1;
}

bool rack_contains_tile(const Rack_t *rack, const Tile_t *tile)
{
    return rack_find_tile_index(rack, tile) != -1;
}

bool rack_is_empty(const Rack_t *rack)
{
    return rack->count == 0;
}

Tile_t rack_get_tile(const Rack_t *rack, int index)
{
    if (index >= 0 && index < rack->count) {
        return rack->tiles[index];
    } else {
        fprintf(stderr, "Error: Index out of bounds in rack_get_tile.\n");
        Tile_t empty_tile = {0};
        return empty_tile;
    }
}

int rack_count(const Rack_t *rack)
{
    return rack->count;
}
