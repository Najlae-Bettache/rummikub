#ifndef DATA_H
#define DATA_H

#define MAX_TILES 106
#define MAX_RACK  14

typedef enum {
    RED, BLUE, BLACK, YELLOW
} Color;

typedef struct {
    int value;      // 1-13
    Color color;
    int is_joker;
} Tile;

typedef struct {
    Tile tiles[MAX_TILES];
    int count;
} Deck;

typedef struct {
    Tile tiles[MAX_RACK];
    int count;
} Rack;

typedef struct {
    Tile tiles[13];
    int count;
} Group;

typedef struct {
    Group groups[50];
    int group_count;
} Board;

/* Fonctions Data */
Deck data_generate_deck(void);
void data_shuffle_deck(Deck *d);
Tile data_draw_tile(Deck *d);
void data_add_tile_to_board(Board *b, Tile t);
void data_remove_tile_from_rack(Rack *r, int index);

#endif
