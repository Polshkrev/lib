#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include <stddef.h> // size_t
#include <stdio.h> // printf, fprintf, stdout, stderr
#include <string.h> // strerror
#include <errno.h> // errno
#include <stdlib.h> // malloc, realloc, exit

#define ARRAY_INITIAL_CAPACITY 256

typedef struct
{
    const char **items;
    size_t size;
    size_t capacity;
} string_buffer_t;

string_buffer_t string_buffer_init();
void string_buffer_append(string_buffer_t *buffer, const char *item);
const char *string_buffer_at(string_buffer_t buffer, size_t index);
void string_buffer_remove(const string_buffer_t *buffer, size_t index);
void string_buffer_print(string_buffer_t buffer);
void string_buffer_delete(string_buffer_t *buffer);

#endif // STRING_BUFFER_H

#ifdef STRING_BUFFER_IMPLEMENTATION

static void _string_buffer_fill(string_buffer_t *buffer, size_t capacity)
{
    buffer->capacity = capacity;
    buffer->size = 0;
    buffer->items = malloc((buffer->capacity * sizeof(const char *)));
    if (NULL == buffer->items)
    {
        fprintf(stderr, "BadAllocationError: Cannot allocate memory: %s\n", strerror(errno));
        exit(1);
    }
}

string_buffer_t string_buffer_init()
{
    string_buffer_t buffer = {0};
    _string_buffer_fill(&buffer, ARRAY_INITIAL_CAPACITY);
    return buffer;
}

void string_buffer_append(string_buffer_t *buffer, const char *item)
{
    if (buffer->size >= buffer->capacity)
    {
        buffer->items = realloc(buffer->items, ((buffer->capacity * 2) * sizeof(const char *)));
    }
    buffer->items[buffer->size++] = item;
}

const char *string_buffer_at(string_buffer_t buffer, size_t index)
{
    if (index > buffer.capacity)
    {
        fprintf(stderr, "IndexError: Cannot acces buffer with capcaity %d at %d\n", (int)buffer.capacity, (int)index);
        exit(1);
    }
    return buffer.items[index];
}

void string_buffer_remove(const string_buffer_t *buffer, size_t index)
{
    if (index > buffer->capacity)
    {
        fprintf(stderr, "IndexError: Cannot acces buffer at %d\n", (int)index);
        exit(1);
    }
    else if (NULL == buffer->items[index])
    {
        fprintf(stderr, "IndexError: Buffer at %d\n is already removed or not filled.", (int)index);
        exit(1);
    }
    buffer->items[index] = NULL;
}

void string_buffer_print(string_buffer_t buffer)
{
    for (size_t i = 0; i < buffer.size; ++i)
    {
        if (NULL == buffer.items[i])
        {
            continue;
        }
        printf("%s", buffer.items[i]);
    }
}

void string_buffer_delete(string_buffer_t *buffer)
{
    if (!(buffer->items))
    {
        return;
    }
    free(buffer->items);
    buffer->items = NULL;
    buffer->capacity = 0;
    buffer->size = 0;
}

#endif // STRING_BUFFER_IMPLEMENTATION