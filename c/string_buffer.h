#ifndef STRING_BUFFER_H_
#define STRING_BUFFER_H_

#include <stddef.h> // size_t
#include <stdio.h> // printf, fprintf, stdout, stderr
#include <string.h> // strerror
#include <errno.h> // errno
#include <stdlib.h> // malloc, realloc, exit

#define ARRAY_INITIAL_CAPACITY 256

static char __tmp_buffer[ARRAY_INITIAL_CAPACITY];

/*
* @brief Classical dynamic array of strings.
*/
typedef struct
{
    const char **items;
    size_t size;
    size_t capacity;
} string_buffer_t;

/*
* @brief Initialize a new string buffer.
* @returns A new heap allocated structure.
*/
string_buffer_t string_buffer_init();

/*
* @brief Append a single element to the buffer.
* @param buffer A pointer to the specific buffer to which to append.
* @param item Item from which to append to the buffer.
*/
void string_buffer_append(string_buffer_t *buffer, const char *item);

/*
* @brief Retrieve the underlying data in the buffer at a given index.
* @param buffer Specific buffer from which to read.
* @param index Index in the buffer where the underlying data is located.
* @returns String data at the given index.
*/
const char *string_buffer_at(string_buffer_t buffer, size_t index);

/*
* @brief Flatten the two-dimensional string array into one constant contiguous string.
* @param buffer The buffer to flatten.
* @returns A string representation of the data within the dynamic array.
*/
const char *string_buffer_data(string_buffer_t buffer);

/*
* @brief Remove a string in the buffer at a given index.
* @param buffer Buffer where the data is located.
* @param index Index within the buffer where the data is located.
*/
void string_buffer_remove(const string_buffer_t *buffer, size_t index);

/*
* @brief Print the buffer.
* @param buffer Buffer from which to print.
*/
void string_buffer_print(string_buffer_t buffer);

/*
* @brief Deallocate the buffer.
* @param buffer Buffer to deallocate.
*/
void string_buffer_delete(string_buffer_t *buffer);

#endif // STRING_BUFFER_H_

#ifdef STRING_BUFFER_IMPLEMENTATION

/*
* @brief Internal way of allocated the item array.
* @param buffer Buffer to fill-up.
* @param capacity Capacity to "cap-off" the array.
*/
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

/*
* @brief Initialize a new string buffer.
* @returns A new heap allocated structure.
*/
string_buffer_t string_buffer_init()
{
    string_buffer_t buffer = {0};
    _string_buffer_fill(&buffer, ARRAY_INITIAL_CAPACITY);
    return buffer;
}

/*
* @brief Append a single element to the buffer.
* @param buffer A pointer to the specific buffer to which to append.
* @param item Item from which to append to the buffer.
*/
void string_buffer_append(string_buffer_t *buffer, const char *item)
{
    if (buffer->size >= buffer->capacity)
    {
        buffer->items = realloc(buffer->items, ((buffer->capacity * 2) * sizeof(const char *)));
    }
    buffer->items[buffer->size++] = item;
}

/*
* @brief Retrieve the underlying data in the buffer at a given index.
* @param buffer Specific buffer from which to read.
* @param index Index in the buffer where the underlying data is located.
* @returns String data at the given index.
*/
const char *string_buffer_at(string_buffer_t buffer, size_t index)
{
    if (index > buffer.capacity)
    {
        fprintf(stderr, "IndexError: Cannot acces buffer with capcaity %d at %d\n", (int)buffer.capacity, (int)index);
        exit(1);
    }
    return buffer.items[index];
}

/*
* @brief Internal way of splitting responsibilities from appending once from many. Simmilar to the way `strcat` works
* @param destination Destination buffer to which to append.
* @param source Source buffer from which to add to the destination.
* @returns A pointer to a new appended string buffer.
*/
const char *__buffer_append(const char *destination, const char *source)
{
    size_t dest_len = strlen(destination);
    size_t source_len = strlen(source);
    size_t full_length = (dest_len + source_len) + 1;
    char *string = __tmp_buffer;
    for (size_t i = 0; i < dest_len; ++i)
    {
        string[i] = destination[i];
    }
    for (size_t j = 0; j < source_len; ++j)
    {
        string[dest_len + j] = source[j];
    }
    string[full_length - 1] = '\0';
    return string;
}

/*
* @brief Flatten the two-dimensional string array into one constant contiguous string.
* @param buffer The buffer to flatten.
* @returns A string representation of the data within the dynamic array.
*/
const char *string_buffer_data(string_buffer_t buffer)
{
    char *string = __buffer_append(buffer.items[0], buffer.items[1]);
    for (size_t i = 2; i < buffer.size; ++i)
    {
        __buffer_append(string, buffer.items[i]);
    }
    return string;
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

/*
* @brief Remove a string in the buffer at a given index.
* @param buffer Buffer where the data is located.
* @param index Index within the buffer where the data is located.
*/
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

/*
* @brief Print the buffer.
* @param buffer Buffer from which to print.
*/
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

/*
* @brief Deallocate the buffer.
* @param buffer Buffer to deallocate.
*/
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