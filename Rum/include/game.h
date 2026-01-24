#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "data.h"

typedef struct {
    Player players[4];
    int player_count;
    int current_player;
    Deck deck;
    Board board;
} GameState;

void game_start(GameState *g, int player_count);
void game_next_turn(GameState *g);
void game_play_move(GameState *g);
int  game_check_end(GameState *g);
void game_compute_scores(GameState *g);

#endif
