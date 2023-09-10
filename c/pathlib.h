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
} Path;

/*
* @brief Initialize a new path. The underlying path data is initialized to NULL after using this function.
* @returns A new, 0-initialized, Path.
*/
Path path_new();

/*
* @brief Initialize a new path.
* @param path String literal to initialize the path object.
* @returns A new Path.
*/
Path path_from(const char *path);

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
Path pasb(Path path);

/*
* @brief Represent a path as a string.
* @param path Read-Only Path to represent.
* @returns The underlying path data within the given object.
*/
const char *passtr(Path path);

/*
* @brief Append a given path to another.
* @param destination Base path to which to append.
* @param source Path from which to be copied into the destination.
* @returns A new path whose raw data is of the two concatenated parametres.
*/
Path path_append(const Path destination, const Path source);

/*
* @brief Append a given string to a path.
* @param destination Base path to which to append.
* @param source String to be converted into a path to be copied into the destination.
* @returns A new path whose raw data is of the two concatenated parametres.
*/
Path path_append_as(const Path destination, const char *source);

/*
* @brief Append a given path to a string representing a path.
* @param destination Base string to which to append.
* @param source Path from which to be copied into the destination.
* @returns A new path whose raw data is of the two concatenated parametres.
*/
Path path_append_to(const char *destination, const Path source);

/*
* @brief Get the parent directory of a given path.
* @param path A path, either relative or absolute, to parse.
* @returns A new path with the data as the path to the parent directory of the given path.
*/
Path path_get_parent(const Path path);

// TODO: Implement a file type dispatch that calls a different function.
/*
* @brief Create a file.
* @param path A path to a file to create.
*/
void path_touch_file(Path path);

/*
* @brief Create a directory.
* @param path Path to a directory to create.
*/
void path_mkdir(Path path);

#endif // PATH_H_

#ifdef PATHLIB_IMPLEMENTATION

/*
* @brief Initialize a new path. The underlying path data is initialized to NULL after using this function.
* @returns A new, 0-initialized, Path.
*/
Path path_new()
{
    Path path = {0};
    return path;
}

/*
* @brief Initialize a new path.
* @param path String literal to initialize the path object.
* @returns A new Path.
*/
Path path_from(const char *path)
{
    Path return_path = {0};
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
Path pasb(Path path)
{
#ifdef _WIN32
    if(GetFullPathName(path.raw, MAX_PATH_SIZE, __path_buffer, NULL) == 0)
    {
        fprintf(stderr, "IOError: cannot get absolute path: %ld\n", GetLastError());
        exit(1);
    }
    Path new_path;
    new_path.raw = __path_buffer;
    // printf("Error: %ld\n", GetLastError());
    return new_path;
#else
    Path new_path = {0};
    return new_path; // TODOl Implement the linux version.
#endif // _WIN32
}

/*
* @brief Represent a path as a string.
* @param path Read-Only Path to represent.
* @returns The underlying path data within the given object.
*/
const char *passtr(Path path)
{
    return path.raw;
}

/*
* @brief Append a given path to another.
* @param destination Base path to which to append.
* @param source Path from which to be copied into the destination.
* @returns A new path whose raw data is of the two concatenated parametres.
*/
Path path_append(const Path destination, const Path source)
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
Path path_append_as(const Path destination, const char *source)
{
    Path source_path = path_from(source);
    Path new = path_append(destination, source_path);
    return new;
}

/*
* @brief Append a given path to a string representing a path.
* @param destination Base string without a seperator to which to append.
* @param source Path from which to be copied into the destination.
* @returns A new path whose raw data is of the two concatenated parametres.
*/
Path path_append_to(const char *destination, const Path source)
{
    Path destination_path = path_from(destination);
    Path new = path_append(destination_path, source);
    return new;
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
Path path_get_parent(const Path path)
{
    Path full_path = pasb(path);
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
    Path new_path = {0};
    new_path.raw = __path_buffer;
    return new_path;
    // return __path_buffer;
}

/*
* @brief Create a file.
* @param path A path to a file to create.
*/
void path_touch_file(Path path)
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
void path_mkdir(Path path)
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
        printf("hit\n");
        fprintf(stderr, "IOError: Cannot create directory: %s\n", strerror(errno));
        exit(1);
    }
#endif // _WIN32
}

#endif // PATHLIB_IMPLEMENTATION