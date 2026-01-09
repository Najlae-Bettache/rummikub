#include <assert.h>
#include <stdio.h>
#include "game.h"

static Game_t create_game(size_t players)
{
    Game_t g;
    bool ok = game_init(&g, players);
    assert(ok);
    return g;
}

static Combination_t make_series(int value, int id_start)
{
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(value, TILE_RED, 0, id_start + 1000));
    combination_add_tile(&c, tile_create(value, TILE_BLUE, 0, id_start + 1001));
    combination_add_tile(&c, tile_create(value, TILE_YELLOW, 0, id_start + 1002));

    return c;
}

static Combination_t make_big_series(void)
{
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(10, TILE_RED, 0, 1001));
    combination_add_tile(&c, tile_create(10, TILE_BLUE, 0, 1002));
    combination_add_tile(&c, tile_create(10, TILE_YELLOW, 0, 1003));
    combination_add_tile(&c, tile_create(10, TILE_BLACK, 0, 1004));

    return c;
}

void test_init_valid(void)
{
    Game_t g = create_game(4);
    assert(g.player_count == 4);
    assert(g.state == GAME_STATE_TURN);
}

void test_init_invalid_players(void)
{
    Game_t g;
    assert(!game_init(&g, 1));
    assert(!game_init(&g, 5));
}

void test_distribution(void)
{
    Game_t g = create_game(4);

    for (size_t i = 0; i < g.player_count; i++)
        assert(rack_count(&g.players[i].rack) == RACK_START);

    assert(g.deck.count == 106 - 4 * RACK_START);
}

void test_turn_rotation(void)
{
    Game_t g = create_game(3);

    assert(game_get_current_player(&g) == 0);
    game_next_turn(&g);
    assert(game_get_current_player(&g) == 1);
    game_next_turn(&g);
    assert(game_get_current_player(&g) == 2);
    game_next_turn(&g);
    assert(game_get_current_player(&g) == 0);
}

void test_draw_action(void)
{
    Game_t g = create_game(2);
    size_t before = rack_count(&g.players[0].rack);

    assert(game_action_draw(&g));
    assert(rack_count(&g.players[0].rack) == (int)before + 1);
    assert(game_get_current_player(&g) == 1);
}

void test_pass_action(void)
{
    Game_t g = create_game(2);
    game_action_pass(&g);
    assert(game_get_current_player(&g) == 1);
}

void test_play_valid_combination(void)
{
    Game_t g = create_game(2);
    Combination_t c = make_big_series();
    GamePlayer_t *p = &g.players[0];
    rack_add_tile(&p->rack, c.tiles[0]);
    rack_add_tile(&p->rack, c.tiles[1]);
    rack_add_tile(&p->rack, c.tiles[2]);
    rack_add_tile(&p->rack, c.tiles[3]);

    assert(game_action_play_combination(&g, &c));
    assert(p->has_opened);
    assert(g.table.count == 1);
}

void test_opening_rule_fail(void)
{
    Game_t g = create_game(2);
    Combination_t c = make_series(5, 100);

    GamePlayer_t *p = &g.players[0];
    rack_add_tile(&p->rack, c.tiles[0]);
    rack_add_tile(&p->rack, c.tiles[1]);
    rack_add_tile(&p->rack, c.tiles[2]);

    assert(!game_action_play_combination(&g, &c));
    assert(!p->has_opened);
}

void test_opening_rule_success(void)
{
    Game_t g = create_game(2);
    Combination_t c = make_big_series(); /* 40 points */

    GamePlayer_t *p = &g.players[0];
    for (size_t i = 0; i < c.count; i++)
        rack_add_tile(&p->rack, c.tiles[i]);

    assert(game_action_play_combination(&g, &c));
    assert(p->has_opened);
}

void test_play_without_tiles(void)
{
    Game_t g = create_game(2);
    Combination_t c = make_big_series();

    assert(!game_action_play_combination(&g, &c));
}

void test_end_game_by_empty_rack(void)
{
    Game_t g = create_game(2);
    GamePlayer_t *p = &g.players[0];

    p->rack.count = 0;
    g.state = GAME_STATE_END;

    assert(game_is_finished(&g));
}


void test_final_scores(void)
{
    Game_t g = create_game(2);

    g.players[0].rack.count = 0;
    g.players[1].rack.tiles[0] = tile_create(10, TILE_RED, 0, 1);
    g.players[1].rack.tiles[1] = tile_create(0, TILE_BLACK, 1, 2);
    g.players[1].rack.count = 2;

    game_compute_final_scores(&g);

    assert(g.players[1].score < 0);
    assert(g.players[0].score > 0);
}

void test_action_after_game_end(void)
{
    Game_t g = create_game(2);
    g.state = GAME_STATE_END;

    assert(!game_action_draw(&g));
}

void test_massive_draw_until_empty(void)
{
    Game_t g = create_game(2);
    while (!deck_is_empty(&g.deck)) {
        GamePlayer_t *p = &g.players[g.current_player];
        if (p->rack.count < RACK_MAX)
            game_action_draw(&g);
        else
            game_action_pass(&g);
    }
    assert(g.state == GAME_STATE_END || deck_is_empty(&g.deck));
}

int main(void)
{
    test_init_valid();
    test_init_invalid_players();
    test_distribution();
    test_turn_rotation();
    test_draw_action();
    test_pass_action();
    test_play_valid_combination();
    test_opening_rule_fail();
    test_opening_rule_success();
    test_play_without_tiles();
    test_end_game_by_empty_rack();
    test_final_scores();
    test_action_after_game_end();
    test_massive_draw_until_empty();
    return 0;
}
