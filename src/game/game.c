#include "../include/game.h"

bool game_init(Game_t *game, size_t nb_players)
{
    if (!game)
        return false;
    if (nb_players < GAME_MIN_PLAYERS || nb_players > GAME_MAX_PLAYERS)
        return false;
    game->state = GAME_STATE_TURN;
    game->player_count = nb_players;
    game->current_player = 0;
    deck_create_full(&game->deck);
    deck_shuffle(&game->deck);
    table_init(&game->table);
    for (size_t i = 0; i < nb_players; i++) {
        rack_init(&game->players[i].rack);
        game->players[i].id = i;
        game->players[i].score = 0;
        game->players[i].has_opened = false;
        game->players[i].is_ai = false;
        for (int j = 0; j < RACK_START; j++) {
            rack_add_tile(&game->players[i].rack, deck_draw(&game->deck));
        }
    }
    return true;
}

void game_reset(Game_t *game)
{
    if (!game)
        return;
    game->state = GAME_STATE_TURN;
    game->current_player = 0;
    deck_create_full(&game->deck);
    deck_shuffle(&game->deck);
    table_init(&game->table);
    for (size_t i = 0; i < game->player_count; i++) {
        rack_init(&game->players[i].rack);
        game->players[i].score = 0;
        game->players[i].has_opened = false;
        for (int j = 0; j < RACK_START; j++) {
            rack_add_tile(&game->players[i].rack, deck_draw(&game->deck));
        }
    }
}

GameState_t game_get_state(const Game_t *game)
{
    return game->state;
}

size_t game_get_current_player(const Game_t *game)
{
    return game->current_player;
}

GamePlayer_t *game_get_player(Game_t *game, size_t player_id)
{
    if (player_id < game->player_count) {
        return &game->players[player_id];
    }
    return NULL;
}

const GamePlayer_t *game_get_player_const(const Game_t *game, size_t player_id)
{
    if (player_id < game->player_count) {
        return &game->players[player_id];
    }
    return NULL;
}

void game_next_turn(Game_t *game)
{
    if (!game || game->state != GAME_STATE_TURN)
        return;
    game->current_player = (game->current_player + 1) % game->player_count;
}

bool game_play_action(Game_t *game, const GameAction_t *action)
{
    if (!game || !action)
        return false;
    if (game->state != GAME_STATE_TURN)
        return false;
    switch (action->type) {
        case GAME_ACTION_DRAW:
            return game_action_draw(game);
        case GAME_ACTION_PLAY_COMBINATION:
            return game_action_play_combination(game, &action->combination);
        case GAME_ACTION_PASS:
            game_action_pass(game);
            return true;
        default:
            return false;
    }
}

bool game_action_draw(Game_t *game)
{
    if (!game || game->state != GAME_STATE_TURN)
        return false;
    GamePlayer_t *player = &game->players[game->current_player];
    if (player->rack.count >= RACK_MAX)
        return false;
    if (deck_is_empty(&game->deck)) {
        game->state = GAME_STATE_END;
        return false;
    }
    rack_add_tile(&player->rack, deck_draw(&game->deck));
    game_next_turn(game);
    return true;
}


bool game_action_play_combination(Game_t *game, const Combination_t *combo)
{
    if (!game || !combo || game->state != GAME_STATE_TURN)
        return false;
    GamePlayer_t *player = &game->players[game->current_player];
    RulesResult result = rules_validate_combination(combo);
    if (!result.valid)
        return false;
    if (!player->has_opened && result.score < GAME_OPENING_SCORE)
        return false;
    for (size_t i = 0; i < combo->count; i++) {
        if (!rack_contains_tile(&player->rack, &combo->tiles[i]))
            return false;
    }
    for (size_t i = 0; i < combo->count; i++) {
        rack_remove_tile_by_id(&player->rack, combo->tiles[i].id);
    }
    table_add_combination(&game->table, combo);
    player->score += result.score;
    player->has_opened = true;
    if (rack_is_empty(&player->rack)) {
        game->state = GAME_STATE_END;
        return true;
    }
    game_next_turn(game);
    return true;
}


void game_action_pass(Game_t *game)
{
    game_next_turn(game);
}

bool game_is_finished(const Game_t *game)
{
    return game && game->state == GAME_STATE_END;
}

void game_compute_final_scores(Game_t *game)
{
    if (!game)
        return;
    int winner = -1;
    for (size_t i = 0; i < game->player_count; i++) {
        if (rack_is_empty(&game->players[i].rack)) {
            winner = i;
            break;
        }
    }
    if (winner == -1)
        return;
    int gain = 0;
    for (size_t i = 0; i < game->player_count; i++) {
        if (i == (size_t)winner)
            continue;
        int penalty = 0;
        for (int j = 0; j < game->players[i].rack.count; j++) {
            Tile_t t = game->players[i].rack.tiles[j];
            penalty += t.is_joker ? 30 : t.value;
        }
        game->players[i].score -= penalty;
        gain += penalty;
    }
    game->players[winner].score += gain;
}
