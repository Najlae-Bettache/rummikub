#include <stdlib.h>
#include <string.h>
#include "rules.h"

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

bool rules_pre_validation(const Combination_t *combo)
{
    if (!combo || combo->count < 3)
        return false;
    int joker_count = 0;
    for (size_t i = 0; i < combo->count; i++) {
        if (combo->tiles[i].is_joker)
            joker_count++;
        for (size_t j = i + 1; j < combo->count; j++) {
            if (combo->tiles[i].id == combo->tiles[j].id)
                return false;
        }
    }
    if (joker_count > 2)
        return false;
    return true;
}

int rules_compute_score(const Combination_t *combo)
{
    int score = 0;
    for (size_t i = 0; i < combo->count; i++) {
        if (combo->tiles[i].is_joker) {
            score += 10;
        } else {
            score += combo->tiles[i].value;
        }
    }
    return score;
}


RulesResult rules_validate_series(const Combination_t *combo)
{
    RulesResult res = { false, RULES_COMBINATION_INVALID, 0 };
    if (!rules_pre_validation(combo))
        return res;
    int value = -1;
    bool used_colors[4] = { false };
    for (size_t i = 0; i < combo->count; i++) {
        const Tile_t *t = &combo->tiles[i];
        if (t->is_joker)
            continue;
        if (value == -1)
            value = t->value;
        else if (t->value != value)
            return res;
        if (used_colors[t->color])
            return res;
        used_colors[t->color] = true;
    }
    res.valid = true;
    res.type = RULES_COMBINATION_SERIES;
    res.score = rules_compute_score(combo);
    return res;
}

RulesResult rules_validate_suite(const Combination_t *combo)
{
    RulesResult res = { false, RULES_COMBINATION_INVALID, 0 };
    if (!rules_pre_validation(combo))
        return res;
    TileColor color = -1;
    int values[COMBINATION_MAX_TILES];
    int count = 0;
    int joker_count = 0;
    int gaps = 0;
    for (size_t i = 0; i < combo->count; i++) {
        const Tile_t *t = &combo->tiles[i];
        if (t->is_joker) {
            joker_count++;
            continue;
        }
        if ((int)color == -1)
            color = t->color;
        else if (t->color != color)
            return res;
        values[count] = t->value;
        count++;
    }
    qsort(values, count, sizeof(int), compare);
    for (int i = 0; i < count - 1; i++) {
        int diff = values[i + 1] - values[i];
        if (diff == 0)
            return res;
        if (diff > 1)
            gaps += diff - 1;
    }
    if (gaps > joker_count)
        return res;
    res.valid = true;
    res.type = RULES_COMBINATION_RUN;
    res.score = rules_compute_score(combo);
    return res;
}

RulesResult rules_validate_combination(const Combination_t *combo)
{
    RulesResult res;
    res = rules_validate_series(combo);
    if (res.valid)
        return res;
    res = rules_validate_suite(combo);
    return res;
}
