#ifndef RULES_H
#define RULES_H
#include "dats.h"

int rules_is_joker(Tile t);
int rules_validate_group(Group *g);
int rules_validate_board(Board *b);
int rules_first_move_valid(Rack *r);

#endif