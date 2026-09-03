#ifndef BUFFER_H
#define BUFFER_H

// https://github.com/tsoding/nob.h/blob/main/nob.h#L604

#include <stddef.h> // size_t

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Allocate a new region of memory.
 * @param size Additional size — in bytes — to allocate.
 * @returns A pointer to the allocated region of memory.
 * @returns If the given size delta is greater than the capacity, `NULL` is returned.
 */
void *buffer_allocate(size_t size);

/**
 * @brief Duplicate a string into a static memory buffer.
 * @param string String from which to copy.
 * @returns A duplicate of the original given string.
 * @exception If the internal allocation is `NULL`, an `AllocationError` is printed `stderr` and the programme exits.
 */
char *buffer_duplicate(const char *string);

/**
 * @brief Obtain a pointer to a formated string.
 * @param format String format.
 * @returns A pointer to a new formated string.
 * @exception If the total amount of bytes written is less than zero, an `AllocationError` is printed `stderr` and the programme exits.
 * @exception If the internal allocation is `NULL`, an `AllocationError` is printed `stderr` and the programme exits.
 */
char *buffer_sprintf(const char *format, ...);

/**
 * @brief Save the current position of the buffers cursor.
 * @returns The cursor.
 */
size_t buffer_save(void);

/**
 * @brief Set the buffer to a new checkpoint.
 * @param checkpoint Point to wich to rewind the buffer.
 */
void buffer_rewind(size_t checkpoint);

/**
 * @brief Reset the allocated region of memeory.
 */
void buffer_reset(void);

#if defined(__cplusplus)
}
#endif

#endif // BUFFER_H

#ifdef BUFFER_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdlib.h> // NULL, exit
#include <stdio.h> // fprintf, vsnprintf, stderr
#include <string.h> // memcpy, strlen
#include <stdarg.h> // va_list, va_start, va_end

#ifndef TEMP_CAPACITY
#define TEMP_CAPACITY (5*1024)
#endif // TEMP_CAPACITY

static size_t __temp_size = 0;
static char __temp_buffer[TEMP_CAPACITY] = {0};

/**
 * @brief Allocate a new region of memory.
 * @param size Additional size — in bytes — to allocate.
 * @returns A pointer to the allocated region of memory.
 * @returns If the given size delta is greater than the capacity, `NULL` is returned.
 */
void *buffer_allocate(size_t size)
{
    if (__temp_size + size > TEMP_CAPACITY)
    {
        return NULL;
    }
    void *result = &__temp_buffer[__temp_size];
    __temp_size += size;
    return result;
}

/**
 * @brief Duplicate a string into a static memory buffer.
 * @param string String from which to copy.
 * @returns A duplicate of the original given string.
 * @exception If the internal allocation is `NULL`, an `AllocationError` is printed `stderr` and the programme exits.
 */
char *buffer_duplicate(const char *string)
{
    size_t length = strlen(string);
    char *result = (char *)buffer_allocate(length + 1);
    if (NULL == result)
    {
        fprintf(stderr, "AllocationError: Can not allocate any more memory.\n");
        exit(1);
    }
    memcpy(result, string, length);
    result[length] = '\0';
    return result;
}

/**
 * @brief Obtain a pointer to a formated string.
 * @param format String format.
 * @returns A pointer to a new formated string.
 * @exception If the total amount of bytes written is less than zero, an `AllocationError` is printed `stderr` and the programme exits.
 * @exception If the internal allocation is `NULL`, an `AllocationError` is printed `stderr` and the programme exits.
 */
char *buffer_sprintf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int n = vsnprintf(NULL, 0, format, args);
    va_end(args);
    if (n < 0)
    {
        fprintf(stderr, "AllocationError: 'vsnprintf' has failed.");
        exit(1);
    }
    char *result = (char *)buffer_allocate(n + 1);
    if (NULL == result)
    {
        fprintf(stderr, "AllocationError: Extend the size of the buffer.\n");
        exit(1);
    }
    va_start(args, format);
    vsnprintf(result, n + 1, format, args);
    va_end(args);
    return result;
}

/**
 * @brief Save the current position of the buffers cursor.
 * @returns The cursor.
 */
size_t buffer_save(void)
{
    return __temp_size;
}

/**
 * @brief Set the buffer to a new checkpoint.
 * @param checkpoint Point to wich to rewind the buffer.
 */
void buffer_rewind(size_t checkpoint)
{
    __temp_size = checkpoint;
}

/**
 * @brief Reset the allocated region of memeory.
 */
void buffer_reset(void)
{
    __temp_size = 0;
}

#if defined(__cplusplus)
}
#endif

#endif // BUFFER_IMPLEMENTATION