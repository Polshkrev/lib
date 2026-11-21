#ifndef ERROR_H_
#define ERROR_H_


#include <stdbool.h> // bool
#include <stdio.h> // sprintf
#include <stddef.h> // NULL

#define ERROR_NAME_IMPLEMENTATION
#include "name.h"

/**
 * @brief Error state.
 */
typedef struct
{
    ErrorName name;
    const char *message;
} error_t;


/**
 * @brief Initialize a base error with a given message.
 * @param message Message with which to initialize the error.
 * @returns A new stack allocated error.
 */
error_t error_init(const char *message);

/**
 * @brief Initialize a new error based on a given name and message.
 * @param name Name with which to initialize the error.
 * @param message Message with which to initialize the error.
 * @returns A new stack allocated error with a given name and message.
 */
error_t error_init_with_name(ErrorName name, const char *message);

/**
 * @brief Determine if the given error is of a given name.
 * @param error Error whose name is checked against.
 * @param name Name with which to check the error's identity.
 * @returns True if the error has the given name, else false.
 */
bool error_is(error_t error, ErrorName name);

/**
 * @brief Determine if the error has not been changed.
 * @returns True if the error has not been changed, else false.
 */
bool error_ok(void);

/**
 * @brief Set the global error state.
 * @param name Name to given the raised error.
 * @param message Message to give to the error.
 */
void error_raise(ErrorName name, const char *message);

/**
 * @brief Raise an error and exit with an exit code of one.
 * @param stream Stream to which to print the error.
 * @param name Name to given the raised error.
 * @param message Message to give to the error.
 */
#define panic(stream, name, message) \
do \
{\
    _raise(stream, name, message, __FILE__, __LINE__); \
    exit(1);\
} while(0)

/**
 * @brief Print the error to a given stream.
 * @param stream Stream to which to print the error.
 */
#define fail(stream) __fail((stream), __FILE__, __LINE__)
#endif // ERROR_H_

#ifdef ERROR_IMPLEMENTATION

/**
 * @brief Global error state. The initial state of the error has a name of `Ok`, and a NULL message.
 */
static error_t __error = {0};

#ifndef __ERROR_BUFFER_SIZE
    /**
     * @brief Initial size of the error message buffer.
     */
    #define __ERROR_BUFFER_SIZE 256
#endif // __ERROR_BUFFER_SIZE

/**
 * @brief Error message buffer.
 */
char __error_buffer[__ERROR_BUFFER_SIZE] = {0};

/**
 * @brief Initialize a base error with a given message.
 * @param message Message with which to initialize the error.
 * @returns A new stack allocated error.
 */
error_t error_init(const char *message)
{
    return error_init_with_name(ErrorBase, message);
}

/**
 * @brief Initialize a new error based on a given name and message.
 * @param name Name with which to initialize the error.
 * @param message Message with which to initialize the error.
 * @returns A new stack allocated error with a given name and message.
 */
error_t error_init_with_name(ErrorName name, const char *message)
{
    return (error_t)
    {
        .name = name,
        .message = message
    };
}

/**
 * @brief Determine if the given error is of a given name.
 * @param error Error whose name is checked against.
 * @param name Name with which to check the error's identity.
 * @returns True if the error has the given name, else false.
 */
bool error_is(error_t error, ErrorName name)
{
    return error.name == name;
}

/**
 * @brief Determine if the error has not been changed.
 * @returns True if the error has not been changed, else false.
 */
bool error_ok(void)
{
    return error_is(__error, ErrorOk);
}

/**
 * @brief Set the global error state.
 * @param name Name to given the raised error.
 * @param message Message to give to the error.
 */
void error_raise(ErrorName name, const char *message)
{
    __error = error_init_with_name(name, message);
}

/**
 * @brief Represent the global error state as a c-string given a file path and line number.
 * @param file File path where the error occured.
 * @param line Line number where the error occured.
 * @returns The global error state as a c-string.
 */
const char *__errtostr(const char *file, int line)
{
    sprintf(__error_buffer, "%s(%d) %s: %s", file, line, error_name_to_string(__error.name), __error.message ? __error.message : "");
    return __error_buffer;
}

/**
 * @brief Print the global error state to a given stream.
 * @param stream Stream to which to print the error.
 * @param file File path where the error occured.
 * @param line Line number where the error occured.
 */
void __fail(FILE *stream, const char *file, int line)
{
    if (error_ok()) return;
    fprintf(stream, "%s\n", __errtostr(file, line));
}

/**
 * @brief Set the global error state and print it to the given stream.
 * @param stream Stream to which to print the error.
 * @param name Name to given the raised error.
 * @param message Message to give to the error.
 * @param file File path where the error occured.
 * @param line Line number where the error occured.
 */
void _raise(FILE *stream, ErrorName name, const char *message, const char *file, int line)
{
    error_raise(name, message);
    __fail(stream, file, line);
}


#endif // ERROR_IMPLEMENTATION
