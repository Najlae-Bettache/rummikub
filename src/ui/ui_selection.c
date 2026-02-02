#include "../../include/ui.h"

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

bool ui_is_table_tile_selected(UI *ui, int combo_i, int tile_i)
{
    for (int i = 0; i < ui->selected_count; i++) {
        if (ui->selected_tiles[i].from_table &&
            ui->selected_tiles[i].combo_i == combo_i &&
            ui->selected_tiles[i].tile_i  == tile_i)
            return true;
    }
    return false;
}

int ui_find_selected(UI *ui, int rack_index)
{
    for (int i = 0; i < ui->selected_count; i++)
        if (ui->selected_indices[i] == rack_index)
            return i;
    return -1;
}

void ui_remove_selected(UI *ui, int idx)
{
    for (int i = idx; i < ui->selected_count - 1; i++) {
        ui->selected_indices[i] = ui->selected_indices[i+1];
        ui->selected_tiles[i]   = ui->selected_tiles[i+1];
    }
    ui->selected_count--;
}

bool ui_build_combination_from_selection(const UI *ui, const Game_t *game, Combination_t *out_combo)
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
