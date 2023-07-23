#ifndef STRINGS_H_
#define STRINGS_H_

#include <stddef.h> // size_t

typedef int char_t;

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

string_t new_string(const char *data, size_t count);
string_t string_from_literal(const char *cstr);
string_t string_trim_left(string_t *string);
string_t string_trim_right(string_t *string);
string_t string_trim(string_t *string);
size_t string_find_last_of(string_t *string, char_t charactor);

#endif // STRINGS_H_

#ifdef STRINGS_IMPLEMENTATION



#endif // STRINGS_IMPLEMENTATION