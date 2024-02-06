#ifndef STRING_BUILDER_H_
#define STRING_BUILDER_H_

#include <stddef.h> // size_t
#include <stdio.h> // fprintf
#include <stdlib.h> // malloc, realloc, NULL, exit

#ifndef STRING_BUILDER_CAPACITY
#define STRING_BUILDER_CAPACITY 56ULL
#endif // STRING_BUILDER_CAPACITY

typedef struct
{
    size_t size;
    size_t capacity;
    char *items;
} string_builder_t;

string_builder_t *string_builder_init();
void string_builder_append(string_builder_t *builder, char item);
const char string_builder_at(const string_builder_t *builder, size_t index);
const char *string_builder_data(const string_builder_t *builder);
void string_builder_delete(string_builder_t *builder);

#endif // STRING_BUILDER_H_

#ifdef STRING_BUILDER_IMPLEMENTATION

string_builder_t *string_builder_init()
{
    string_builder_t *builder = (string_builder_t *)malloc(sizeof(string_builder_t));
    if (NULL == builder)
    {
        fprintf(stderr, "AllocationError: Can not allocate enough memory to build a string.\n");
        exit(1);
    }
    builder->capacity = STRING_BUILDER_CAPACITY;
    builder->size = 0;
    builder->items = (char *)malloc(sizeof(char) * STRING_BUILDER_CAPACITY);
    if (NULL == builder->items)
    {
        fprintf(stderr, "AllocationError: Can not allocate enough memory to build a string array.\n");
        exit(1);
    }
    return builder;
}

void string_builder_append(string_builder_t *builder, char item)
{
    if (builder->size >= builder->capacity)
    {
        builder->capacity *= 2;
        builder->items = (char *)realloc(builder->items, builder->capacity);
        if (NULL == builder->items)
        {
            fprintf(stderr, "AllocationError: Can not rallocate array.\n");
            exit(1);
        }
    }
    builder->items[builder->size++] = item;
}

const char string_builder_at(const string_builder_t *builder, size_t index)
{
    if (index > builder->size)
    {
        fprintf(stderr, "IndexError: Can not access an element outside of array.\n");
        exit(1);
    }
    return builder->items[index];
}

const char *string_builder_data(const string_builder_t *builder)
{
    return builder->items;
}

void string_builder_delete(string_builder_t *builder)
{
    if (!(builder))
    {
        return;
    }
    builder->size = 0;
    builder->capacity = 0;
    if (!(builder->items))
    {
        return;
    }
    free(builder->items);
    builder->items = NULL;
    builder = NULL;
}

#endif // STRING_BUILDER_IMPLEMENTATION