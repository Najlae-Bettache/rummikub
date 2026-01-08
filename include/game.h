#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "deck.h"
#include "rack.h"
#include "table.h"
#include "combination.h"
#include "rules.h"

#define GAME_MAX_PLAYERS 4
#define GAME_MIN_PLAYERS 2
#define GAME_OPENING_SCORE 30

typedef enum {
    GAME_STATE_INIT,
    GAME_STATE_TURN,
    GAME_STATE_END
} GameState_t;

typedef enum {
    GAME_ACTION_DRAW,
    GAME_ACTION_PLAY_COMBINATION,
    GAME_ACTION_PASS
} GameActionType_t;

typedef struct {
    GameActionType_t type;
    Combination_t combination;
} GameAction_t;

typedef struct GamePlayer_t {
    int id;
    Rack_t rack;
    int score;
    bool has_opened;
    bool is_ai;
} GamePlayer_t;

typedef struct Game_s{
    GameState_t state;
    Deck_t deck;
    Table_t table;
    GamePlayer_t players[GAME_MAX_PLAYERS];
    size_t player_count;
    size_t current_player;
} Game_t;

bool game_init(Game_t *game, size_t nb_players);
void game_reset(Game_t *game);
GameState_t game_get_state(const Game_t *game);
size_t game_get_current_player(const Game_t *game);
GamePlayer_t *game_get_player(Game_t *game, size_t player_id);
const GamePlayer_t *game_get_player_const(const Game_t *game, size_t player_id);
void game_next_turn(Game_t *game);
bool game_play_action(Game_t *game, const GameAction_t *action);
bool game_action_draw(Game_t *game);
bool game_action_play_combination(Game_t *game, const Combination_t *combo);
void game_action_pass(Game_t *game);
bool game_is_finished(const Game_t *game);
void game_compute_final_scores(Game_t *game);