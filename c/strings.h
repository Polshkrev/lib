#ifndef STRINGS_H_
#define STRINGS_H_

#include <stddef.h> // size_t

typedef struct
{
    size_t count;
    const char *data;
} string_t;

#endif // STRINGS_H_