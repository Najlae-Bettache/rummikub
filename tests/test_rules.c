#include <assert.h>
#include <stdio.h>
#include "../include/rules.h"

void test_series_basic_valid(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(7, TILE_RED, 0, 1));
    combination_add_tile(&c, tile_create(7, TILE_BLUE, 0, 2));
    combination_add_tile(&c, tile_create(7, TILE_YELLOW, 0, 3));

    RulesResult r = rules_validate_combination(&c);
    assert(r.valid);
    assert(r.type == RULES_COMBINATION_SERIES);
}

void test_series_invalid_color_duplicate(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(9, TILE_RED, 0, 1));
    combination_add_tile(&c, tile_create(9, TILE_RED, 0, 2));
    combination_add_tile(&c, tile_create(9, TILE_BLUE, 0, 3));

    RulesResult r = rules_validate_combination(&c);
    assert(!r.valid);
}

void test_series_invalid_value(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(8, TILE_RED, 0, 1));
    combination_add_tile(&c, tile_create(9, TILE_BLUE, 0, 2));
    combination_add_tile(&c, tile_create(8, TILE_YELLOW, 0, 3));

    RulesResult r = rules_validate_combination(&c);
    assert(!r.valid);
}

void test_series_with_joker(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(6, TILE_RED, 0, 1));
    combination_add_tile(&c, tile_create(6, TILE_BLUE, 0, 2));
    combination_add_tile(&c, tile_create(0, TILE_BLACK, 1, 3)); /* joker */

    RulesResult r = rules_validate_combination(&c);
    assert(r.valid);
    assert(r.type == RULES_COMBINATION_SERIES);
}

void test_run_basic_valid(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(3, TILE_RED, 0, 1));
    combination_add_tile(&c, tile_create(4, TILE_RED, 0, 2));
    combination_add_tile(&c, tile_create(5, TILE_RED, 0, 3));

    RulesResult r = rules_validate_combination(&c);
    assert(r.valid);
    assert(r.type == RULES_COMBINATION_RUN);
}

void test_run_unordered_input(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(10, TILE_BLUE, 0, 1));
    combination_add_tile(&c, tile_create(8, TILE_BLUE, 0, 2));
    combination_add_tile(&c, tile_create(9, TILE_BLUE, 0, 3));

    RulesResult r = rules_validate_combination(&c);
    assert(r.valid);
}

void test_run_invalid_gap(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(1, TILE_RED, 0, 1));
    combination_add_tile(&c, tile_create(3, TILE_RED, 0, 2));
    combination_add_tile(&c, tile_create(4, TILE_RED, 0, 3));

    RulesResult r = rules_validate_combination(&c);
    assert(!r.valid);
}

void test_run_invalid_color(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(5, TILE_RED, 0, 1));
    combination_add_tile(&c, tile_create(6, TILE_BLUE, 0, 2));
    combination_add_tile(&c, tile_create(7, TILE_RED, 0, 3));

    RulesResult r = rules_validate_combination(&c);
    assert(!r.valid);
}

void test_run_with_single_joker_gap(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(4, TILE_YELLOW, 0, 1));
    combination_add_tile(&c, tile_create(6, TILE_YELLOW, 0, 2));
    combination_add_tile(&c, tile_create(0, TILE_BLACK, 1, 3)); /* joker */

    RulesResult r = rules_validate_combination(&c);
    assert(r.valid);
}

void test_run_with_two_jokers(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(8, TILE_BLUE, 0, 1));
    combination_add_tile(&c, tile_create(0, TILE_BLACK, 1, 2));
    combination_add_tile(&c, tile_create(0, TILE_BLACK, 1, 3));

    RulesResult r = rules_validate_combination(&c);
    assert(r.valid);
}

void test_too_many_jokers(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(0, TILE_BLACK, 1, 1));
    combination_add_tile(&c, tile_create(0, TILE_BLACK, 1, 2));
    combination_add_tile(&c, tile_create(0, TILE_BLACK, 1, 3));

    RulesResult r = rules_validate_combination(&c);
    assert(!r.valid);
}

void test_duplicate_tile_id(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(5, TILE_RED, 0, 42));
    combination_add_tile(&c, tile_create(5, TILE_BLUE, 0, 42));
    combination_add_tile(&c, tile_create(5, TILE_YELLOW, 0, 43));

    RulesResult r = rules_validate_combination(&c);
    assert(!r.valid);
}

void test_too_short(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(1, TILE_RED, 0, 1));
    combination_add_tile(&c, tile_create(2, TILE_RED, 0, 2));

    RulesResult r = rules_validate_combination(&c);
    assert(!r.valid);
}

void test_mixed_series_and_run(void) {
    Combination_t c;
    combination_init(&c);

    combination_add_tile(&c, tile_create(7, TILE_RED, 0, 1));
    combination_add_tile(&c, tile_create(8, TILE_RED, 0, 2));
    combination_add_tile(&c, tile_create(7, TILE_BLUE, 0, 3));

    RulesResult r = rules_validate_combination(&c);
    assert(!r.valid);
}

int main(void) {
    test_series_basic_valid();
    test_series_invalid_color_duplicate();
    test_series_invalid_value();
    test_series_with_joker();
    test_run_basic_valid();
    test_run_unordered_input();
    test_run_invalid_gap();
    test_run_invalid_color();
    test_run_with_single_joker_gap();
    test_run_with_two_jokers();
    test_too_many_jokers();
    test_duplicate_tile_id();
    test_too_short();
    test_mixed_series_and_run();
    return 0;
}
