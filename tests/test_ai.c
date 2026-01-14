#include <assert.h>
#include <stdio.h>

#include "../include/ai.h"
#include "../include/game.h"

static Game_t create_game_ai(size_t players)
{
    Game_t g;
    bool ok = game_init(&g, players);
    assert(ok);
    return g;
}

static void clear_rack(GamePlayer_t *p)
{
    p->rack.count = 0;
}

static void add_tile(GamePlayer_t *p, Tile_t t)
{
    rack_add_tile(&p->rack, t);
}

static Combination_t make_opening_combo(void)
{
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(11, TILE_RED, 0, 100));
    combination_add_tile(&c, tile_create(12, TILE_RED, 0, 101));
    combination_add_tile(&c, tile_create(13, TILE_RED, 0, 102));

    return c;
}

void test_ai_init(void)
{
    AIPlayer ai;
    ai_init(&ai, 1);

    assert(ai.player_id == 1);
}

void test_ai_no_combination_draw(void)
{
    Game_t g = create_game_ai(2);
    AIPlayer ai;

    ai_init(&ai, 0);

    GamePlayer_t *p = &g.players[0];
    clear_rack(p);

    add_tile(p, tile_create(1, TILE_RED, 0, 1));
    add_tile(p, tile_create(3, TILE_BLUE, 0, 2));

    size_t before = rack_count(&p->rack);

    ai_play_simple(&ai, &g);

    assert(rack_count(&p->rack) == (int)before + 1);
}

void test_ai_play_valid_opening(void)
{
    Game_t g = create_game_ai(2);
    AIPlayer ai;

    ai_init(&ai, 0);

    GamePlayer_t *p = &g.players[0];
    clear_rack(p);

    Combination_t c = make_opening_combo();

    for (size_t i = 0; i < c.count; i++)
        add_tile(p, c.tiles[i]);

    bool played = ai_play_simple(&ai, &g);

    assert(played);
    assert(p->has_opened);
    assert(g.table.count == 1);
}

void test_ai_refuses_small_opening(void)
{
    Game_t g = create_game_ai(2);
    AIPlayer ai;
    ai_init(&ai, 0);
    GamePlayer_t *p = &g.players[0];
    clear_rack(p);
    Combination_t c;
    combination_init(&c);
    combination_add_tile(&c, tile_create(5, TILE_RED, 0, 10));
    combination_add_tile(&c, tile_create(5, TILE_BLUE, 0, 11));
    combination_add_tile(&c, tile_create(5, TILE_YELLOW, 0, 12));
    for (size_t i = 0; i < c.count; i++)
        add_tile(p, c.tiles[i]);
    size_t before = rack_count(&p->rack);
    bool played = ai_play_simple(&ai, &g);
    assert(played);
    assert(!p->has_opened);
    assert(g.table.count == 0);
    assert(rack_count(&p->rack) == (int)before + 1);
}

void test_ai_play_small_combo_after_open(void)
{
    Game_t g = create_game_ai(2);
    AIPlayer ai;

    ai_init(&ai, 0);

    GamePlayer_t *p = &g.players[0];
    clear_rack(p);

    p->has_opened = true;

    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(2, TILE_RED, 0, 20));
    combination_add_tile(&c, tile_create(2, TILE_BLUE, 0, 21));
    combination_add_tile(&c, tile_create(2, TILE_YELLOW, 0, 22));

    for (size_t i = 0; i < c.count; i++)
        add_tile(p, c.tiles[i]);

    bool played = ai_play_simple(&ai, &g);

    assert(played);
    assert(g.table.count == 1);
}

void test_ai_with_full_rack(void)
{
    Game_t g = create_game_ai(2);
    AIPlayer ai;

    ai_init(&ai, 0);

    GamePlayer_t *p = &g.players[0];
    clear_rack(p);

    for (int i = 0; i < RACK_MAX; i++) {
        add_tile(p, tile_create(i % 13 + 1, TILE_RED, 0, 200 + i));
    }

    bool played = ai_play_simple(&ai, &g);

    assert(!played || g.current_player != 0);
}

void test_ai_when_game_ended(void)
{
    Game_t g = create_game_ai(2);
    AIPlayer ai;

    ai_init(&ai, 0);
    g.state = GAME_STATE_END;

    bool played = ai_play_simple(&ai, &g);

    assert(!played);
}

void test_ai_does_not_cheat(void)
{
    Game_t g = create_game_ai(2);
    AIPlayer ai;

    ai_init(&ai, 0);

    GamePlayer_t *p = &g.players[0];
    clear_rack(p);

    Combination_t fake;
    combination_init(&fake);

    combination_add_tile(&fake, tile_create(10, TILE_RED, 0, 999));
    combination_add_tile(&fake, tile_create(11, TILE_RED, 0, 998));
    combination_add_tile(&fake, tile_create(12, TILE_RED, 0, 997));

    bool played = game_action_play_combination(&g, &fake);

    assert(!played);
}

int main(void)
{
    test_ai_init();
    test_ai_no_combination_draw();
    test_ai_play_valid_opening();
    test_ai_refuses_small_opening();
    test_ai_play_small_combo_after_open();
    test_ai_with_full_rack();
    test_ai_when_game_ended();
    test_ai_does_not_cheat();
    return 0;
}
