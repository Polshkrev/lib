#ifndef STRINGS_H_
#define STRINGS_H_

#include <string.h> // strlen, size_t, NULL
#include <ctype.h> // isspace

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

string_t new_string(const char *data, size_t count)
{
    string_t string;
    string.count = count;
    string.data = data;
    return string;
}

string_t string_from_literal(const char *cstr)
{
    return new_string(cstr, strlen(cstr));
}

string_t string_trim_left(string_t *string)
{
    size_t i = 0;
    while (i < string->count && isspace(string->data[i]))
    {
        i++;
    }
    return new_string(string->data + i, string->count - i);
}

string_t string_trim_right(string_t *string)
{
    size_t i = 0;
    while (i < string->count && isspace(string->data[string->count - 1 - i]))
    {
        i++;
    }
    return new_string(string->data, string->count - i);
}

string_t string_trim(string_t *string)
{
    string_t left_trim = string_trim_left(string);
    return string_trim_right(&left_trim);
}

size_t string_find_last_of(string_t *string, char_t charactor)
{
    char *rest = strrchr(string->data, charactor);
    if (NULL == rest)
    {
        return 0;
    }
    return rest - string->data;
}

#endif // STRINGS_IMPLEMENTATION