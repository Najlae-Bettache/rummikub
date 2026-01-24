#include "game.h"

void game_start(GameState *g, int player_count) {
    g->player_count = player_count;
    g->current_player = 0;
}

void game_next_turn(GameState *g) {
    g->current_player = (g->current_player + 1) % g->player_count;
}

void game_play_move(GameState *g) {
    // action du joueur
}

int game_check_end(GameState *g) {
    return 0;
}

void game_compute_scores(GameState *g) {
    // calcul des scores
}
