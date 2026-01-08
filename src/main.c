#include <stdio.h>
#include "game.h"

static const char *color_to_string(TileColor color)
{
    switch (color) {
        case TILE_RED:    return "RED";
        case TILE_BLUE:   return "BLUE";
        case TILE_YELLOW: return "YELLOW";
        case TILE_BLACK:  return "BLACK";
        default:          return "UNKNOWN";
    }
}

static void print_rack(const Rack_t *rack)
{
    printf("Rack (%d tuiles):\n", rack->count);
    for (int i = 0; i < rack->count; i++) {
        const Tile_t *t = &rack->tiles[i];
        if (t->is_joker) {
            printf("\t[ID:%d] JOKER\n", t->id);
        } else {
            printf("\t[ID:%d] %2d [%s]\n", t->id, t->value, color_to_string(t->color));
        }
    }
}


static void print_table(const Table_t *table)
{
    printf("\nTable (%zu combinaisons):\n", table->count);
    for (size_t i = 0; i < table->count; i++) {
        const Combination_t *c = &table->combinations[i];
        printf("\tCombinaison %zu: ", i);
        for (size_t j = 0; j < c->count; j++) {
            const Tile_t *t = &c->tiles[j];
            if (t->is_joker)
                printf("JOKER ");
            else
                printf("%d[%s] ", t->value, color_to_string(t->color));
        }
        printf("\n");
    }
}


int main(void)
{
    Game_t game;

    if (!game_init(&game, 2)) {
        printf("Erreur init jeu\n");
        return 1;
    }

    while (!game_is_finished(&game)) {
        GamePlayer_t *player = &game.players[game.current_player];

        printf("\n============================\n");
        printf("Joueur %d (score: %d)\n", player->id, player->score);

        print_table(&game.table);
        print_rack(&player->rack);

        printf("\nActions:\n");
        printf("1 - Piocher\n");
        printf("2 - Poser une combinaison\n");
        printf("3 - Passer\n");
        printf("Choix: ");

        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            if (!game_action_draw(&game))
                printf("Impossible de piocher\n");

        } else if (choice == 2) {
            Combination_t combo;
            combination_init(&combo);

            int n;
            printf("Nombre de tuiles: ");
            scanf("%d", &n);

            for (int i = 0; i < n; i++) {
                int id;
                printf("ID tuile %d: ", i + 1);
                scanf("%d", &id);

                for (int j = 0; j < player->rack.count; j++) {
                    if (player->rack.tiles[j].id == id) {
                        combination_add_tile(&combo, player->rack.tiles[j]);
                        break;
                    }
                }
            }

            if (!game_action_play_combination(&game, &combo))
                printf("Combinaison invalide\n");

        } else if (choice == 3) {
            game_action_pass(&game);

        } else {
            printf("Choix invalide\n");
        }
    }

    printf("\n=== FIN DE PARTIE ===\n");
    game_compute_final_scores(&game);

    for (size_t i = 0; i < game.player_count; i++) {
        printf("Joueur %zu : score %d\n", i, game.players[i].score);
    }

    return 0;
}
