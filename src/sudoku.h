#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <stdint.h>
#include "utils.h"

#define WIDTH 3
#define HEIGHT 3

typedef uint16_t val_t;

typedef struct {
  uint32_t depth;
  uint32_t width, height;
  uint32_t range;   // = width*height;
  val_t    *values; // len = range^2
  uint8_t  *map;    // range^3;

  u32_astack_t to_set;

  u32_astack_t tries;
} sudoku_t;

void sudoku_init(sudoku_t *sudoku, uint32_t width, uint32_t height);
void sudoku_reset(sudoku_t *sudoku);
void sudoku_copy(sudoku_t *dst_sudoku, const sudoku_t *sudoku, int _new);
void sudoku_load_from_file(sudoku_t *sudoku, const char *filepath);
void sudoku_print(const sudoku_t *sudoku);
int  sudoku_mask_value(sudoku_t *sudoku, uint32_t x, uint32_t y, val_t value);
int  sudoku_set_value(sudoku_t *sudoku, uint32_t x, uint32_t y, val_t value);
int  sudoku_check_square(sudoku_t *sudoku, uint32_t x, uint32_t y);
int  sudoku_solve(sudoku_t *sudoku, int *solutions, int max_solutions);

#endif
