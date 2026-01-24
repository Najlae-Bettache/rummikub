#include "save.h"
#include <stdio.h>

void save_scores(const char *path, GameState *g) {
    FILE *f = fopen(path, "w");
    if (!f) return;
    fclose(f);
}

void load_scores(const char *path) {
    // lecture fichier
}
