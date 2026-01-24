#ifndef UI_H
#define UI_H

#include "game.h"
#include "assets.h"

void ui_init(void);
void ui_render(GameState *g, Assets *a);
void ui_handle_click(int x, int y);

#endif
