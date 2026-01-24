#ifndef PLAYER_H
#define PLAYER_H

#include "data.h"

#define HUMAN 0
#define AI    1

typedef struct {
    char name[32];
    int type;
    Rack rack;
    int score;
} Player;

void player_init(Player *p, const char *name, int type);
void ai_play_random(void);

#endif
