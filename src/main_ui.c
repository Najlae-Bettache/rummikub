#include <stdio.h>
#include <stdbool.h>
#include <SFML/Graphics.h>
#include "../include/ui.h"
#include "../include/game.h"

bool ui_init(UI *ui)
{
    if (!ui)
        return false;

    sfVideoMode mode = {1280, 720, 32};

    ui->window = sfRenderWindow_create(
        mode,
        "Rummikub - CSFML",
        sfResize | sfClose,
        NULL
    );

    if (!ui->window)
        return false;

    ui->font = sfFont_createFromFile("./src/assets/font.TTF");
    if (!ui->font) {
        printf("Erreur : impossible de charger la police assets/font.TTF\n");
        sfRenderWindow_destroy(ui->window);
        return false;
    }

    sfRenderWindow_setFramerateLimit(ui->window, 60);
    return true;
}

void ui_destroy(UI *ui)
{
    if (!ui)
        return;

    if (ui->font)
        sfFont_destroy(ui->font);

    if (ui->window)
        sfRenderWindow_destroy(ui->window);
}

void ui_draw_tile(UI *ui, const Tile_t *tile, float x, float y)
{
    sfRectangleShape *rect = sfRectangleShape_create();
    sfRectangleShape_setSize(rect, (sfVector2f){TILE_WIDTH, TILE_HEIGHT});
    sfRectangleShape_setPosition(rect, (sfVector2f){x, y});
    sfRectangleShape_setFillColor(rect, sfWhite);
    sfRectangleShape_setOutlineThickness(rect, 2.f);
    sfRectangleShape_setOutlineColor(rect, sfBlack);
    sfRenderWindow_drawRectangleShape(ui->window, rect, NULL);
    sfText *text = sfText_create();
    sfText_setFont(text, ui->font);
    sfText_setCharacterSize(text, 18);
    sfText_setFillColor(text, sfBlack);

    char buffer[16];
    if (tile->is_joker)
        snprintf(buffer, sizeof(buffer), "J");
    else
        snprintf(buffer, sizeof(buffer), "%d", tile->value);

    sfText_setString(text, buffer);
    sfText_setPosition(text, (sfVector2f){x + 20.f, y + 35.f});
    sfRenderWindow_drawText(ui->window, text, NULL);
    sfText_destroy(text);
    sfRectangleShape_destroy(rect);
}

void ui_draw_rack(UI *ui, const Game_t *game)
{
    const Rack_t *rack = &game->players[game->current_player].rack;

    float start_x = TILE_MARGIN;
    float y = WINDOW_HEIGHT - TILE_HEIGHT - 20.f;

    for (int i = 0; i < rack->count; i++) {
        float x = start_x + i * (TILE_WIDTH + TILE_MARGIN);
        ui_draw_tile(ui, &rack->tiles[i], x, y);
    }
}

void ui_render(UI *ui, const Game_t *game)
{
    sfRenderWindow_clear(ui->window, sfGreen);

    sfText *text = sfText_create();
    sfText_setFont(text, ui->font);
    sfText_setCharacterSize(text, 24);
    sfText_setFillColor(text, sfWhite);

    char buffer[128];
    snprintf(
        buffer,
        sizeof(buffer),
        "Joueur %zu | Score : %d",
        game->current_player,
        game->players[game->current_player].score
    );

    sfText_setString(text, buffer);
    sfText_setPosition(text, (sfVector2f){20.f, 20.f});

    sfRenderWindow_drawText(ui->window, text, NULL);
    sfText_destroy(text);

    ui_draw_rack(ui, game);

    sfRenderWindow_display(ui->window);
}

int main(void)
{
    Game_t game;
    UI ui;

    if (!game_init(&game, 2)) {
        printf("Erreur initialisation du jeu\n");
        return 1;
    }

    if (!ui_init(&ui)) {
        printf("Erreur initialisation UI\n");
        return 1;
    }

    while (sfRenderWindow_isOpen(ui.window)) {
        sfEvent event;

        while (sfRenderWindow_pollEvent(ui.window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(ui.window);
        }

        ui_render(&ui, &game);
    }

    ui_destroy(&ui);
    return 0;
}
