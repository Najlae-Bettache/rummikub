#ifndef SAVE_H
#define SAVE_H

#include "game.h"

void save_scores(const char *path, GameState *g);
void load_scores(const char *path);

#endif
