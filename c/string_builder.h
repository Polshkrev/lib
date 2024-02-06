#ifndef STRING_BUILDER_H_
#define STRING_BUILDER_H_

#include <stddef.h>

typedef struct
{
    size_t size;
    size_t capacity;
    char *items;
} string_builder_t;

#endif // STRING_BUILDER_H_