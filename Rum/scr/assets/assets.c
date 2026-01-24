#include "assets.h"

Assets assets_load_all(void) {
    Assets a;
    a.font = sfFont_createFromFile("assets/fonts/main_font.ttf");
    return a;
}

void assets_destroy(Assets *a) {
    sfFont_destroy(a->font);
}
