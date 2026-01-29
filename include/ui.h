#pragma once
#include <SFML/Graphics.h>
#include "game.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define TILE_WIDTH 90.f
#define TILE_HEIGHT 110.f
#define TILE_MARGIN 10.f
#define UI_MAX_SELECTION 32
#define BUTTON_WIDTH 140.f
#define BUTTON_HEIGHT 40.f
#define BUTTON_GAP 10.f
#define RACK_VIEW_X 20.f
#define RACK_VIEW_Y (WINDOW_HEIGHT - TILE_HEIGHT - 20.f)
#define RACK_VIEW_WIDTH (WINDOW_WIDTH - 200.f)
#define UI_MAX_ANIMATIONS 32
#define TABLE_START_X 40.f
#define TABLE_START_Y 80.f
#define TABLE_ROW_GAP 15.f
#define MAX_PLAYERS 4
#define NAME_MAX_LEN 32
#define MAX_SCORES 64
#define TILE_W 110
#define TILE_H 170
#define TILE_STEP_X 118
#define TILE_STEP_Y 184
#define TILE_START_X 10
#define TILE_START_Y 12

typedef enum {
    UI_STATE_MENU,
    UI_STATE_RULE,
    UI_STATE_SETTING,
    UI_STATE_GAME,
    UI_STATE_PAUSE,
    UI_STATE_END
} UIState;

typedef struct {
    char name[NAME_MAX_LEN];
    int total_score;
} ScoreEntry;

typedef struct {
    Tile_t tile;
    sfVector2f start;
    sfVector2f end;
    sfVector2f now;
    float duration;
    float elapsed;
} TileAnimation;

typedef struct {
    int player_count;
    bool vs_ai;
    char player_names[MAX_PLAYERS][NAME_MAX_LEN];
} GameConfig;

typedef struct {
    sfRenderWindow *window;
    sfFont *font;
    sfFloatRect btn_play;
    sfFloatRect btn_draw;
    sfFloatRect btn_pass;
    sfFloatRect btn_mode;
    sfFloatRect btn_players;
    sfFloatRect btn_start;
    sfFloatRect btn_back;
    sfFloatRect btn_resume;
    sfFloatRect btn_quit_menu;
    sfFloatRect btn_menu;
    sfFloatRect btn_quit;
    sfFloatRect btn_name[MAX_PLAYERS];
    int selected_indices[UI_MAX_SELECTION];
    int selected_count;
    float rack_scroll_x;
    TileAnimation animations[UI_MAX_ANIMATIONS];
    int animation_count;
    UIState state;
    sfFloatRect menu_play_bounds;
    sfFloatRect menu_quit_bounds;
    sfFloatRect menu_rule_bounds;
    GameConfig config;
    int active_name_index;
    bool entering_name;
    sfSprite *sprite;
    sfTexture *texture;
    sfTexture **texture_black;
    sfTexture **texture_blue;
    sfTexture **texture_red;
    sfTexture **texture_yellow;
    sfTexture *texture_joker;
    sfTexture *texture_menu;
    sfTexture *texture_rule;
    sfTexture *texture_config;
} UI;

bool ui_init(UI *);

void ui_destroy(UI *);

void ui_render(UI *, const Game_t *);

void ui_draw_tile(UI *, const Tile_t *, float, float, bool);

void ui_draw_rack(UI *, const Game_t *);

void ui_toggle_tile_selection(UI *, int);

bool ui_is_tile_selected(const UI *, int);

sfFloatRect ui_draw_play_button(UI *);

bool ui_build_combination_from_selection(const UI *, const Game_t *, Combination_t *);

sfColor ui_color_from_tile(const Tile_t *);

sfColor ui_text_color_for_tile(const Tile_t *);

sfFloatRect ui_draw_button(UI *, const char *, float, float, sfColor);

void ui_update_animations(UI *, float);

void ui_draw_table(UI *, const Game_t *);

sfFloatRect ui_draw_menu_button(UI *, const char *, float, float);

void ui_render_config(UI *);

void ui_render_menu(UI *);

void ui_render_pause(UI *);

void ui_render_end(UI *, const Game_t *);

void ui_render_rule(UI *);
