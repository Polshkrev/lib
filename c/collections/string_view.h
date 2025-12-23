#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h> // size_t
#include <stdbool.h> // bool

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
#define static_string_new(cstr_lit)\
{\
    sizeof(cstr_lit) - 1,\
    (cstr_lit)\
}

/*
* @brief Macro to represent how a null value within the string structure.
*/
#define string_null string_new(NULL, 0)

/*
* @brief Custom formatting for printing the string structure.
*/
#define string_format "%.*s"

#define string_argument(string) (int) (string).count, (string).data

/*
* @brief Constructor for a new string with a given length and data.
* @param data A char pointer marked with const that is directly added to the structure.
* @param count The provided length of the given data.
* @returns A new string with a given count and data.
*/
string_t string_new(const char *data, size_t count);

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
string_t string_trim_left(const string_t *string);

/*
* @brief Trim a string of its preceeding spaces.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @returns A copy of the same string passed into the function without its preceeding spaces.
*/
string_t string_trim_right(const string_t *string);

/*
* @brief Trim a string of both its preceeding and following spaces. This function is a combination of both the `string_trim_left` and the `string_trim_right` functions. This function is similar to the default behaviour of Python's `string.strip()` function.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @returns A copy of the same string passed into the function without its preceeding or following spaces.
*/
string_t string_trim(const string_t *string);

/*
* @brief Chop a string by a given delimetre.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @param delimetre A charactor by which to split the string.
* @returns A string where the data is set to the first chunk of string before the given delimetre.
*/
string_t string_chop_by_delimetre(string_t *string, char delimetre);

/*
* @brief Find the first occurance of a given charactor within a given string.
* @param string A pointer to the string that contains the given charactor. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @param charactor The charactor to find within the given string.
* @returns The index of the given charactor within the given string. Much like the `strchr` function, if the charactor is not found within the string, 0 is returned.
*/
size_t string_find_first_of(const string_t *string, char_t charactor);

/*
* @brief Find the last occurance of a given charactor within a given string.
* @param string A pointer to the string that contains the given charactor. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @param charactor The charactor to find within the given string.
* @returns The index of the given charactor within the given string. Much like the `strrchr` function, if the charactor is not found within the string, 0 is returned.
*/
size_t string_find_last_of(const string_t *string, char_t charactor);

/*
* @brief Determine whether a given string starts with a given expected string.
* @param string A pointer to the string that contains the given string. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @param expected The expected prefix to check against.
* @returns A boolean on whether `string` starts with `expected`.
*/
bool string_starts_with(const string_t *string, string_t expected);

/*
* @brief Determine whether a given string ends with a given expected string.
* @param string A pointer to the string that contains the given string. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @param expected The expected suffix to check against.
* @returns A boolean on whether `string` ends with `expected`.
*/
bool string_ends_with(const string_t *string, string_t expected);

/*
* @brief Compare two given — case sensitive — string types.
* @param a The string to check against.
* @param b The string to which a comparison will be made in parametre `a`.
* @returns A boolean on wheather `a` and `b` are equal.
*/
bool string_equals(string_t a, string_t b);

#endif // STRING_VIEW_H

#ifdef STRING_VIEW_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#include <string.h> // strlen, strchr, strrchr, memcmp, NULL
#include <ctype.h> // isspace

/*
* @brief Constructor for a new string with a given length and data.
* @param data A char pointer marked with const that is directly added to the structure.
* @param count The provided length of the given data.
* @returns A new string with a given count and data.
*/
string_t string_new(const char *data, size_t count)
{
    string_t string = {0};
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
    return string_new(cstr, strlen(cstr));
}

/*
* @brief Trim a string of its following spaces.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @returns A copy of the same string passed into the function without its following spaces.
*/
string_t string_trim_left(const string_t *string)
{
    size_t i = 0;
    while (i < string->count && isspace(string->data[i]))
    {
        i++;
    }
    return string_new(string->data + i, string->count - i);
}

/*
* @brief Trim a string of its preceeding spaces.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @returns A copy of the same string passed into the function without its preceeding spaces.
*/
string_t string_trim_right(const string_t *string)
{
    size_t i = 0;
    while (i < string->count && isspace(string->data[string->count - 1 - i]))
    {
        i++;
    }
    return string_new(string->data, string->count - i);
}

/*
* @brief Trim a string of both its preceeding and following spaces. This function is a combination of both the `string_trim_left` and the `string_trim_right` functions. This function is similar to the default behaviour of Python's `string.strip()` function.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @returns A copy of the same string passed into the function without its preceeding or following spaces.
*/
string_t string_trim(const string_t *string)
{
    string_t left_trim = string_trim_left(string);
    return string_trim_right(&left_trim);
}

/*
* @brief Chop a string by a given delimetre.
* @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @param delimetre A charactor by which to split the string.
* @returns A string where the data is set to the first chunk of string before the given delimetre.
*/
string_t string_chop_by_delimetre(string_t *string, char delimetre)
{
    size_t i = 0;
    while (i < string->count && string->data[i] != delimetre)
    {
        i++;
    }
    string_t result = string_new(string->data, i);
    if (i < string->count)
    {
        string->count -= i + 1;
        string->data += i + 1;
    }
    else
    {
        string->count -= i;
        string->data += i;
    }
    return result;
}

/*
* @brief Find the first occurance of a given charactor within a given string.
* @param string A pointer to the string that contains the given charactor. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @param charactor The charactor to find within the given string.
* @returns The index of the given charactor within the given string. Much like the `strchr` function, if the charactor is not found within the string, 0 is returned.
*/
size_t string_find_first_of(const string_t *string, char_t charactor)
{
    const char *result = strchr(string->data, charactor);
    const size_t full_length = strlen(string->data);
    if (!result)
    {
        return 0;
    }
    return full_length - strlen(result);
}

/*
* @brief Find the last occurance of a given charactor within a given string.
* @param string A pointer to the string that contains the given charactor. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @param charactor The charactor to find within the given string.
* @returns The index of the given charactor within the given string. Much like the `strrchr` function, if the charactor is not found within the string, 0 is returned.
*/
size_t string_find_last_of(const string_t *string, char_t charactor)
{
    const char *result = strrchr(string->data, charactor);
    const size_t full_length = strlen(string->data);
    if (!result)
    {
        return 0;
    }
    return full_length - strlen(result);
}

/*
* @brief Determine whether a given string starts with a given expected string.
* @param string A pointer to the string that contains the given string. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @param expected The expected prefix to check against.
* @returns A boolean on whether `string` starts with `expected`.
*/
bool string_starts_with(const string_t *string, string_t expected)
{
    if (expected.count > string->count)
    {
        return false;
    }
    string_t actual = string_new(string->data, expected.count);
    return string_equals(expected, actual);
}

/*
* @brief Determine whether a given string ends with a given expected string.
* @param string A pointer to the string that contains the given string. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
* @param expected The expected suffix to check against.
* @returns A boolean on whether `string` ends with `expected`.
*/
bool string_ends_with(const string_t *string, string_t expected)
{
    if (expected.count > string->count)
    {
        return false;
    }
    string_t actual = string_new(string->data + string->count - expected.count, expected.count);
    return string_equals(expected, actual);
}

/*
* @brief Compare two given — case sensitive — string types.
* @param a The string to check against.
* @param b The string to which a comparison will be made in parametre `a`.
* @returns A boolean on wheather `a` and `b` are equal.
*/
bool string_equals(string_t a, string_t b)
{
    if (a.count != b.count)
    {
        return false;
    }
    return memcmp(a.data, b.data, a.count) == 0;
}

#if defined(__cplusplus)
}
#endif

#endif // STRING_VIEW_IMPLEMENTATION