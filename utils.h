#ifndef UTILS_H_
#define UTILS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t element_size;
    size_t num_of_elements;
    size_t max_num_of_elements;

    size_t alloc_size; // = element_size * max_num_of_elements
    uint8_t *values;
} darray_t;

void darray_init(darray_t *array, size_t element_size, size_t num_of_elements);

void darray_copy_to(darray_t *dst_array, const darray_t *src_array);

// if num_of_elements <= 0, copy all elements
// if num_of_elements < array->num_of_elements, only the first 
//          num_of_elements elements are copied
// if num_of_elements > array->num_of_elements, 
//          the new array will have max_num_of_elements = num_of_elements
darray_t darray_copy(const darray_t *src_array, size_t num_of_elements);  

void darray_enlarge(darray_t *array, size_t num_of_elements);
void darray_push(darray_t *array, uint8_t *value);

void darray_append(darray_t *array_a, darray_t *array_b);

inline void *darray_at_index(darray_t *array, size_t it);
inline void *darray_end(darray_t *array);

typedef struct {
    size_t size;
    size_t capacity;
    uint32_t *values;
} u32_astack_t;

void u32_astack_init(u32_astack_t *stack, const uint32_t init_size);
void u32_astack_copy(u32_astack_t *dst_stack, const u32_astack_t *stack, int _new);
void u32_astack_push(u32_astack_t *stack, const uint32_t value);
uint32_t u32_astack_pop(u32_astack_t *stack);


typedef struct 
{
    char *buffer;
    uint32_t buffer_size;
    uint32_t bytes_read;
} file_buffer;

int load_file(const char *filepath, file_buffer *buff);

#endif
