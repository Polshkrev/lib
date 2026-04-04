#ifndef PATH_H
#define PATH_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h> // bool

/**
 * @brief Representation of a path on the file system.
 */
typedef struct
{
    const char *raw;
} path_t;

/**
 * @brief Initialize a new path.
 * @returns A new path storing the current working directory.
 */
path_t *path_init();

/**
 * @brief Initialize a new path from a given raw value.
 * @param raw Raw string path value to initialize the path.
 * @returns A new path storing the given raw parametre.
 * @exception If the path can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
path_t *path_from(const char *raw);

/**
 * @brief Determine if the given path exists on the filesystem.
 * @param path Path to evaluate.
 * @returns True if the path is evaluated to exist on the filesystem, else false.
 */
bool path_exists(const path_t *path);

/**
 * @brief Obtain the absolute value of the given path.
 * @param path Path from which to obtian the absolute value.
 * @returns A new path storing the absolute value of the given path parametre.
 * @exception If the given path does not exist, a `FileNotFoundError` is printed to `stderr` and the programme exits.
 * @exception If the absolute path value can not be obtained, an `IOError` is printed to `stderr` and the programme exits.
 */
path_t *pasb(const path_t *path);

/**
 * @brief Obtain the string representation of the given path value.
 * @param path Path from which to represent as a string.
 * @returns A string representation of the given path value.
 */
const char *passtr(const path_t *path);

/**
 * @brief Append a path to another.
 * @param destination Destination path to which to append.
 * @param source Source path from which to append.
 * @returns A new path constructed of the given source appended to the given destination.
 */
path_t *path_append(const path_t *destination, const path_t *source);

/**
 * @brief Append a given c-string to a given path.
 * @param destination Destination path to which to append.
 * @param source C-string from which to append.
 * @returns A new path constructed of the given source and the given destination.
 */
path_t *path_append_as(const path_t *destination, const char *source);

/**
 * @brief Append a given path to a given string.
 * @param destination Destination string to which to append.
 * @param source Path from which to append.
 * @returns A new path constructed of the given source and the given destination.
 */
path_t *path_append_to(const char *destination, const path_t *source);

/**
 * @brief Obtain the parent directory of the given path.
 * @param path Path from which a parent directory is obtained.
 * @returns A new path constructed from the parent directory of the given path.
 * @exception If the given path does not exist, a `FileNotFoundError` is printed to `stderr` and the programme exits.
 * @exception If the absolute path value can not be obtained, an `IOError` is printed to `stderr` and the programme exits.
 */
path_t *path_get_parent(const path_t *path);

/**
 * @brief Obtain the root of the filesystem.
 * @param path Path from which to obtain its root.
 * @returns A new path containing the root of the filesystem. If the root can not be obtained the original unchanged path is returned.
 */
path_t *path_get_root(const path_t *path);

/**
 * @brief Obtain the filename of the given path.
 * @param path Path from which to obtain the filename.
 * @returns The filename of the given path.
 */
path_t *path_filename(const path_t *path);

/**
 * @brief Obtain the extension of the given path.
 * @param path Path from which to obtain the file extension.
 * @returns The extension of the given path.
 */
const char *path_extension(const path_t *path);

/**
 * @brief Deallocate the given path parametre.
 * @param path Path to deallocate.
 */
void path_delete(path_t *path);

#if defined(__cplusplus)
}
#endif

#endif // PATH_H

#ifdef PATH_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdlib.h> // malloc, NULL, MAX_PATH, free, exit
#include <stdio.h> // fprintf, stderr
#include <string.h> // strlen, strrchr, strerror
#include <errno.h> // errno, ENOENT
#include <stddef.h> // size_t

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
// #include <minwinbase.h> // DWORD
// #include <winbase.h> // CopyFile
// #include <fileapi.h> // GetFullPathName, GetFileAttributes, INVALID_FILE_ATTRIBUTES
// #include <errhandlingapi.h> // GetLastError
#else
#include <sys/stat.h> // struct stat, stat, S_ISDIR
#endif // _WIN32

#ifdef _WIN32
/**
 * @brief OS-dependant path seperator.
 */
#define PATH_SEPERATOR '\\'
#else
/**
 * @brief OS-dependant path seperator.
 */
#define PATH_SEPERATOR '/'
#endif // _WIN32

char __path_buffer[_MAX_PATH] = {0};

/**
 * @brief Initialize a new path.
 * @returns A new path storing the current working directory.
 */
path_t *path_init()
{
    return path_from(".");
}

/**
 * @brief Initialize a new path from a given raw value.
 * @param raw Raw string path value to initialize the path.
 * @returns A new path storing the given raw parametre.
 * @exception If the path can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
path_t *path_from(const char *raw)
{
    path_t *path = (path_t *)malloc(sizeof(path_t));
    if (NULL == path)
    {
        fprintf(stderr, "AllocationError: Can not allocate enough memory for a new path.\n");
        exit(1);
    }
    path->raw = raw;
    return path;
}

/**
 * @brief Determine if the given path exists on the filesystem.
 * @param path Path to evaluate.
 * @returns True if the path is evaluated to exist on the filesystem, else false.
 */
bool path_exists(const path_t *path)
{
#ifdef _WIN32
    DWORD dwAttributes = GetFileAttributes(passtr(path));
    return (dwAttributes != INVALID_FILE_ATTRIBUTES);
#else
    struct stat stat_buffer = {0};
    if (stat(passtr(path), &stat_buffer) < 0)
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

/**
 * @brief Obtain the absolute value of the given path.
 * @param path Path from which to obtian the absolute value.
 * @returns A new path storing the absolute value of the given path parametre.
 * @exception If the given path does not exist, a `FileNotFoundError` is printed to `stderr` and the programme exits.
 * @exception If the absolute path value can not be obtained, an `IOError` is printed to `stderr` and the programme exits.
 */
path_t *pasb(const path_t *path)
{
#ifdef _WIN32
    if (GetFullPathName(passtr(path), MAX_PATH, __path_buffer, NULL) == 0)
    {
        fprintf(stderr, "IOError: Can not get absolute path: %ld\n", GetLastError());
        exit(1);
    }
#else
    if (NULL == realpath(passtr(path), buffer))
    {
        fprintf(stderr, "IOError: Can not get absolute path: %s\n", strerror(errno));
        exit(1);
    }
#endif // _WIN32
    return path_from(__path_buffer);
}

/**
 * @brief Obtain the string representation of the given path value.
 * @param path Path from which to represent as a string.
 * @returns A string representation of the given path value.
 */
const char *passtr(const path_t *path)
{
    return path->raw;
}

/**
 * @brief Append a path to another.
 * @param destination Destination path to which to append.
 * @param source Source path from which to append.
 * @returns A new path constructed of the given source appended to the given destination.
 */
path_t *path_append(const path_t *destination, const path_t *source)
{
    return path_append_as(destination, passtr(source));
}

/**
 * @brief Append a given c-string to a given path.
 * @param destination Destination path to which to append.
 * @param source C-string from which to append.
 * @returns A new path constructed of the given source and the given destination.
 */
path_t *path_append_as(const path_t *destination, const char *source)
{
#ifdef _WIN32
    StringCbPrintf(__path_buffer, MAX_PATH, "%s%c%s", destination->raw, PATH_SEPERATOR, source);
#else
    sprintf(__path_buffer, "%s%c%s", destination->raw, PATH_SEPERATOR, source);
#endif // _WIN32
    return path_from(__path_buffer);
}

/**
 * @brief Append a given path to a given string.
 * @param destination Destination string to which to append.
 * @param source Path from which to append.
 * @returns A new path constructed of the given source and the given destination.
 */
path_t *path_append_to(const char *destination, const path_t *source)
{
#ifdef _WIN32
    StringCbPrintf(__path_buffer, _MAX_PATH, "%s%c%s", destination, PATH_SEPERATOR, source->raw);
#else
    sprintf(__path_buffer, "%s%c%s", destination, PATH_SEPERATOR, source->raw);
#endif // _WIN32
    return path_from(__path_buffer);
}

/**
 * @brief Find the last stroke of the given path.
 * @param path Path from which to obtain the last stroke.
 * @returns Index where the last stroke was found.
 * @returns If the index of the last stroke can not be found, 0 is returned.
 */
static ssize_t _find_last_stroke(const char *path)
{
    const char *result = strrchr(path, PATH_SEPERATOR);
    size_t full_length = strlen(path);
    if (!result)
    {
        return -1;
    }
    return full_length - strlen(result);
}

/**
 * @brief Obtain the parent directory of the given path.
 * @param path Path from which a parent directory is obtained.
 * @returns A new path constructed from the parent directory of the given path.
 * @exception If the given path does not exist, a `FileNotFoundError` is printed to `stderr` and the programme exits.
 * @exception If the absolute path value can not be obtained, an `IOError` is printed to `stderr` and the programme exits.
 */
path_t *path_get_parent(const path_t *path)
{
    path_t *absolute = pasb(path);
    ssize_t last_stroke = _find_last_stroke(passtr(absolute));
    if (last_stroke < 0)
    {
        return path_from(passtr(path));
    }
    for (ssize_t i = 0; i < last_stroke; ++i)
    {
        __path_buffer[i] = absolute->raw[i];
    }
    __path_buffer[last_stroke] = '\0';
    path_delete(absolute);
    return path_from(__path_buffer);
}

/**
 * @brief Find the first stroke of the given path.
 * @param path Path from which to obtain the first stroke.
 * @returns The index where the first stroke was found.
 * @returns If the index of the last stroke can not be found, negative one is returned.
 */
static ssize_t _find_first_stroke(const char *path)
{
    const char *result = strchr(path, PATH_SEPERATOR);
    size_t full_length = strlen(path);
    if (!result)
    {
        return -1;
    }
    return full_length - strlen(result);
}

/**
 * @brief Obtain the root of the filesystem.
 * @param path Path from which to obtain its root.
 * @returns A new path containing the root of the filesystem. If the root can not be obtained the original unchanged path is returned.
 */
path_t *path_get_root(const path_t *path)
{
#ifdef _WIN32
    path_t *absolute = pasb(path);
    ssize_t first_stroke = _find_first_stroke(passtr(absolute));
    if (first_stroke < 0)
    {
        path_delete(absolute);
        return path_from(passtr(path));
    }
    for (ssize_t i = 0; i < first_stroke; ++i)
    {
        __path_buffer[i] = absolute->raw[i];
    }
    __path_buffer[first_stroke] = '\0';
    path_delete(absolute);
    return path_from(__path_buffer);
#else
    return path_from("/");
#endif // _WIN32
}

/**
 * @brief Obtain the filename of the given path.
 * @param path Path from which to obtain the filename.
 * @returns The filename of the given path.
 */
path_t *path_filename(const path_t *path)
{
#ifndef _WIN32
    if (!path->raw || !*path->raw) return path_from(".");
    char *string = strdup(path->raw);
    size_t i = strlen(string) - 1;
    for (; i && string[i] == PATH_SEPERATOR; i--) string[i] = 0;
    for (; i && string[i-1] != PATH_SEPERATOR; i--);
    return path_from(string + i);
#else
    static char filename[_MAX_FNAME] = {0};
    errno_t result = _splitpath_s(path->raw, NULL, 0, NULL, 0, filename, _MAX_FNAME, NULL, 0);
    if (result != 0) return path_from(passtr(path));
    filename[_MAX_FNAME - 1] = '\0';
    return path_from(filename);
#endif // _WIN32
}

/**
 * @brief Obtain the extension of the given path.
 * @param path Path from which to obtain the file extension.
 * @returns The extension of the given path.
 */
const char *path_extension(const path_t *path)
{
#ifndef _WIN32
    return strrchr(path_filename(path), ".");
#else
    errno_t result = _splitpath_s(passtr(path), NULL, 0, NULL, 0, NULL, 0, __path_buffer, _MAX_EXT);
    if (result != 0) return passtr(path);
    __path_buffer[_MAX_EXT] = '\0';
    return __path_buffer + 1;
#endif // _WIN32
}

/**
 * @brief Deallocate the given path parametre.
 * @param path Path to deallocate.
 */
void path_delete(path_t *path)
{
    if (!path)
    {
        return;
    }
    free(path);
    path = NULL;
}

#if defined(__cplusplus)
}
#endif

#endif // PATH_IMPLEMENTATION