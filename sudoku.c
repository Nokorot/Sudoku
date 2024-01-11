#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "grid_print.h"
#include "utils.h"
#include "sudoku.h"

uint32_t
sudoku_get_value(sudoku_t *sudoku, uint32_t x, uint32_t y)
{
    return sudoku->values[y*sudoku->range + x];
}

int
sudoku_check_square(sudoku_t *sudoku, uint32_t x, uint32_t y)
{
    uint32_t range = sudoku->range;
    if (sudoku->values[y*sudoku->range + x])
        return 0;
    
    uint16_t value = 0;
    for (int j=0; j < sudoku->range; ++j) {
        if (sudoku->map[y*range*range + x*range + j] == 0) {
            if (value != 0) 
                return 0;
            value = j+1;
        }
    }

    if (value == 0) {
        // printf("ERRRR\n");
        // PROBLEMS
        return -1;
    }

    // printf("Value %u found at %u %u \n", value, x, y);
    
    // sudoku_set_value(sudoku, x, y, value);
    u32_astack_push(&(sudoku->to_set), x);
    u32_astack_push(&(sudoku->to_set), y);
    u32_astack_push(&(sudoku->to_set), value);

    return 0;
}

int
sudoku_mask_value(sudoku_t *sudoku, uint32_t x, uint32_t y, val_t value) 
{

    uint32_t range = sudoku->range;
    uint32_t idx = y*range*range + x*range + value-1;
    if (sudoku->map[idx] == 1)
        return 0;

    // printf("Mask %u at %u %u\n", value, x, y);

    sudoku->map[idx] = 1;
    return sudoku_check_square(sudoku, x, y);
}

int
sudoku_set_value(sudoku_t *sudoku, uint32_t x, uint32_t y, val_t value)
{
    int return_val;
    uint32_t range = sudoku->range;
    uint32_t width = sudoku->width;
    uint32_t height = sudoku->height;

    assert(0 < value && value <= range);

    uint32_t box_x = x / width;
    uint32_t box_y = y / height;
    uint32_t box_rel_x = x % width;
    uint32_t box_rel_y = y % height;

    if (sudoku->values[y*range + x] != 0) {
        return - (sudoku->values[y*range + x] != value);
    }
    sudoku->values[y*range + x] = value;

    // printf("# Setting %u: \n", value);
    for (int j=0; j < range; ++j) {
        // Set the mask at (x,y)
        sudoku->map[y*range*range + x*range + j] = (j+1 != value);

        // Set the masks on row y
        if (j != x) {
            return_val = sudoku_mask_value(sudoku, j, y, value);
            if (return_val) 
                return return_val;
        }

        // Set the masks on col x
        if (j != y) {
            return_val = sudoku_mask_value(sudoku, x, j, value);
            if (return_val) 
                return return_val;
        }

        if (j != box_rel_y*width + box_rel_x) {
            return_val = sudoku_mask_value(sudoku, box_x*width + j%width, box_y*height + j/width, value);
            if (return_val) 
                return return_val;
        }
    }
    return 0;
}

void
sudoku_init(sudoku_t *sudoku, uint32_t width, uint32_t height)
{
    sudoku->width = width;
    sudoku->height = height;
    uint32_t range = sudoku->range = width*height;

    sudoku->values = malloc(range*range * sizeof(val_t));
    memset(sudoku->values, 0, range*range * sizeof(val_t));

    sudoku->map = malloc(range*range*range * sizeof(uint8_t));
    memset(sudoku->map, 0, range*range*range * sizeof(uint8_t));

    u32_astack_init(&(sudoku->to_set), sudoku->range);
}

void
sudoku_copy(sudoku_t *dst_sudoku, sudoku_t *sudoku, int new)
{
    dst_sudoku->width = sudoku->width;
    dst_sudoku->height = sudoku->height;
    uint32_t range = dst_sudoku->range = sudoku->range;

    if (new)
        dst_sudoku->values = malloc(range*range * sizeof(val_t));
    memcpy(dst_sudoku->values, sudoku->values, range*range * sizeof(val_t));

    if (new)
        dst_sudoku->map = malloc(range*range*range * sizeof(uint8_t));
    memcpy(dst_sudoku->map, sudoku->map, range*range*range * sizeof(uint8_t));

    u32_astack_copy(&(dst_sudoku->to_set), &(sudoku->to_set), new);
}

void
sudoku_load_from_file(sudoku_t *sudoku, char *filepath)
{
    file_buffer fb;
    if (load_file(filepath, &fb)) {
        fprintf(stderr, "ERROR: Failed to load file '%s'!\n", filepath);
        exit(1);
    }

    char *ch = fb.buffer;
    uint32_t width = strtoll(ch, &ch, 10);
    uint32_t height = strtoll(ch, &ch, 10);
    uint32_t range = sudoku->range = width * height;

    sudoku_init(sudoku, width, height);

    // while (ch < fb.buffer + fb.bytes_read) {
    //     uint32_t height = strtoll(ch, &ch, 10);
    // }

    for (int j=0; j < range; ++j) {
        for (int i=0; i < range; ++i) {
            val_t val = strtoll(ch, &ch, 10);
            if (val) {
                sudoku_set_value(sudoku, i, j, val);
            }
        }
    }
}

int
sudoku_apply_to_set(sudoku_t *sudoku)
{
    int return_val; 
    uint32_t val = 0;

    while ( (val = u32_astack_pop(&(sudoku->to_set))) ) {
        uint32_t y = u32_astack_pop(&(sudoku->to_set));
        uint32_t x = u32_astack_pop(&(sudoku->to_set));
    

        if ((return_val = sudoku_set_value(sudoku, x, y, val))) {
            return return_val;
        }
    }

    return 0;
}

bool
sudoku_is_masked(sudoku_t *sudoku, uint32_t x, uint32_t y, uint32_t val)
{
    uint32_t range = sudoku->range;
    return (sudoku->map[y*range*range + x*range + val-1] != 0);
}

int
sudoku_row_unique(sudoku_t *sudoku, uint32_t y, uint32_t val)
{
    uint32_t range = sudoku->range;

    int32_t x = -1;
    for (int j=0; j < range; ++j) {
        // printf("row (%u): %u %u %b\n", val, j, y, sudoku_is_masked(sudoku, j, y, val));
        if (sudoku_get_value(sudoku, j, y) == val)
            return 0;
        if (!sudoku_is_masked(sudoku, j, y, val)) {
            if (x >= 0) 
                return 0;
            x = j;
        }
    }
    
    if (x < 0)
        return 0;

    // printf("Unique-row %u @ %u %u\n", val, x, y);
    return sudoku_set_value(sudoku, x, y, val);
}

int
sudoku_col_unique(sudoku_t *sudoku, uint32_t x, uint32_t val)
{
    uint32_t range = sudoku->range;

    int32_t y = -1;
    for (int j=0; j < range; ++j) {
        // printf("col (%u): %u %u %b\n", val, x, j, sudoku_is_masked(sudoku, x, j, val));
        if (sudoku_get_value(sudoku, x, j) == val)
            return 0;
        if (!sudoku_is_masked(sudoku, x, j, val)) {
            if (y >= 0) 
                return 0;
            y = j;
        }
    }
    
    if (y < 0)
        return 0;

    // sudoku_print(sudoku);

    // printf("Unique-col %u @ %u %u\n", val, x, y);
    return sudoku_set_value(sudoku, x, y, val);
}

int
sudoku_box_unique(sudoku_t *sudoku, uint32_t i, uint32_t val)
{

    uint32_t range = sudoku->range;
    uint32_t width = sudoku->width;
    uint32_t height = sudoku->height;

    uint32_t box_x = i % height;
    uint32_t box_y = i / height;

    int32_t idx = -1;
    uint32_t x, y;
    for (int j=0; j < range; ++j) {
        x = box_x*width + j%width;
        y = box_y*height + j/width;

        // printf("box (%u): %u %u %b\n", val, x, y, sudoku_is_masked(sudoku, x, y, val));
        if (sudoku_get_value(sudoku, x, y) == val)
            return 0;
        if (!sudoku_is_masked(sudoku, x, y, val)) {
            if (idx >= 0) 
                return 0;
            idx = j;
        }
    }

    if (idx < 0)
        return 0;

    x = box_x*width +  idx%width;
    y = box_y*height + idx/width;
    
    // printf("Unique-box %u @ %u %u\n", val, x, y);
    return sudoku_set_value(sudoku, x, y, val);
}



int
sudoku_solve(sudoku_t *sudoku)
{
    int return_val; 
    uint32_t range = sudoku->range;
    uint32_t width = sudoku->width;
    uint32_t height = sudoku->height;


    if ((return_val = sudoku_apply_to_set(sudoku)))
        return return_val;

    sudoku_t sudoku_cpy;
    sudoku_copy(&sudoku_cpy, sudoku, 1);
    
    // Unique
    for (int val=1; val <= range; ++val) {
        for (int i=0; i < range; ++i) {
            int32_t col = -1;
            int32_t box = -1;

            int return_val;
            if ((return_val = sudoku_row_unique(sudoku, i, val))) 
                return return_val;

            if ((return_val = sudoku_col_unique(sudoku, i, val))) 
                return return_val;

            if ((return_val = sudoku_box_unique(sudoku, i, val))) 
                return return_val;
        } 
    } 

    // try_value
    for (int i=0; i<range*range; ++i) {
        if (sudoku->values[i])
            continue;


        for (int j=0; j<range; ++j) {
            if (sudoku->map[i*range + j] != 0)
                continue;
            sudoku_set_value(sudoku, i % range, i / range, j+1);

            // printf("@ %u, set %u\n", i, j+1);
            // sudoku_print(sudoku);
            return_val = sudoku_solve(sudoku);
            // printf(": %d\n", return_val);

            if (return_val < 0) {
                // sudoku_print(sudoku);
                // printf(":!! \n", return_val);

                sudoku_copy(sudoku, &sudoku_cpy, 0);
                continue;
            }

            return return_val;
        }
        return -1;
    }

    free(sudoku_cpy.values);
    free(sudoku_cpy.map);

    return 0;
}

void
sudoku_print(sudoku_t *sudoku)
{
    text_grid grid;
    gird_print_init(&grid, sudoku->height, sudoku->range);

    uint32_t col_width = sudoku->width*2 - 1;
    grid_print_fixed_width(&grid, col_width);
    grid_print_fixed_height(&grid, sudoku->height);

    char *buff = malloc((col_width + 1) 
            * sudoku->range 
            * sudoku->height * sizeof(char));
    
    int line_count = 0;
    char *line = buff;

    uint32_t idx = 0;

    uint32_t range = sudoku->range;
    for (int j=0; j < range; ++j) {
        for (int k=0; k < sudoku->height; ++k) {
            grid.text[line_count++] = line;
    
            for (int i=0; i < sudoku->height; ++i) {
                if (sudoku->values[idx] == 0)
                    *(line++) = '_';
                else
                    *(line++) = '0' + sudoku->values[idx];
                ++idx;
                *(line++) = ' ';
            }
            *(line-1) = '\0';
        }
    }

    grid_print(&grid);
}


int main(int argc, char **argv) {
    sudoku_t sudoku;
    sudoku_load_from_file(&sudoku, "a3.su");

    printf("Initial: \n");
    sudoku_print(&sudoku);
    int k = sudoku_solve(&sudoku);
    printf("%d\n", k);

    printf("Solved: \n");
    sudoku_print(&sudoku);

    free(sudoku.values);
    free(sudoku.map);
}
