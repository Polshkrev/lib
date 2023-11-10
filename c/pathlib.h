#ifndef PATHLIB_H_
#define PATHLIB_H_

#ifdef _WIN32
#include <direct.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define PATH_SEPERATOR "\\"
#else
#define PATH_SEPERATOR "/"
#include <sys/types.h>
#endif // _WIN32

#include <stdbool.h>
#include <errno.h>

#include <stdio.h>

#ifndef MAX_PATH_SIZE
#define MAX_PATH_SIZE 256
#endif // MAX_PATH_SIZE

char __path_buffer[MAX_PATH_SIZE];

/*
* @brief Representation of a system path.
* @property raw A string, marked with const, that holds the pointer to raw path data.
*/
typedef struct
{
    const char *raw;
} path_t;

/*
* @brief Initialize a new path. The underlying path data is initialized to NULL after using this function.
* @returns A new, 0-initialized, Path.
*/
path_t path_new();

/*
* @brief Initialize a new path.
* @param path String literal to initialize the path object.
* @returns A new Path.
*/
path_t path_from(const char *path);

/*
* @brief Determine whether a given path exists. On Windows, the `windows.h` header is used.
* @param path String literal, marked with const, to check against whether it exists.
* @returns Whether the path exists.
*/
bool path_exists(const char *path);

/*
* @brief Get the absolute value of a given path.
* @param path Relative path to extract the absolute.
* @returns A new path with the absolute path of the realtive given path.
*/
path_t pasb(const path_t path);

/*
* @brief Represent a path as a string.
* @param path Read-Only Path to represent.
* @returns The underlying path data within the given object.
*/
const char *passtr(const path_t path);

/*
* @brief Append a given path to another.
* @param destination Base path to which to append.
* @param source Path from which to be copied into the destination.
* @returns A new path whose raw data is of the two concatenated parametres.
*/
path_t path_append(const path_t destination, const path_t source);

/*
* @brief Append a given string to a path.
* @param destination Base path to which to append.
* @param source String to be converted into a path to be copied into the destination.
* @returns A new path whose raw data is of the two concatenated parametres.
*/
path_t path_append_as(const path_t destination, const char *source);

/*
* @brief Append a given path to a string representing a path.
* @param destination Base string to which to append.
* @param source Path from which to be copied into the destination.
* @returns A new path whose raw data is of the two concatenated parametres.
*/
path_t path_append_to(const char *destination, const path_t source);

/*
* @brief Get the parent directory of a given path.
* @param path A path, either relative or absolute, to parse.
* @returns A new path with the data as the path to the parent directory of the given path.
*/
path_t path_get_parent(const path_t path);

/*
* @brief Obtain the root directory of a given path.
* @param path Either absolute or realtive path from which to extract the root.
* @returns A path object representation of the root directory.
*/
path_t path_get_root(const path_t path);

/*
* @brief Obtain a path suffix (file extension) from a given path.
* @param path Path from which to obtain a suffix.
* @returns A path representation of a path suffix.
*/
path_t path_get_suffix(const path_t path);

// TODO: Implement a file type dispatch that calls a different function.
/*
* @brief Create a file.
* @param path A path to a file to create.
*/
void path_touch_file(const path_t path);

/*
* @brief Create a directory.
* @param path Path to a directory to create.
*/
void path_mkdir(const path_t path);

#endif // PATH_H_

#ifdef PATHLIB_IMPLEMENTATION

/*
* @brief Initialize a new path. The underlying path data is initialized to NULL after using this function.
* @returns A new, 0-initialized, Path.
*/
path_t path_new()
{
    path_t path = {0};
    return path;
}

/*
* @brief Initialize a new path.
* @param path String literal to initialize the path object.
* @returns A new Path.
*/
path_t path_from(const char *path)
{
    path_t return_path = {0};
    return_path.raw = path;
    return return_path;
}

/*
* @brief Determine whether a given path exists. On Windows, the `windows.h` header is used.
* @param path String literal, marked with const, to check against whether it exists.
* @returns Whether the path exists.
*/
bool path_exists(const char *path)
{
#ifdef _WIN32
    DWORD dwAttrib = GetFileAttributes(path);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES);
#else
    struct stat stat_buffer = {0};
    if (stat(path, &stat_buffer) < 0)
    {
        if (errno == ENOENT)
        {
            errno = 0;
            return false;
        }
    }
    return S_ISDIR(stat_buffer.st_mode);
#endif // _WIN32
}

/*
* @brief Get the absolute value of a given path.
* @param path Relative path to extract the absolute.
* @returns A new path with the absolute path of the realtive given path.
*/
path_t pasb(const path_t path)
{
#ifdef _WIN32
    if(GetFullPathName(path.raw, MAX_PATH_SIZE, __path_buffer, NULL) == 0)
    {
        fprintf(stderr, "IOError: cannot get absolute path: %ld\n", GetLastError());
        exit(1);
    }
    path_t new_path = {0};
    new_path.raw = __path_buffer;
    // printf("Error: %ld\n", GetLastError());
    return new_path;
#else
    path_t new_path = {0};
    return new_path; // TODOl Implement the linux version.
#endif // _WIN32
}

/*
* @brief Represent a path as a string.
* @param path Read-Only Path to represent.
* @returns The underlying path data within the given object.
*/
const char *passtr(const path_t path)
{
    return path.raw;
}

/*
* @brief Append a given path to another.
* @param destination Base path to which to append.
* @param source Path from which to be copied into the destination.
* @returns A new path whose raw data is of the two concatenated parametres.
*/
path_t path_append(const path_t destination, const path_t source)
{
    size_t dest_len = strlen(destination.raw);
    size_t source_len = strlen(source.raw);
    size_t full_length = (dest_len + source_len) + 2;
    char *string = __path_buffer;
    string[dest_len] = *PATH_SEPERATOR;
    for (size_t i = 1; i < dest_len; ++i)
    {
        string[i] = destination.raw[i];
    }
    for (size_t j = 0; j < source_len; ++j)
    {
        string[(dest_len + 1) + j] = source.raw[j];
    }
    string[full_length - 1] = '\0';
    return path_from(string);
}

/*
* @brief Append a given string to a path.
* @param destination Base path to which to append.
* @param source String without a seperator to be converted into a path to be copied into the destination.
* @returns A new path whose raw data is of the two concatenated parametres.
*/
path_t path_append_as(const path_t destination, const char *source)
{
    path_t source_path = path_from(source);
    path_t new_path = path_append(destination, source_path);
    return new_path;
}

/*
* @brief Append a given path to a string representing a path.
* @param destination Base string without a seperator to which to append.
* @param source Path from which to be copied into the destination.
* @returns A new path whose raw data is of the two concatenated parametres.
*/
path_t path_append_to(const char *destination, const path_t source)
{
    path_t destination_path = path_from(destination);
    path_t new_path = path_append(destination_path, source);
    return new_path;
}

/*
* @brief Internal way of finding the last path seperator in a given path.
* @param path Path string to parse.
* @returns Index of the last stroke.
*/
static size_t _find_last_stroke(const char *path)
{
#ifdef _WIN32
    #define __SEP '\\'
#else
    #define __SEP '/'
#endif // _WIN32
    const char *result = strrchr(path, __SEP);
    const size_t full_length = strlen(path);
    if (!result)
    {
        return 0;
    }
    return full_length - strlen(result);
}

/*
* @brief Get the parent directory of a given path.
* @param path A path, either relative or absolute, to parse.
* @returns A new path with the data as the path to the parent directory of the given path.
*/
path_t path_get_parent(const path_t path)
{
    path_t full_path = pasb(path);
    // printf("full: %s\n", full_path.raw);
    const size_t last_stroke = _find_last_stroke(full_path.raw);
    // printf("i: %d\n", last_stroke);
    // printf("Full Path: %s\nIndex: %d\n", full_path.raw, (int)last_stroke);
    for (size_t i = 0; i < last_stroke; ++i)
    {
        // printf("%c\n", full_path.raw[i]);
        __path_buffer[i] = full_path.raw[i];
    }
    __path_buffer[last_stroke] = '\0';
    path_t new_path = {0};
    new_path.raw = __path_buffer;
    return new_path;
    // return __path_buffer;
}

/*
* @brief Obtain the root directory of a given path.
* @param path Either absolute or realtive path from which to extract the root.
* @returns A path object representation of the root directory.
*/
path_t path_get_root(const path_t path)
{
    const path_t absolute = pasb(path);
    const char *result = strchr(absolute.raw, *PATH_SEPERATOR);
    const size_t full_length = strlen(absolute.raw);
    const size_t result_length = full_length - strlen(result);
    for (size_t i = 0; i < result_length; ++i)
    {
        __path_buffer[i] = absolute.raw[i];
    }
    __path_buffer[result_length] = '\0';
    path_t new_path = {0};
    new_path.raw = __path_buffer;
    return new_path;
}

/*
* @brief Obtain a path suffix (file extension) from a given path.
* @param path Path from which to obtain a suffix.
* @returns A path representation of a path suffix.
*/
path_t path_get_suffix(const path_t path)
{
    const path_t absolute = pasb(path);
    const char *result = strrchr(absolute.raw, '.');
    const size_t result_length = strlen(result);
    for (size_t i = 0; i < result_length; ++i)
    {
        __path_buffer[i] = result[i];
    }
    __path_buffer[result_length] = '\0';
    path_t new_path = {0};
    new_path.raw = __path_buffer;
    return new_path;
}

/*
* @brief Create a file.
* @param path A path to a file to create.
*/
void path_touch_file(const path_t path)
{
    FILE *file = fopen(path.raw, "w");
    if (NULL == file || errno == EEXIST)
    {
        fprintf(stderr, "IOError: Cannot open file '%s': %s\n", path.raw, strerror(errno));
        exit(1);
    }
    fclose(file);
}

/*
* @brief Create a directory.
* @param path Path to a directory to create.
*/
void path_mkdir(const path_t path)
{
#ifdef _WIN32
    int error = _mkdir(path.raw);
    if(error < 0 || errno == EEXIST || errno == ENOENT)
    {
        fprintf(stderr, "IOError: Cannot create directory: %s\n", strerror(errno));
        exit(1);
    }
#else
    int error = mkdir(path.raw, 0755);
    if(error < 0 || errno == EEXIST || errno == ENOENT)
    {
        fprintf(stderr, "IOError: Cannot create directory: %s\n", strerror(errno));
        exit(1);
    }
#endif // _WIN32
}

#endif // PATHLIB_IMPLEMENTATION