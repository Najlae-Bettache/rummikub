#include "../include/ai.h"

void ai_init(AIPlayer *ai, size_t player_id)
{
    ai->player_id = player_id;
}

bool ai_find_valid_combination(const Game_t *game, const GamePlayer_t *player, Combination_t *out_combo)
{
    if (!game || !player || !out_combo)
        return false;
    for (int value = 1; value <= 13; value++) {
        combination_clear(out_combo);
        bool used_colors[4] = { false };
        for (int i = 0; i < player->rack.count; i++) {
            Tile_t t = player->rack.tiles[i];
            if (t.is_joker)
                continue;
            if (t.value == value && !used_colors[t.color]) {
                combination_add_tile(out_combo, t);
                used_colors[t.color] = true;
            }
        }
        if (out_combo->count >= 3) {
            RulesResult r = rules_validate_combination(out_combo);
            if (r.valid) {
                if (player->has_opened || r.score >= GAME_OPENING_SCORE)
                    return true;
            }
        }
    }
    for (int color = 0; color < 4; color++) {
        Tile_t temp[COMBINATION_MAX_TILES];
        int count = 0;
        for (int i = 0; i < player->rack.count; i++) {
            Tile_t t = player->rack.tiles[i];
            if (!t.is_joker && (int)t.color == color)
                temp[count++] = t;
        }
        if (count < 3)
            continue;
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (temp[j].value < temp[i].value) {
                    Tile_t tmp = temp[i];
                    temp[i] = temp[j];
                    temp[j] = tmp;
                }
            }
        }
        for (int i = 0; i <= count - 3; i++) {
            combination_clear(out_combo);
            combination_add_tile(out_combo, temp[i]);
            int last_value = temp[i].value;
            for (int j = i + 1; j < count; j++) {
                if (temp[j].value == last_value + 1) {
                    combination_add_tile(out_combo, temp[j]);
                    last_value++;
                }
                if (out_combo->count >= 3) {
                    RulesResult r = rules_validate_combination(out_combo);
                    if (r.valid) {
                        if (player->has_opened || r.score >= GAME_OPENING_SCORE)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

bool ai_play_simple(AIPlayer *ai, Game_t *game)
{
    if (!ai || !game)
        return false;
    if (game->state != GAME_STATE_TURN)
        return false;
    if (game->current_player != ai->player_id)
        return false;
    GamePlayer_t *player = &game->players[ai->player_id];
    Combination_t combo;
    combination_init(&combo);
    if (ai_find_valid_combination(game, player, &combo)) {
        return game_action_play_combination(game, &combo);
    }
    if (game_action_draw(game))
        return true;
    game_action_pass(game);
    return false;
}
