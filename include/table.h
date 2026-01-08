#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "combination.h"

#define TABLE_MAX_COMBINATIONS 64

typedef struct Table_s {
    Combination_t combinations[TABLE_MAX_COMBINATIONS];
    size_t count;
} Table_t;

void table_init(Table_t *);
bool table_add_combination(Table_t *, const Combination_t *);
bool table_remove_combination(Table_t *, size_t);
Combination_t *table_get_combination(Table_t *, size_t);
const Combination_t *table_get_combination_const(const Table_t *, size_t);
bool table_is_empty(const Table_t *);
void table_clear(Table_t *);