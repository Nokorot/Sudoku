#ifndef GRID_PRINT_H_
#define GRID_PRINT_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint32_t columns, lines;
    uint32_t *widths, *heights;
    // uint32_t flags;
    char **text;
} text_grid;

void gird_print_init(text_grid *grid, uint32_t columns, uint32_t lines);

void grid_print(text_grid *grid);
void grid_print_fixed_width(text_grid *grid, uint32_t width);
void grid_print_fixed_height(text_grid *grid, uint32_t height);
void grid_print_srtlen(text_grid *grid);

// void print_top_line(text_grid *grid)
// void print_middle_line(text_grid *grid)
// void print_bottom_line(text_grid *grid)
// void print_text_line(text_grid *grid, uint32_t line)

#endif
