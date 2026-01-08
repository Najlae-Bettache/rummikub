#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "combination.h"

typedef enum {
    RULES_COMBINATION_INVALID,
    RULES_COMBINATION_SERIES,
    RULES_COMBINATION_RUN
} RulesCombinationType;

typedef struct {
    bool valid;
    RulesCombinationType type;
    int score;
} RulesResult;

RulesResult rules_validate_combination(const Combination_t *);
RulesResult rules_validate_series(const Combination_t *);
RulesResult rules_validate_suite(const Combination_t *);
int rules_compute_score(const Combination_t *);
bool rules_pre_validation(const Combination_t *);
