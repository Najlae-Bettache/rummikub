#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.h>
#include "../include/ui.h"
#include "../include/game.h"
#include "../include/ai.h"
#include <math.h>

static ScoreEntry g_scores[MAX_SCORES];
static int g_score_count = 0;

static bool ui_is_table_tile_selected(UI *ui, int combo_i, int tile_i)
{
    for (int i = 0; i < ui->selected_count; i++) {
        if (ui->selected_tiles[i].from_table &&
            ui->selected_tiles[i].combo_i == combo_i &&
            ui->selected_tiles[i].tile_i  == tile_i)
            return true;
    }
    return false;
}

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
    ui->selected_count = 0;
    ui->rack_scroll_x = 0.f;
    ui->animation_count = 0;
    ui->state = UI_STATE_MENU;
    ui->config.player_count = 2;
    ui->config.vs_ai = true;
    ui->active_name_index = 0;
    ui->entering_name = false;
    ui->texture = sfTexture_createFromFile("./assets/fond_bois.png", NULL);
    ui->texture_menu = sfTexture_createFromFile("./assets/menu.png", NULL);
    ui->texture_config = sfTexture_createFromFile("./assets/config.png", NULL);
    ui->texture_black = (sfTexture **)malloc(sizeof(sfTexture *) * 14);
    ui->texture_blue = (sfTexture **)malloc(sizeof(sfTexture *) * 14);
    ui->texture_red = (sfTexture **)malloc(sizeof(sfTexture *) * 14);
    ui->texture_yellow = (sfTexture **)malloc(sizeof(sfTexture *) * 14);
    for (int i = 0; i < 13; i++) {
        char black[256];
        char blue[256];
        char red[256];
        char yellow[256];
        snprintf(black, sizeof(black), "./assets/tuile_black_%i.png", i + 1);
        snprintf(blue, sizeof(blue), "./assets/tuile_blue_%i.png", i + 1);
        snprintf(red, sizeof(red), "./assets/tuile_red_%i.png", i + 1);
        snprintf(yellow, sizeof(yellow), "./assets/tuile_yellow_%i.png", i + 1);
        ui->texture_black[i] = sfTexture_createFromFile(black, NULL);
        ui->texture_blue[i] = sfTexture_createFromFile(blue, NULL);
        ui->texture_red[i] = sfTexture_createFromFile(red, NULL);
        ui->texture_yellow[i] = sfTexture_createFromFile(yellow, NULL);
    }
    ui->texture_joker = sfTexture_createFromFile("./assets/tuile_joker.png", NULL);
    ui->texture_rule = sfTexture_createFromFile("./assets/rules.png", NULL);
    if (!ui->texture) {
        printf("Erreur chargement tileset\n");
        return false;
    }

    ui->sprite = sfSprite_create();
    sfSprite_setTexture(ui->sprite, ui->texture, sfTrue);
    for (int i = 0; i < MAX_PLAYERS; i++)
        strcpy(ui->config.player_names[i], "Joueur");
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
    sfSprite_destroy(ui->sprite);
    sfTexture_destroy(ui->texture);
    for (int i = 0; i < 13; i++) {
        sfTexture_destroy(ui->texture_black[i]);
        sfTexture_destroy(ui->texture_blue[i]);
        sfTexture_destroy(ui->texture_red[i]);
        sfTexture_destroy(ui->texture_yellow[i]);
    }
    sfTexture_destroy(ui->texture_joker);
    sfTexture_destroy(ui->texture_menu);
    sfTexture_destroy(ui->texture_rule);
    free(ui->texture_black);
    free(ui->texture_blue);
    free(ui->texture_red);
    free(ui->texture_yellow);
}

bool game_current_player_is_ai(const Game_t *game)
{
    return game->players[game->current_player].is_ai;
}

void load_scores(void)
{
    FILE *f = fopen("scores.dat", "rb");
    if (!f) {
        g_score_count = 0;
        return;
    }
    g_score_count = fread(
        g_scores,
        sizeof(ScoreEntry),
        MAX_SCORES,
        f
    );
    fclose(f);
}

void save_scores(void)
{
    FILE *f = fopen("scores.dat", "wb");
    if (!f)
        return;
    fwrite(
        g_scores,
        sizeof(ScoreEntry),
        g_score_count,
        f
    );
    fclose(f);
}

void add_score(const char *name, int score)
{
    for (int i = 0; i < g_score_count; i++) {
        if (strcmp(g_scores[i].name, name) == 0) {
            g_scores[i].total_score += score;
            save_scores();
            return;
        }
    }
    if (g_score_count < MAX_SCORES) {
        strncpy(
            g_scores[g_score_count].name,
            name,
            NAME_MAX_LEN - 1
        );
        g_scores[g_score_count].name[NAME_MAX_LEN - 1] = '\0';
        g_scores[g_score_count].total_score = score;
        g_score_count++;
        save_scores();
    }
}

bool ui_is_tile_selected(const UI *ui, int index)
{
    for (int i = 0; i < ui->selected_count; i++) {
        if (ui->selected_indices[i] == index)
            return true;
    }
    return false;
}

void ui_toggle_tile_selection(UI *ui, int index)
{
    for (int i = 0; i < ui->selected_count; i++) {
        if (ui->selected_indices[i] == index) {
            ui->selected_indices[i] =
                ui->selected_indices[ui->selected_count - 1];
            ui->selected_count--;
            return;
        }
    }
    if (ui->selected_count < UI_MAX_SELECTION) {
        ui->selected_indices[ui->selected_count++] = index;
    }
}

sfColor ui_color_from_tile(const Tile_t *tile)
{
    if (tile->is_joker)
        return sfColor_fromRGB(180, 180, 180);
    switch (tile->color) {
        case TILE_RED:
            return sfColor_fromRGB(220, 70, 70);
        case TILE_BLUE:
            return sfColor_fromRGB(70, 70, 220);
        case TILE_YELLOW:
            return sfColor_fromRGB(220, 200, 70);
        case TILE_BLACK:
            return sfColor_fromRGB(40, 40, 40);
        default:
            return sfWhite;
    }
}

sfColor ui_text_color_for_tile(const Tile_t *tile)
{
    if (tile->is_joker)
        return sfBlack;

    if (tile->color == TILE_BLACK || tile->color == TILE_BLUE)
        return sfWhite;

    return sfBlack;
}

bool ui_build_combination_from_selection(
    const UI *ui,
    const Game_t *game,
    Combination_t *out_combo
)
{
    if (ui->selected_count < 3)
        return false;
    combination_clear(out_combo);
    const Rack_t *rack = &game->players[game->current_player].rack;
    for (int i = 0; i < ui->selected_count; i++) {
        int index = ui->selected_indices[i];
        if (index < 0 || index >= rack->count)
            return false;
        combination_add_tile(out_combo, rack->tiles[index]);
    }
    return true;
}

void ui_draw_tile(UI *ui, const Tile_t *tile,
                  float x, float y, bool selected)
{
    if (tile->is_joker) {
        sfSprite_setTexture(ui->sprite, ui->texture_joker, sfTrue);
    } else if (tile->color == TILE_BLUE) {
        sfSprite_setTexture(ui->sprite, ui->texture_blue[tile->value - 1], sfTrue);
    } else if (tile->color == TILE_BLACK) {
        sfSprite_setTexture(ui->sprite, ui->texture_black[tile->value - 1], sfTrue);
    } else if (tile->color == TILE_RED) {
        sfSprite_setTexture(ui->sprite, ui->texture_red[tile->value - 1], sfTrue);
    } else if (tile->color == TILE_YELLOW) {
        sfSprite_setTexture(ui->sprite, ui->texture_yellow[tile->value - 1], sfTrue);
    }
    sfSprite_setPosition(ui->sprite, (sfVector2f){x, y});
    if (selected)
        sfSprite_setColor(ui->sprite, sfColor_fromRGB(180, 255, 180));
    else
        sfSprite_setColor(ui->sprite, sfWhite);
    sfRenderWindow_drawSprite(ui->window, ui->sprite, NULL);
}

void ui_draw_rack(UI *ui, const Game_t *game)
{
    const Rack_t *rack = &game->players[game->current_player].rack;
    float rack_width =
        rack->count * (TILE_WIDTH + TILE_MARGIN);
    float max_scroll = 0.f;
    float min_scroll = 0.f;
    if (rack_width > RACK_VIEW_WIDTH) {
        max_scroll = rack_width - RACK_VIEW_WIDTH;
    }
    if (ui->rack_scroll_x < min_scroll)
        ui->rack_scroll_x = min_scroll;
    if (ui->rack_scroll_x > max_scroll)
        ui->rack_scroll_x = max_scroll;
    float start_x = RACK_VIEW_X;
    float y = WINDOW_HEIGHT - TILE_HEIGHT - 20.f;
    for (int i = 0; i < rack->count; i++) {
        float x =
            start_x +
            i * (TILE_WIDTH + TILE_MARGIN) -
            ui->rack_scroll_x;
        if (x + TILE_WIDTH < RACK_VIEW_X ||
            x > RACK_VIEW_X + RACK_VIEW_WIDTH)
            continue;
        bool selected = ui_is_tile_selected(ui, i);
        ui_draw_tile(ui, &rack->tiles[i], x, y, selected);
    }
}

void ui_draw_table(UI *ui, const Game_t *game)
{
    const Table_t *table = &game->table;
    for (size_t i = 0; i < table->count; i++) {
        const Combination_t *combo = &table->combinations[i];
        float y =
            TABLE_START_Y +
            i * (TILE_HEIGHT + TABLE_ROW_GAP) - ui->table_scroll_y;
        for (size_t j = 0; j < combo->count; j++) {
            float x =
                TABLE_START_X +
                j * (TILE_WIDTH + TILE_MARGIN);
            bool selected =
                ui_is_table_tile_selected(ui, i, j);
            ui_draw_tile(ui, &combo->tiles[j], x, y, selected);
        }
    }
}

sfFloatRect ui_draw_button(UI *ui, const char *label, float x, float y, sfColor color)
{
    sfRectangleShape *rect = sfRectangleShape_create();
    sfRectangleShape_setSize(rect, (sfVector2f){BUTTON_WIDTH, BUTTON_HEIGHT});
    sfRectangleShape_setPosition(rect, (sfVector2f){x, y});
    sfRectangleShape_setFillColor(rect, color);
    sfRectangleShape_setOutlineThickness(rect, 2.f);
    sfRectangleShape_setOutlineColor(rect, sfBlack);
    sfRenderWindow_drawRectangleShape(ui->window, rect, NULL);
    sfText *text = sfText_create();
    sfText_setFont(text, ui->font);
    sfText_setCharacterSize(text, 20);
    sfText_setFillColor(text, sfWhite);
    sfText_setString(text, label);
    sfFloatRect bounds = sfText_getLocalBounds(text);
    sfText_setPosition(
        text,
        (sfVector2f){
            x + (BUTTON_WIDTH - bounds.width) / 2.f,
            y + 6.f
        }
    );
    sfRenderWindow_drawText(ui->window, text, NULL);
    sfText_destroy(text);
    sfRectangleShape_destroy(rect);
    return (sfFloatRect){x, y, BUTTON_WIDTH, BUTTON_HEIGHT};
}

sfFloatRect ui_draw_menu_button(UI *ui, const char *label, float x, float y)
{
    sfRectangleShape *rect = sfRectangleShape_create();
    sfRectangleShape_setSize(rect, (sfVector2f){260.f, 60.f});
    sfRectangleShape_setPosition(rect, (sfVector2f){x, y});
    sfRectangleShape_setFillColor(rect, sfColor_fromRGB(245, 189, 7));
    sfRectangleShape_setOutlineThickness(rect, 3.f);
    sfRectangleShape_setOutlineColor(rect, sfWhite);
    sfRenderWindow_drawRectangleShape(ui->window, rect, NULL);
    sfText *text = sfText_create();
    sfText_setFont(text, ui->font);
    sfText_setCharacterSize(text, 28);
    sfText_setFillColor(text, sfWhite);
    sfText_setString(text, label);
    sfFloatRect bounds = sfText_getLocalBounds(text);
    sfText_setPosition(
        text,
        (sfVector2f){
            x + (260.f - bounds.width) / 2.f,
            y + 10.f
        }
    );
    sfRenderWindow_drawText(ui->window, text, NULL);
    sfText_destroy(text);
    sfRectangleShape_destroy(rect);
    return (sfFloatRect){x, y, 260.f, 60.f};
}

void ui_update_animations(UI *ui, float dt)
{
    for (int i = 0; i < ui->animation_count; i++) {
        TileAnimation *a = &ui->animations[i];
        a->elapsed += dt;

        float t = a->elapsed / a->duration;
        if (t > 1.f) t = 1.f;
        a->now.x = a->start.x + t * (a->end.x - a->start.x);
        a->now.y = a->start.y + t * (a->end.y - a->start.y);
    }
    int write = 0;
    for (int i = 0; i < ui->animation_count; i++) {
        if (ui->animations[i].elapsed < ui->animations[i].duration) {
            ui->animations[write++] = ui->animations[i];
        }
    }
    ui->animation_count = write;
}

void ui_render_config(UI *ui)
{
    sfRenderWindow_clear(ui->window, sfColor_fromRGB(25, 25, 25));
    sfSprite_setTexture(ui->sprite, ui->texture_config, sfTrue);
    sfSprite_setPosition(ui->sprite, (sfVector2f){0.0f, 0.0f});
    sfRenderWindow_drawSprite(ui->window, ui->sprite, NULL);
    sfText *title = sfText_create();
    sfText_setFont(title, ui->font);
    sfText_setCharacterSize(title, 40);
    sfText_setFillColor(title, sfWhite);
    sfText_setString(title, "Configuration");
    sfFloatRect tb = sfText_getLocalBounds(title);
    sfText_setPosition(
        title,
        (sfVector2f){(WINDOW_WIDTH - tb.width) / 2.f, 60.f}
    );
    sfRenderWindow_drawText(ui->window, title, NULL);
    sfText_destroy(title);
    float x = (WINDOW_WIDTH - 300.f) / 2.f;
    float y = 150.f;
    float spacing = 70.f;
    char mode_text[64];
    snprintf(
        mode_text,
        sizeof(mode_text),
        "Mode : %s",
        ui->config.vs_ai ? "SOLO (vs IA)" : "MULTI"
    );
    ui->btn_mode = ui_draw_menu_button(ui, mode_text, x, y);
    y += spacing;
    char players_text[64];
    snprintf(
        players_text,
        sizeof(players_text),
        "Nombre de joueurs : %d",
        ui->config.player_count
    );
    ui->btn_players = ui_draw_menu_button(ui, players_text, x, y);
    y += spacing + 10.f;
    for (int i = 0; i < ui->config.player_count; i++) {
        char label[64];
        snprintf(
            label,
            sizeof(label),
            "Nom Joueur %d : %s",
            i + 1,
            ui->config.player_names[i]
        );
        ui->btn_name[i] = ui_draw_menu_button(ui, label, x, y);
        y += spacing;
    }
    y += 20.f;
    ui->btn_start = ui_draw_menu_button(ui, "DEMARRER", x, y);
    y += spacing;
    ui->btn_back = ui_draw_menu_button(ui, "RETOUR", x, y);
    sfRenderWindow_display(ui->window);
}

void ui_render_menu(UI *ui)
{
    sfRenderWindow_clear(ui->window, sfColor_fromRGB(30, 30, 30));
    sfSprite_setTexture(ui->sprite, ui->texture_menu, sfTrue);
    sfSprite_setPosition(ui->sprite, (sfVector2f){0.0f, 0.0f});
    sfRenderWindow_drawSprite(ui->window, ui->sprite, NULL);
    float center_x = (WINDOW_WIDTH - 260.f) / 2.f;
    sfFloatRect btn_play = ui_draw_menu_button(ui, "JOUER", center_x, 300.f);
    sfFloatRect btn_regle = ui_draw_menu_button(ui, "VOIR LES REGLES", center_x, 380.f);
    sfFloatRect btn_quit = ui_draw_menu_button(ui, "QUITTER", center_x, 460.f);
    ui->menu_play_bounds = btn_play;
    ui->menu_rule_bounds = btn_regle;
    ui->menu_quit_bounds = btn_quit;
    (void)btn_regle;
    sfRenderWindow_display(ui->window);
}

void ui_render_pause(UI *ui)
{
    float x = (WINDOW_WIDTH - 260.f) / 2.f;
    ui->btn_resume = ui_draw_menu_button(ui, "REPRENDRE", x, 260.f);
    ui->btn_quit_menu = ui_draw_menu_button(ui, "MENU PRINCIPAL", x, 340.f);
    sfRenderWindow_display(ui->window);
}

void ui_render_end(UI *ui, const Game_t *game)
{
    sfRenderWindow_clear(ui->window, sfColor_fromRGB(20, 20, 20));
    int winner = 0;
    int best_score = game->players[0].score;
    for (int i = 1; i < (int)game->player_count; i++) {
        if (game->players[i].score > best_score) {
            best_score = game->players[i].score;
            winner = i;
        }
    }
    char title_text[128];
    snprintf(
        title_text,
        sizeof(title_text),
        "Victoire de %s !",
        ui->config.player_names[winner]
    );
    sfText *title = sfText_create();
    sfText_setFont(title, ui->font);
    sfText_setCharacterSize(title, 44);
    sfText_setFillColor(title, sfWhite);
    sfText_setString(title, title_text);
    sfFloatRect tb = sfText_getLocalBounds(title);
    sfText_setPosition(
        title,
        (sfVector2f){(WINDOW_WIDTH - tb.width) / 2.f, 120.f}
    );
    sfRenderWindow_drawText(ui->window, title, NULL);
    sfText_destroy(title);
    char score_text[64];
    snprintf(score_text, sizeof(score_text),
             "Score final : %d points", best_score);
    sfText *score = sfText_create();
    sfText_setFont(score, ui->font);
    sfText_setCharacterSize(score, 26);
    sfText_setFillColor(score, sfWhite);
    sfText_setString(score, score_text);
    sfFloatRect sb = sfText_getLocalBounds(score);
    sfText_setPosition(
        score,
        (sfVector2f){(WINDOW_WIDTH - sb.width) / 2.f, 200.f}
    );
    sfRenderWindow_drawText(ui->window, score, NULL);
    sfText_destroy(score);
    float x = (WINDOW_WIDTH - 260.f) / 2.f;
    float y = 300.f;
    float spacing = 80.f;
    ui->btn_menu = ui_draw_menu_button(ui, "MENU PRINCIPAL", x, y);
    ui->btn_quit = ui_draw_menu_button(ui, "QUITTER", x, y + spacing);
    sfRenderWindow_display(ui->window);
}

void ui_render_rule(UI *ui)
{
    sfRenderWindow_clear(ui->window, sfWhite);
    sfSprite_setTexture(ui->sprite, ui->texture_rule, sfTrue);
    sfSprite_setPosition(ui->sprite, (sfVector2f){0.0f, 0.0f});
    sfRenderWindow_drawSprite(ui->window, ui->sprite, NULL);
    sfRenderWindow_display(ui->window);
}

void ui_render(UI *ui, const Game_t *game)
{
    if (ui->state == UI_STATE_MENU) {
        ui_render_menu(ui);
        return;
    }
    if (ui->state == UI_STATE_SETTING) {
        ui_render_config(ui);
        return;
    }
    if (ui->state == UI_STATE_PAUSE) {
        ui_render_pause(ui);
        return;
    }
    if (ui->state == UI_STATE_RULE) {
        ui_render_rule(ui);
        return;
    }
    if (ui->state == UI_STATE_END) {
        ui_render_end(ui, game);
        return;
    }
    ui_update_animations(ui, 1.f / 60.f);
    if (ui->state == UI_STATE_GAME && !game_current_player_is_ai(game)) {
        sfRenderWindow_clear(ui->window, sfWhite);
        sfSprite_setTexture(ui->sprite, ui->texture, sfTrue);
        sfSprite_setPosition(ui->sprite, (sfVector2f){0.0f, 0.0f});
        sfRenderWindow_drawSprite(ui->window, ui->sprite, NULL);
        sfText *text = sfText_create();
        sfText_setFont(text, ui->font);
        sfText_setCharacterSize(text, 30);
        sfText_setFillColor(text, sfWhite);
        char buffer[128];
        snprintf(
            buffer,
            sizeof(buffer),
            "%s | Score : %d",
            ui->config.player_names[game->current_player],
            game->players[game->current_player].score
        );
        sfText_setString(text, buffer);
        sfText_setPosition(text, (sfVector2f){20.f, 20.f});
        sfRenderWindow_drawText(ui->window, text, NULL);
        sfText_destroy(text);
        ui_draw_table(ui, game);
        ui_draw_rack(ui, game);
        for (int i = 0; i < ui->animation_count; i++) {
            TileAnimation *a = &ui->animations[i];
            ui_draw_tile(ui, &a->tile, a->now.x, a->now.y, false);
        }
        float base_y = WINDOW_HEIGHT - BUTTON_HEIGHT - 20.f;
        float base_x = WINDOW_WIDTH - BUTTON_WIDTH - 20.f;
        ui->btn_play =
            ui_draw_button(ui, "JOUER", base_x, base_y,
                        sfColor_fromRGB(50, 150, 50));
        ui->btn_draw =
            ui_draw_button(ui, "PIOCHER",
                        base_x,
                        base_y - (BUTTON_HEIGHT + BUTTON_GAP),
                        sfColor_fromRGB(50, 50, 150));
        ui->btn_pass =
            ui_draw_button(ui, "PASSER",
                        base_x,
                        base_y - 2 * (BUTTON_HEIGHT + BUTTON_GAP),
                        sfColor_fromRGB(120, 120, 120));
        sfRenderWindow_display(ui->window);
    }
}

static int ui_find_selected(UI *ui, int rack_index)
{
    for (int i = 0; i < ui->selected_count; i++)
        if (ui->selected_indices[i] == rack_index)
            return i;
    return -1;
}

static void ui_remove_selected(UI *ui, int idx)
{
    for (int i = idx; i < ui->selected_count - 1; i++) {
        ui->selected_indices[i] = ui->selected_indices[i+1];
        ui->selected_tiles[i]   = ui->selected_tiles[i+1];
    }
    ui->selected_count--;
}

int main(void)
{
    Game_t game;
    UI ui;
    sfClock *clock = sfClock_create();
    srand((unsigned int)time(NULL));
    if (!game_init(&game, 2)) {
        printf("Erreur initialisation du jeu\n");
        return 1;
    }
    if (!ui_init(&ui)) {
        printf("Erreur initialisation UI\n");
        return 1;
    }
    load_scores();

    while (sfRenderWindow_isOpen(ui.window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(ui.window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(ui.window);
            if (event.type == sfEvtMouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sfMouseHorizontalWheel ||
                    event.mouseWheelScroll.wheel == sfMouseVerticalWheel)
                {
                    float my = event.mouseWheelScroll.y;
                    float delta = event.mouseWheelScroll.delta * 40.f;
                    if (my > WINDOW_HEIGHT - TILE_HEIGHT - 60.f)
                    {
                        ui.rack_scroll_x -= delta;
                    }
                    else if (my >= TABLE_VIEW_Y && my <= TABLE_VIEW_Y + TABLE_VIEW_HEIGHT)
                    {
                        ui.table_scroll_y -= delta;
                    }
                    float table_height = game.table.count * (TILE_HEIGHT + 20.f);
                    float max_scroll = fmaxf(0.f, table_height - TABLE_VIEW_HEIGHT);
                    if (ui.table_scroll_y < 0)
                        ui.table_scroll_y = 0;
                    if (ui.table_scroll_y > max_scroll)
                        ui.table_scroll_y = max_scroll;
                }
            }
            if (ui.state == UI_STATE_GAME &&
                game_is_finished(&game))
            {
                ui.state = UI_STATE_END;
                for (int i = 0; i < (int)game.player_count; i++) {
                    add_score(ui.config.player_names[i],
                            game.players[i].score);
                }
                game_compute_final_scores(&game);
                save_scores();
            }
            if (ui.state == UI_STATE_END &&
                event.type == sfEvtMouseButtonPressed)
            {
                float mx = event.mouseButton.x;
                float my = event.mouseButton.y;
                if (sfFloatRect_contains(&ui.btn_menu, mx, my)) {
                    ui.state = UI_STATE_MENU;
                }
                else if (sfFloatRect_contains(&ui.btn_quit, mx, my)) {
                    sfRenderWindow_close(ui.window);
                }
            }
            if (event.type == sfEvtKeyPressed &&
                event.key.code == sfKeyEscape)
            {
                if (ui.state == UI_STATE_GAME)
                    ui.state = UI_STATE_PAUSE;
                else if (ui.state == UI_STATE_PAUSE)
                    ui.state = UI_STATE_GAME;
                else if (ui.state == UI_STATE_RULE)
                    ui.state = UI_STATE_MENU;
            }
            if (ui.state == UI_STATE_MENU &&
                event.type == sfEvtMouseButtonPressed &&
                event.mouseButton.button == sfMouseLeft)
            {
                float mx = event.mouseButton.x;
                float my = event.mouseButton.y;

                if (sfFloatRect_contains(&ui.menu_play_bounds, mx, my)) {
                    ui.state = UI_STATE_SETTING;
                }
                else if (sfFloatRect_contains(&ui.menu_quit_bounds, mx, my)) {
                    sfRenderWindow_close(ui.window);
                } else if (sfFloatRect_contains(&ui.menu_rule_bounds, mx, my)) {
                    ui.state = UI_STATE_RULE;
                }
            }
            if (ui.state == UI_STATE_SETTING &&
                event.type == sfEvtMouseButtonPressed &&
                event.mouseButton.button == sfMouseLeft)
            {
                float mx = event.mouseButton.x;
                float my = event.mouseButton.y;
                if (sfFloatRect_contains(&ui.btn_mode, mx, my)) {
                    ui.config.vs_ai = !ui.config.vs_ai;

                    if (ui.config.vs_ai)
                        ui.config.player_count = 2;
                }
                else if (sfFloatRect_contains(&ui.btn_players, mx, my)) {
                    ui.config.player_count++;
                    if (ui.config.player_count > 4)
                        ui.config.player_count = 2;
                }
                else if (sfFloatRect_contains(&ui.btn_start, mx, my)) {
                    game_init(&game, ui.config.player_count);

                    if (ui.config.vs_ai) {
                        for (int i = 1; i < ui.config.player_count; i++)
                            game.players[i].is_ai = true;
                    }

                    ui.state = UI_STATE_GAME;
                }
                else if (sfFloatRect_contains(&ui.btn_back, mx, my)) {
                    ui.state = UI_STATE_MENU;
                }
            }
            if (ui.state == UI_STATE_PAUSE &&
                event.type == sfEvtMouseButtonPressed)
            {
                float mx = event.mouseButton.x;
                float my = event.mouseButton.y;
                if (sfFloatRect_contains(&ui.btn_resume, mx, my)) {
                    ui.state = UI_STATE_GAME;
                }
                else if (sfFloatRect_contains(&ui.btn_quit_menu, mx, my)) {
                    ui.state = UI_STATE_MENU;
                }
            }
            if (event.type == sfEvtMouseButtonPressed &&
                event.mouseButton.button == sfMouseLeft)
            {
                float mouse_x = event.mouseButton.x;
                float mouse_y = event.mouseButton.y;
                const Rack_t *rack = &game.players[game.current_player].rack;
                float y = WINDOW_HEIGHT - TILE_HEIGHT - 20.f;
                for (int i = 0; i < rack->count; i++) {
                    float x =
                        RACK_VIEW_X +
                        i * (TILE_WIDTH + TILE_MARGIN) -
                        ui.rack_scroll_x;
                    if (mouse_x >= x && mouse_x <= x + TILE_WIDTH &&
                        mouse_y >= y && mouse_y <= y + TILE_HEIGHT)
                    {
                        int pos = ui_find_selected(&ui, i);
                        if (pos >= 0) {
                            ui_remove_selected(&ui, pos);
                        }
                        else {
                            ui.selected_indices[ui.selected_count] = i;
                            ui.selected_tiles[ui.selected_count++] =
                                (SelectedTile){
                                    .tile = rack->tiles[i],
                                    .from_table = false
                                };
                        }
                        break;
                    }
                }
                float table_start_y = 100.f;
                for (size_t c = 0; c < game.table.count; c++) {
                    Combination_t *combo = &game.table.combinations[c];
                    float y = table_start_y + c * (TILE_HEIGHT + 20.f) - ui.table_scroll_y;
                    for (size_t t = 0; t < combo->count; t++) {
                        float x = 50.f + t * (TILE_WIDTH + 5.f);
                        if (mouse_x >= x && mouse_x <= x + TILE_WIDTH &&
                            mouse_y >= y && mouse_y <= y + TILE_HEIGHT)
                        {
                            bool found = false;
                            for (int s = 0; s < ui.selected_count; s++) {
                                if (ui.selected_tiles[s].from_table &&
                                    ui.selected_tiles[s].combo_i == (int)c &&
                                    ui.selected_tiles[s].tile_i  == (int)t)
                                {
                                    ui_remove_selected(&ui, s);
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) {
                                ui.selected_indices[ui.selected_count] = -1;
                                ui.selected_tiles[ui.selected_count++] =
                                    (SelectedTile){
                                        .tile = combo->tiles[t],
                                        .from_table = true,
                                        .combo_i = c,
                                        .tile_i = t
                                    };
                            }
                            break;
                        }
                    }
                }
            }
            if (ui.entering_name &&
                event.type == sfEvtTextEntered)
            {
                char *name = ui.config.player_names[ui.active_name_index];
                size_t len = strlen(name);
                if (event.text.unicode == 8) {
                    if (len > 0)
                        name[len - 1] = '\0';
                }
                else if (event.text.unicode == 13) {
                    ui.entering_name = false;
                    ui.active_name_index++;
                }
                else if (event.text.unicode < 128 &&
                        len < NAME_MAX_LEN - 1)
                {
                    name[len] = (char)event.text.unicode;
                    name[len + 1] = '\0';
                }
            }
            if (event.type == sfEvtMouseButtonPressed &&
                event.mouseButton.button == sfMouseLeft)
            {
                float mx = event.mouseButton.x;
                float my = event.mouseButton.y;
                for (int i = 0; i < ui.config.player_count; i++)
                    if (sfFloatRect_contains(&ui.btn_name[i], mx, my)) {
                        ui.active_name_index = i;
                        ui.entering_name = true;
                    }
                if (sfFloatRect_contains(&ui.btn_play, mx, my))
                {
                    Combination_t combo;
                    combination_init(&combo);
                    for (int i = 0; i < ui.selected_count; i++)
                        combination_add_tile(&combo, ui.selected_tiles[i].tile);
                    RulesResult r = rules_validate_combination(&combo);
                    if (!r.valid) {
                        continue;
                    }
                    GamePlayer_t *player = &game.players[game.current_player];
                    for (int i = 0; i < ui.selected_count; i++) {
                        if (ui.selected_tiles[i].from_table) {
                            Combination_t *c = &game.table.combinations[ui.selected_tiles[i].combo_i];
                            combination_remove_tile_at(c, ui.selected_tiles[i].tile_i);
                            if (c->count == 0)
                                table_remove_combination(&game.table, ui.selected_tiles[i].combo_i);
                        }
                        else {
                            rack_remove_tile_by_id(&player->rack, ui.selected_tiles[i].tile.id);
                        }
                    }
                    table_add_combination(&game.table, &combo);
                    game_next_turn(&game);
                    ui.selected_count = 0;
                    continue;
                }
                else if (sfFloatRect_contains(&ui.btn_draw, mx, my)) {
                    if (game_action_draw(&game)) {
                        ui.selected_count = 0;
                    }
                }
                else if (sfFloatRect_contains(&ui.btn_pass, mx, my)) {
                    game_action_pass(&game);
                    ui.selected_count = 0;
                }
            }
        }
        float dt = sfTime_asSeconds(sfClock_restart(clock));
        ui_update_animations(&ui, dt);
        ui_render(&ui, &game);
        if (ui.state == UI_STATE_GAME &&
            game_current_player_is_ai(&game))
        {
            sfVector2f center = {
                WINDOW_WIDTH / 2.f - 100.f,
                WINDOW_HEIGHT / 2.f - 20.f
            };
            sfRenderWindow_clear(ui.window, sfBlack);
            sfText *text = sfText_create();
            sfText_setFont(text, ui.font);
            sfText_setCharacterSize(text, 24);
            sfText_setFillColor(text, sfWhite);
            char buffer[128];
            snprintf(
                buffer,
                sizeof(buffer),
                "Tour de l'IA : %s | Score : %d",
                ui.config.player_names[game.current_player],
                game.players[game.current_player].score
            );
            sfText_setString(text, buffer);
            sfText_setPosition(text, center);
            sfRenderWindow_drawText(ui.window, text, NULL);
            sfText_destroy(text);
            sfRenderWindow_display(ui.window);
            ai_play_simple(&(AIPlayer){game.current_player}, &game);
            sfSleep(sfMilliseconds(3000));

        }
    }
    ui_destroy(&ui);
    sfClock_destroy(clock);
    return 0;
}
