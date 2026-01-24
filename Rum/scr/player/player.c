#include "player.h"
#include <string.h>

void player_init(Player *p, const char *name, int type) {
    strcpy(p->name, name);
    p->type = type;
    p->score = 0;
    p->rack.count = 0;
}

void ai_play_random(void) {
    // stratégie simple IA
}
