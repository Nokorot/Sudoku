
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "grid_print.h"

void
gird_print_init(text_grid *grid, uint32_t columns, uint32_t lines)
{
    grid->columns = columns;
    grid->lines = lines;
    
    grid->text = (char **) malloc(columns * lines * sizeof(char *));

    grid->widths = malloc(columns * sizeof(uint32_t));
    grid->heights = malloc(lines * sizeof(uint32_t));
}

void
print_top_line(text_grid *grid)
{
    // printf("%u %u\n", grid->widths, grid->widths[0]);

    printf("┌");
    for (int j=0; j < grid->columns; ++j) {
        if (j)
            printf("┬");
        for (int i=0; i < grid->widths[j]+2; ++i) {
            printf("─");
        }
    }
    printf("┐\n");
    // exit(0);
}

void
print_middle_line(text_grid *grid)
{
    printf("├");
    for (int j=0; j < grid->columns; ++j) {
        if (j)
            printf("┼");
        for (int i=0; i < grid->widths[j]+2; ++i) {
            printf("─");
        }
    }
    printf("┤\n");
}

void
print_bottom_line(text_grid *grid)
{
    printf("└");
    for (int j=0; j < grid->columns; ++j) {
        if (j)
            printf("┴");
        for (int i=0; i < grid->widths[j]+2; ++i) {
            printf("─");
        }
    }
    printf("┘\n");
}

void
print_text_line(text_grid *grid, uint32_t line)
{
    int k;
    for (int j=0; j < grid->columns; ++j) {
        printf("│");
        printf(" %.*s ", grid->widths[j], grid->text[line*grid->columns + j]);
        k = strlen(grid->text[line*grid->columns + j]) + 2;
        for (; k < grid->widths[j]; ++k)
            printf(" ");

    }
    printf("│\n");
}

void
grid_print(text_grid *grid)
{
    // Printing the top line
    print_top_line(grid);

    int k = 0;
    int j = 0;
    for (int i=0; i < grid->lines; ++i) {
        if (k >= grid->heights[j]) {
            k = 0; ++j;
            print_middle_line(grid);
        }
        ++k;

        print_text_line(grid, i);
    }
    print_bottom_line(grid);
}

void
grid_print_fixed_width(text_grid *grid, uint32_t width)
{
    for (int i=0; i < grid->columns; ++i)
        grid->widths[i] = width;
}

void
grid_print_fixed_height(text_grid *grid, uint32_t height)
{
    for (int i=0; i < grid->lines; ++i)
        grid->heights[i] = height;
}

void
grid_print_srtlen(text_grid *grid) 
{
    uint32_t *widths = malloc(grid->columns * sizeof(uint32_t));
    // uint32_t *heights = malloc(grid.rows * sizeof(uint32_t));
    memset(widths, 0, grid->columns * sizeof(uint32_t));
    // memset(heights, 0, grid.rows * sizeof(uint32_t));

    uint32_t w;
    for (int j=0; j < grid->lines; ++j) {
        for (int i=0; i < grid->columns; ++i) {
            w = strlen(grid->text[j*grid->columns + i]);
            widths[i] = widths[i] < w ? w : widths[i];
        }
    }
    
    grid->widths = widths;
}

/* 
int main(int argc, char **argv) {
    
    text_grid grid;
    gird_print_init(&grid, 2, 3);

    grid.text[0] = "alpha";
    grid.text[1] = "beta";
    grid.text[2] = "gamma";
    grid.text[3] = "delta";
    grid.text[4] = "delta";
    grid.text[5] = "delta";

    grid_print_fixed_width(&grid, 12);
    grid_print_fixed_height(&grid, 1);

    grid_print(&grid);
} */
