#include <stdio.h>
#include "../include/rummikub.h"

int main(void) {
    Game g;

    game_init(&g, 2);       // prototype: 2 joueurs
    game_deal_initial(&g);  // distribue 14 tuiles chacun

    printf("=== Prototype Rummikub (initialisation) ===\n");
    printf("Pioche restante: %d\n\n", g.deck.count);

    for (int p = 0; p < g.player_count; p++) {
        printf("%s: ", g.players[p].name);
        rack_print(&g.players[p].rack);
    }

    return 0;
}
