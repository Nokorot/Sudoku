#ifndef UTILS_H_
#define UTILS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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
