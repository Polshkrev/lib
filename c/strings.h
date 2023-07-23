#ifndef STRINGS_H_
#define STRINGS_H_

#include <stddef.h> // size_t

typedef struct
{
    size_t count;
    const char *data;
} string_t;

#define new_static_string(cstr_lit)\
{\
    sizeof(cstr_lit) - 1,\
    (cstr_lit)\
}

#define string_null new_string(NULL, 0)

#define string_format "%.*s"
#define string_arg(string) (int) (string).count, (string).data

#endif // STRINGS_H_