#ifndef RUMMIKUB_H
#define RUMMIKUB_H

#include <stdio.h>

/* Tailles */
#define MAX_DECK 106
#define MAX_PLAYERS 4

#define RACK_START 14   // distribution initiale
#define RACK_MAX   30   // un joueur peut piocher donc > 14

#define GROUP_MAX_TILES 20
#define BOARD_MAX_GROUPS 30

/* Types */
typedef enum { ROUGE=0, BLEU=1, JAUNE=2, NOIR=3 } Color;

typedef struct {
    int value;      // 1..13, 0 si joker
    Color color;    // ignoré si joker
    int is_joker;   // 1 si joker
} Tile;

typedef struct {
    Tile tiles[MAX_DECK];
    int count;      // nb tuiles restantes
} Deck;

typedef struct {
    Tile tiles[RACK_MAX];
    int count;
} Rack;

typedef struct {
    Tile tiles[GROUP_MAX_TILES];
    int count;
} Group;

typedef struct {
    Group groups[BOARD_MAX_GROUPS];
    int count;
} Board;

typedef struct {
    char name[32];
    int score;
    int is_ai;
    Rack rack;
} Player;

typedef struct {
    Deck deck;
    Board board;
    Player players[MAX_PLAYERS];
    int player_count;
    int current_player;
} Game;

/* Deck */
void deck_generate(Deck *d);
void deck_shuffle(Deck *d);
int  deck_draw(Deck *d, Tile *out);

/* Rack */
int rack_add(Rack *r, Tile t);
int rack_remove_at(Rack *r, int index, Tile *out);

/* Board */
int board_add_group(Board *b, const Tile *tiles, int n);

/* Game */
// ===== Module Game =====
void game_init(Game *g, int player_count);
void game_deal_initial(Game *g);
void game_init(Game *g, int player_count);
void game_deal_initial(Game *g);
void game_next_turn(Game *g);
int  game_is_finished(Game *g);
void game_play_turn(Game *g);
void game_compute_scores(Game *g);

/* Debug */
void tile_print(Tile t);
void rack_print(const Rack *r);

#endif
