#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


typedef struct 
{
    char *buffer;
    uint32_t buffer_size;
    uint32_t bytes_read;
} file_buffer;

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
