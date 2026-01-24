
#include    "rules.h"


int rules_is_joker(Tile t) {
    return t.is_joker;
}

int rules_validate_group(Group *g) {
    // validation série / suite
    return 1;
}

int rules_validate_board(Board *b) {
    for (int i = 0; i < b->group_count; i++) {
        if (!rules_validate_group(&b->groups[i]))
            return 0;
    }
    return 1;
}

int rules_first_move_valid(Rack *r) {
    int sum = 0;
    for (int i = 0; i < r->count; i++)
        if (!rules_is_joker(r->tiles[i]))
            sum += r->tiles[i].value;
    return sum >= 30;
}
 
