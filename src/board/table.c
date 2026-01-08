#include "../include/table.h"

void table_init(Table_t *table)
{
    table->count = 0;
}

bool table_add_combination(Table_t *table, const Combination_t *combination)
{
    if (table->count >= TABLE_MAX_COMBINATIONS) {
        return false;
    }
    table->combinations[table->count++] = *combination;
    return true;
}

bool table_remove_combination(Table_t *table, size_t index)
{
    if (index >= table->count) {
        return false;
    }
    for (size_t i = index; i < table->count - 1; i++) {
        table->combinations[i] = table->combinations[i + 1];
    }
    table->count--;
    return true;
}

Combination_t *table_get_combination(Table_t *table, size_t index)
{
    if (index >= table->count) {
        return NULL;
    }
    return &table->combinations[index];
}

const Combination_t *table_get_combination_const(const Table_t *table, size_t index)
{
    if (index >= table->count) {
        return NULL;
    }
    return &table->combinations[index];
}

bool table_is_empty(const Table_t *table)
{
    return table->count == 0;
}

void table_clear(Table_t *table)
{
    table->count = 0;
}