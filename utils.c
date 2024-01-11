
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"

void
u32_astack_init(u32_astack_t *stack, const uint32_t init_capacity)
{
    stack->size = 0;
    stack->capacity = init_capacity;
    stack->values = (uint32_t *) malloc(init_capacity*sizeof(uint32_t));
}

void
u32_astack_copy(u32_astack_t *dst_stack, u32_astack_t *stack, int new)
{
    dst_stack->size = stack->size;
    dst_stack->capacity = stack->capacity;
    if (new)
        dst_stack->values = (uint32_t *) malloc(stack->capacity*sizeof(uint32_t));
    memcpy(dst_stack->values, stack->values, stack->size * sizeof(uint32_t) );
}

void
u32_astack_push(u32_astack_t *stack, const uint32_t value)
{
    if (stack->size + 1 > stack->capacity) {
        stack->values = (uint32_t *) realloc(stack->values, 
                (1 + stack->size + stack->size / 2)*sizeof(uint32_t));
        stack->capacity = (1 + stack->size + stack->size / 2);
    }

    stack->values[stack->size++] = value;
}

uint32_t
u32_astack_pop(u32_astack_t *stack)
{
    if (stack->size == 0)
        return 0;

    return stack->values[--stack->size];
}

int
load_file(char *filepath, file_buffer *buff)
{
    FILE *file;
    int64_t file_size;
    int64_t file_head;

    file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s'\n", filepath);
        return -1;
    }

    file_head = ftell(file);
    fseek(file, 0, SEEK_END);

    file_size = ftell(file);
    fseek(file, file_head, SEEK_SET);

    buff->buffer = (char *) malloc(file_size * sizeof(char));
    buff->buffer_size = file_size;

    buff->bytes_read = fread(buff->buffer, 1, file_size, file);

    fclose(file);
    return 0;
}
