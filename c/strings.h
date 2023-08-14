#ifndef STRINGS_H_
#define STRINGS_H_

#include <string.h> // strlen, size_t, NULL
#include <ctype.h> // isspace

/*
* @brief Typedef for a single charactor within a string.
*/
typedef int char_t;

/*
* @brief Definition of a string. Encapsulates a char pointer marked with const along with the length — or "count" — of the data.
*/
typedef struct
{
    size_t count;
    const char *data;
} string_t;

/*
* @brief Macro to allocate a string whose lifetime is that of the entire programme.
*/
#define new_static_string(cstr_lit)\
{\
    sizeof(cstr_lit) - 1,\
    (cstr_lit)\
}

/*
* @brief Macro to represent how a null value within the string structure.
*/
#define string_null new_string(NULL, 0)

/*
* @brief Custom formatting for printing the string structure.
*/
#define string_format "%.*s"

#define string_arg(string) (int) (string).count, (string).data

/*
* @brief Constructor for a new string with a given length and data.
* @param data A char pointer marked with const that is directly added to the structure.
* @param count The provided length of the given data.
* @returns A new string with a given count and data.
*/
string_t new_string(const char *data, size_t count);

/*
* @brief Allocate a new string structure from a c-string literal marked with const.
* @param cstr A char pointer marked with const to be passed to the `new_string` function that was previously defined.
* @returns A new string with data as the given cstr and the `strlen` count.
*/
string_t string_from_literal(const char *cstr);

/*
* @brief Trim a string of its following spaces.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @returns A copy of the same string passed into the function without its following spaces.
*/
string_t string_trim_left(string_t *string);

/*
* @brief Trim a string of its preceeding spaces.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @returns A copy of the same string passed into the function without its preceeding spaces.
*/
string_t string_trim_right(string_t *string);

/*
* @brief Trim a string of both its preceeding and following spaces. This function is a combination of both the `string_trim_left` and the `string_trim_right` functions. This function is similar to the default behaviour of Python's `string.strip()` function.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @returns A copy of the same string passed into the function without its preceeding or following spaces.
*/
string_t string_trim(string_t *string);

/*
* @brief Find the last occurance of a given charactor within a given string.
* @param string A pointer to the string that contains the given charactor. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @param charactor The charactor to find within the given string.
* @returns The index of the given charactor within the given string. Much like the `strrchr` function, if the charactor is not found within the string, 0 is returned.
*/
size_t string_find_last_of(string_t *string, char_t charactor);

#endif // STRINGS_H_

#ifdef STRINGS_IMPLEMENTATION

/*
* @brief Constructor for a new string with a given length and data.
* @param data A char pointer marked with const that is directly added to the structure.
* @param count The provided length of the given data.
* @returns A new string with a given count and data.
*/
string_t new_string(const char *data, size_t count)
{
    string_t string;
    string.count = count;
    string.data = data;
    return string;
}

/*
* @brief Allocate a new string structure from a c-string literal marked with const.
* @param cstr A char pointer marked with const to be passed to the `new_string` function that was previously defined.
* @returns A new string with data as the given cstr and the `strlen` count.
*/
string_t string_from_literal(const char *cstr)
{
    return new_string(cstr, strlen(cstr));
}

/*
* @brief Trim a string of its following spaces.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @returns A copy of the same string passed into the function without its following spaces.
*/
string_t string_trim_left(string_t *string)
{
    size_t i = 0;
    while (i < string->count && isspace(string->data[i]))
    {
        i++;
    }
    return new_string(string->data + i, string->count - i);
}

/*
* @brief Trim a string of its preceeding spaces.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @returns A copy of the same string passed into the function without its preceeding spaces.
*/
string_t string_trim_right(string_t *string)
{
    size_t i = 0;
    while (i < string->count && isspace(string->data[string->count - 1 - i]))
    {
        i++;
    }
    return new_string(string->data, string->count - i);
}

/*
* @brief Trim a string of both its preceeding and following spaces. This function is a combination of both the `string_trim_left` and the `string_trim_right` functions. This function is similar to the default behaviour of Python's `string.strip()` function.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @returns A copy of the same string passed into the function without its preceeding or following spaces.
*/
string_t string_trim(string_t *string)
{
    string_t left_trim = string_trim_left(string);
    return string_trim_right(&left_trim);
}

/*
* @brief Find the last occurance of a given charactor within a given string.
* @param string A pointer to the string that contains the given charactor. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @param charactor The charactor to find within the given string.
* @returns The index of the given charactor within the given string. Much like the `strrchr` function, if the charactor is not found within the string, 0 is returned.
*/
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