#include "../../include/ai.h"
#include <string.h>
#include <stdlib.h>

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

int tile_compare_value(const void *a, const void *b)
{
    const Tile_t *t1 = a;
    const Tile_t *t2 = b;
    return t1->value - t2->value;
}

int ai_evaluate_combo(const Combination_t *combo)
{
    int score = 0;

    for (size_t i = 0; i < combo->count; i++)
        score += combo->tiles[i].value;
    score += (int)combo->count * 3;
    return score;
}

void ai_find_series(
    const Rack_t *rack,
    AIChoice *best,
    const GamePlayer_t *player
)
{
    for (int value = 1; value <= 13; value++) {

        bool used_colors[4] = {false};
        Combination_t combo;
        combination_init(&combo);

        for (int i = 0; i < rack->count; i++) {
            Tile_t t = rack->tiles[i];

            if (!t.is_joker &&
                t.value == value &&
                !used_colors[t.color])
            {
                used_colors[t.color] = true;
                combination_add_tile(&combo, t);
            }
        }

        if (combo.count >= 3) {
            RulesResult r = rules_validate_combination(&combo);
            if (!r.valid)
                continue;

            if (!player->has_opened &&
                r.score < GAME_OPENING_SCORE)
                continue;

            int ai_score = ai_evaluate_combo(&combo);

            if (ai_score > best->ai_score) {
                best->combo = combo;
                best->ai_score = ai_score;
            }
        }
    }
}

void ai_find_runs(
    const Rack_t *rack,
    AIChoice *best,
    const GamePlayer_t *player
)
{
    Tile_t tiles[32];
    for (int color = 0; color < 4; color++) {
        int count = 0;
        for (int i = 0; i < rack->count; i++) {
            if (!rack->tiles[i].is_joker &&
                (int)rack->tiles[i].color == color)
            {
                tiles[count++] = rack->tiles[i];
            }
        }
        if (count < 3)
            continue;
        qsort(tiles, count, sizeof(Tile_t), tile_compare_value);
        for (int i = 0; i < count; i++) {
            Combination_t combo;
            combination_init(&combo);
            combination_add_tile(&combo, tiles[i]);
            int last = tiles[i].value;
            for (int j = i + 1; j < count; j++) {
                if (tiles[j].value == last + 1) {
                    combination_add_tile(&combo, tiles[j]);
                    last = tiles[j].value;
                    if (combo.count >= 3) {
                        RulesResult r =
                            rules_validate_combination(&combo);
                        if (!r.valid)
                            continue;
                        if (!player->has_opened &&
                            r.score < GAME_OPENING_SCORE)
                            continue;
                        int ai_score =
                            ai_evaluate_combo(&combo);
                        if (ai_score > best->ai_score) {
                            best->combo = combo;
                            best->ai_score = ai_score;
                        }
                    }
                }
                else if (tiles[j].value > last + 1) {
                    break;
                }
            }
        }
    }
}

bool ai_play_simple(AIPlayer *ai, Game_t *game)
{
    (void)ai;
    GamePlayer_t *player =
        &game->players[game->current_player];
    AIChoice best;
    memset(&best, 0, sizeof(best));
    best.ai_score = -1;
    ai_find_series(&player->rack, &best, player);
    ai_find_runs(&player->rack, &best, player);
    if (best.ai_score >= 0) {
        game_action_play_combination(game, &best.combo);
        return true;
    }
    game_action_draw(game);
    return false;
}
