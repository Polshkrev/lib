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

#endif // STRING_BUILDER_IMPLEMENTATION