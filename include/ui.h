#pragma once
#include <SFML/Graphics.h>
#include "game.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

#define TILE_WIDTH   60.f
#define TILE_HEIGHT  90.f
#define TILE_MARGIN  10.f

typedef struct {
    sfRenderWindow *window;
    sfFont *font;
} UI;

bool ui_init(UI *ui);

void ui_destroy(UI *ui);

void ui_render(UI *ui, const Game_t *game);

void ui_draw_tile(UI *, const Tile_t *, float, float);

void ui_draw_rack(UI *, const Game_t *);
