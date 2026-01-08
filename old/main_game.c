#include <stdio.h>
#include "../include/rummikub.h"

// ---------------------------------------------------------
// main_game.c
// ---------------------------------------------------------
// Ce fichier me permet de tester mon module Game
// indépendamment du main de ma collègue.
// Je peux ainsi vérifier que mes fonctions Game
// fonctionnent correctement avant l'intégration finale.
// ---------------------------------------------------------

int main(void) {

    Game g;

    printf("=== Initialisation de la partie ===\n");

    // J'initialise une partie avec 2 joueurs
    game_init(&g, 2);

    // Je distribue les tuiles initiales
    game_deal_initial(&g);

    // J'affiche les racks de départ pour vérifier que
    // la distribution fonctionne correctement
    printf("\n=== Racks initiaux ===\n");
    for (int i = 0; i < g.player_count; i++) {
        printf("%s : ", g.players[i].name);
        rack_print(&g.players[i].rack);
    }

    // Je lance une boucle de jeu simple
    printf("\n=== Début de la partie ===\n");

    while (!game_is_finished(&g)) {

        // Je joue un tour complet
        game_play_turn(&g);

        // Je passe au joueur suivant
        game_next_turn(&g);
    }

    // Quand la partie est terminée, je calcule les scores
    printf("\n=== Fin de la partie ===\n");
    game_compute_scores(&g);

    // J'affiche les scores finaux
    printf("\n=== Scores finaux ===\n");
    for (int i = 0; i < g.player_count; i++) {
        printf("%s : %d points\n", g.players[i].name, g.players[i].score);
    }

    return 0;
}