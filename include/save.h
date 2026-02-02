#pragma once

#include "ui.h"

#define MAX_HISTORY 128
#define NAME_MAX_LEN 32

typedef struct {
    char name[NAME_MAX_LEN];
    int score;
} ScorePlayer;

typedef struct {
    int player_count;
    ScorePlayer players[MAX_PLAYERS];
} ScoreGame;

void score_load_history(void);
void score_save_history(void);
void score_add_game(const Game_t *game, const char **names);
int  score_get_count(void);
const ScoreGame *score_get(int index);
