#include "../../include/save.h"
#include <stdio.h>
#include <string.h>

static ScoreGame g_history[MAX_HISTORY];
static int g_history_count = 0;

void score_load_history(void)
{
    FILE *f = fopen("scores.dat", "rb");
    if (!f)
    {
        g_history_count = 0;
        return;
    }

    g_history_count = fread(
        g_history,
        sizeof(ScoreGame),
        MAX_HISTORY,
        f
    );

    fclose(f);
}

void score_save_history(void)
{
    FILE *f = fopen("scores.dat", "wb");
    if (!f) return;

    fwrite(
        g_history,
        sizeof(ScoreGame),
        g_history_count,
        f
    );

    fclose(f);
}

void score_add_game(const Game_t *game, const char **names)
{
    if (g_history_count >= MAX_HISTORY)
        return;

    ScoreGame *sg = &g_history[g_history_count++];

    sg->player_count = game->player_count;

    for (size_t i = 0; i < game->player_count; i++)
    {
        strncpy(sg->players[i].name, names[i], NAME_MAX_LEN-1);
        sg->players[i].name[NAME_MAX_LEN-1] = '\0';
        sg->players[i].score = game->players[i].score;
    }

    score_save_history();
}

int score_get_count(void)
{
    return g_history_count;
}

const ScoreGame *score_get(int index)
{
    if (index < 0 || index >= g_history_count)
        return NULL;

    return &g_history[index];
}
