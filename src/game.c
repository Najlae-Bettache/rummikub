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

// ---------------------------------------------------------
// game_next_turn
// ---------------------------------------------------------
// Cette fonction fait avancer le tour au joueur suivant.
// Exemple : 0 → 1 → 2 → 3 → 0 → 1 → ...
// On utilise l'opérateur modulo (%) pour revenir à 0
// lorsque l'on dépasse le dernier joueur.
//
// Paramètre :
//   g = pointeur vers la structure Game contenant l'état du jeu
// ---------------------------------------------------------
void game_next_turn(Game *g) {

    // On passe au joueur suivant en incrémentant current_player
    g->current_player++;

    // Si on dépasse le nombre total de joueurs,
    // le modulo ramène automatiquement à 0.
    // Exemple : (4 % 4) = 0
    g->current_player %= g->player_count;

    // À ce stade, current_player pointe sur le bon joueur
}
// ---------------------------------------------------------
// game_is_finished
// ---------------------------------------------------------
// Cette fonction vérifie si la partie est terminée.
// La partie se termine lorsqu'un joueur n'a plus de tuiles
// dans son chevalet (rack).
//
// Retourne :
//   1 → si un joueur a gagné (rack vide)
//   0 → sinon
// ---------------------------------------------------------
int game_is_finished(Game *g) {

    // On parcourt tous les joueurs
    for (int i = 0; i < g->player_count; i++) {

        // Si un joueur n'a plus de tuiles, il a gagné
        if (g->players[i].rack.count == 0) {
            return 1;  // partie terminée
        }
    }

    // Aucun joueur n'a encore gagné
    return 0;
}

// ---------------------------------------------------------
// game_play_turn
// ---------------------------------------------------------
// Version avancée :
// - Affiche le joueur courant
// - Affiche son chevalet
// - Permet de poser une tuile sur le plateau
// - Permet de piocher une tuile
// ---------------------------------------------------------
void game_play_turn(Game *g) {
    Player *p = &g->players[g->current_player];

    printf("\n--- Tour de %s ---\n", p->name);

    // 1. Afficher le chevalet du joueur
    printf("Votre chevalet : ");
    rack_print(&p->rack);

    // 2. Choix de l'action
    printf("Actions disponibles :\n");
    printf("1. Poser une tuile\n");
    printf("2. Piocher une tuile\n");
    printf("Choix : ");

    int choix;
    scanf("%d", &choix);

    // -----------------------------------------------------
    // ACTION 1 : Poser une tuile
    // -----------------------------------------------------
    if (choix == 1) {
        int index;
        printf("Index de la tuile à poser : ");
        scanf("%d", &index);

        Tile t;

        // Retirer la tuile du rack
        if (!rack_remove_at(&p->rack, index, &t)) {
            printf("Index invalide.\n");
            return;
        }

        // Créer un groupe de 1 tuile
        Tile tiles[1] = { t };

        // Ajouter ce groupe au plateau
        if (!board_add_group(&g->board, tiles, 1)) {
            printf("Erreur : impossible d'ajouter le groupe.\n");
            return;
        }

        printf("Tuile posée sur le plateau.\n");
    }

    // -----------------------------------------------------
    // ACTION 2 : Piocher une tuile
    // -----------------------------------------------------
    else if (choix == 2) {
        Tile t;

        if (!deck_draw(&g->deck, &t)) {
            printf("La pioche est vide.\n");
            return;
        }

        rack_add(&p->rack, t);
        printf("Tuile piochée.\n");
    }

    else {
        printf("Choix invalide.\n");
    }
}

// ---------------------------------------------------------
// game_compute_scores
// ---------------------------------------------------------
// Calcule le score final de chaque joueur.
// Le score = somme des valeurs des tuiles restantes dans le rack.
// Si un joueur n'a plus de tuiles, son score est 0.
// ---------------------------------------------------------
void game_compute_scores(Game *g) {

    printf("\n=== Calcul des scores ===\n");

    // Parcourir tous les joueurs
    for (int p = 0; p < g->player_count; p++) {

        int score = 0;

        // Parcourir les tuiles du chevalet
        for (int i = 0; i < g->players[p].rack.count; i++) {
            Tile t = g->players[p].rack.tiles[i];

            // Les jokers ont une valeur spéciale (souvent 30)
            // mais pour l'instant on peut mettre 0 ou 15 selon vos règles
            if (t.is_joker)
                score += 0;
            else
                score += t.value;
        }

        // Stocker le score dans la structure Player
        g->players[p].score = score;

        // Affichage pour debug
        printf("%s : %d points\n", g->players[p].name, score);
    }
}