#include <stdio.h>
#include "../include/rummikub.h"

void game_init(Game *g, int player_count) {
    g->player_count = player_count;
    g->current_player = 0;

    // Initialise la table (board)
    g->board.count = 0;

    // Crée et mélange la pioche
    deck_generate(&g->deck);
    deck_shuffle(&g->deck);

    // Initialise les joueurs
    for (int i = 0; i < player_count; i++) {
        snprintf(g->players[i].name, sizeof(g->players[i].name), "Joueur%d", i + 1);
        g->players[i].score = 0;
        g->players[i].is_ai = 0;
        g->players[i].rack.count = 0;
    }
}

void game_deal_initial(Game *g) {
    for (int p = 0; p < g->player_count; p++) {
        g->players[p].rack.count = 0;

        // Distribution initiale : 14 tuiles
        for (int i = 0; i < RACK_START; i++) {
            Tile t;
            if (!deck_draw(&g->deck, &t)) return; // pioche vide
            rack_add(&g->players[p].rack, t);
        }
    }
}
