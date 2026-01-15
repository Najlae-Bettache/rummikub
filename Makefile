NAME        := rummikub
CC          := gcc
CFLAGS      := -Wall -Wextra -Werror -std=c11 -g
INCLUDES    := -Iinclude -lcsfml-graphics -lcsfml-window -lcsfml-system

SRC_DIR     := src
TEST_DIR    := tests
OBJ_DIR     := build

CORE_SRC    :=
BOARD_SRC   := \
    $(SRC_DIR)/board/tile.c \
    $(SRC_DIR)/board/deck.c \
    $(SRC_DIR)/board/rack.c \
    $(SRC_DIR)/board/combination.c \
    $(SRC_DIR)/board/table.c

RULES_SRC   := \
    $(SRC_DIR)/rules/rules.c

AI_SRC	  := \
	$(SRC_DIR)/ai/ai.c

GAME_SRC    :=\
    $(SRC_DIR)/game/game.c
	
UI_SRC      :=
NET_SRC     :=

SRC         := \
    $(CORE_SRC) \
    $(BOARD_SRC) \
    $(RULES_SRC) \
    $(GAME_SRC) \
    $(UI_SRC) \
    $(NET_SRC) \
	$(AI_SRC) \
    $(SRC_DIR)/main_ui.c

OBJ         := $(SRC:%.c=$(OBJ_DIR)/%.o)

TEST_SRC    := \
    $(TEST_DIR)/test_tile.c \
    $(TEST_DIR)/test_deck.c \
    $(TEST_DIR)/test_rack.c \
    $(TEST_DIR)/test_combination.c \
	$(TEST_DIR)/test_table.c \
	$(TEST_DIR)/test_rules.c \
	$(TEST_DIR)/test_game.c	 \
	$(TEST_DIR)/test_ai.c

TEST_BIN    := $(TEST_SRC:$(TEST_DIR)/%.c=$(OBJ_DIR)/tests/%)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(NAME)"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

tests: $(TEST_BIN)
	@echo "All tests compiled"

$(OBJ_DIR)/tests/%: $(TEST_DIR)/%.c $(BOARD_SRC) $(RULES_SRC) $(GAME_SRC) $(AI_SRC)
	@mkdir -p $(dir $@)
	@echo "$@"
	@$(CC) $(CFLAGS) $(INCLUDES) $< $(BOARD_SRC) $(RULES_SRC) $(GAME_SRC) $(AI_SRC) -o $@

test: tests
	@echo "Running tests"
	@for t in $(TEST_BIN); do \
		echo "-> Running $$t"; \
		$$t || exit 1; \
	done
	@echo "All tests passed!"

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

print:
	@echo "Sources:"
	@echo $(SRC)
