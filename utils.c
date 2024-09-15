
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#include "utils.h"

void 
darray_init(darray_t *array, size_t element_size, size_t max_num_of_elements)
{
    array->element_size = element_size;
    array->num_of_elements = 0;

    array->max_num_of_elements = max_num_of_elements;
    array->alloc_size = element_size * max_num_of_elements;
    array->values = malloc(array->alloc_size);
}

darray_t
darray_copy(const darray_t *array, size_t num_of_elements)
{
    darray_t new_array; 
    if (num_of_elements <= 0) 
        num_of_elements = array->max_num_of_elements;

    darray_init(&new_array, array->element_size, num_of_elements);
    darray_copy_to(&new_array, array);

    return new_array;
}

void 
darray_copy_to(darray_t *dst_array, const darray_t *src_array)
{
    assert(dst_array->alloc_size >= src_array->alloc_size);

    dst_array->element_size = src_array->element_size;
    dst_array->num_of_elements = src_array->num_of_elements;
    size_t used_size = src_array->element_size * src_array->num_of_elements;
    memcpy(dst_array->values, src_array->values, used_size);
}

void 
darray_enlarge(darray_t *array, size_t num_of_elements)
{
    size_t new_size = array->element_size * num_of_elements;
    if (array->alloc_size < new_size) {
        array->values = realloc(array->values, new_size);
        array->alloc_size = new_size;
    }
}

inline
void *
darray_end(darray_t *array)
{
    return array->values + array->num_of_elements * array->element_size;
}

inline
void *
darray_at_index(darray_t *array, size_t it) 
{
    // TODO: Check 0 <= it < num_of_elements
    assert(0 <= it && it < array->num_of_elements);
    return array->values + it * array->element_size;
}

void 
darray_push(darray_t *array, uint8_t *value)
{
    if (array->max_num_of_elements < array->max_num_of_elements + 1) {
        size_t new_max = array->max_num_of_elements + \
                            array->max_num_of_elements / 2 + 1;
        darray_enlarge(array, new_max);
    }

    void *dst = darray_end(array);
    memcpy(dst, value, array->element_size);
}

void 
darray_append(darray_t *array_a, darray_t *array_b)
{
    assert(array_a->element_size == array_b->element_size);

    darray_enlarge(array_a, array_a->alloc_size + array_b->alloc_size);

    void *dst = darray_end(array_a);
    memcpy(dst, array_b->values, array_b->alloc_size);
}




void
u32_astack_init(u32_astack_t *stack, const uint32_t init_capacity)
{
    stack->size = 0;
    stack->capacity = init_capacity;
    stack->values = (uint32_t *) malloc(init_capacity*sizeof(uint32_t));
}

void
u32_astack_copy(u32_astack_t *dst_stack, const u32_astack_t *stack, int new)
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
load_file(const char *filepath, file_buffer *buff)
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
