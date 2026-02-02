#include "../../include/ui.h"
#include <stdio.h>
#include <string.h>

extern ScoreEntry g_scores[MAX_SCORES];
extern int g_score_count;

void load_scores(void)
{
    FILE *f = fopen("scores.dat", "rb");
    if (!f) {
        g_score_count = 0;
        return;
    }
    g_score_count = fread(
        g_scores,
        sizeof(ScoreEntry),
        MAX_SCORES,
        f
    );
    fclose(f);
}

void save_scores(void)
{
    FILE *f = fopen("scores.dat", "wb");
    if (!f)
        return;
    fwrite(
        g_scores,
        sizeof(ScoreEntry),
        g_score_count,
        f
    );
    fclose(f);
}

void add_score(const char *name, int score)
{
    for (int i = 0; i < g_score_count; i++) {
        if (strcmp(g_scores[i].name, name) == 0) {
            g_scores[i].total_score += score;
            save_scores();
            return;
        }
    }
    if (g_score_count < MAX_SCORES) {
        strncpy(
            g_scores[g_score_count].name,
            name,
            NAME_MAX_LEN - 1
        );
        g_scores[g_score_count].name[NAME_MAX_LEN - 1] = '\0';
        g_scores[g_score_count].total_score = score;
        g_score_count++;
        save_scores();
    }
}
