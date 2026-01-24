#ifndef ASSETS_H
#define ASSETS_H

#include <SFML/Graphics.h>

typedef struct {
    sfTexture *tiles[106];
    sfTexture *board;
    sfFont    *font;
} Assets;

Assets assets_load_all(void);
void assets_destroy(Assets *a);

#endif
