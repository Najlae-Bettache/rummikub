#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "game.h"
#include "combination.h"

typedef struct {
    size_t player_id;
} AIPlayer;

void ai_init(AIPlayer *, size_t);

bool ai_find_valid_combination(
    const Game_t *,
    const GamePlayer_t *,
    Combination_t *
);

bool ai_play_simple(AIPlayer *, Game_t *);
