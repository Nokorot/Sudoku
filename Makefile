# Config
PRGNAME = sudoku

CC = gcc
CFLAGS = -g3 -Wfatal-errors -Wall -Wextra -Wpedantic

BUILD_DIR = build

SRC = $(wildcard src/*c)
OBJ = $(SRC:src/%.c=$(BUILD_DIR)/%.o)
DEP = $(OBJ:%.o=%.d)

$(PRGNAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $< $(CFLAGS) -c -MMD

-include $(DEP)

.PHONY: clean

clean:
	rm -f $(PRGNAME) $(OBJ) .depend.mk

