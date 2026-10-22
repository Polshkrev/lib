#ifndef PATH_H
#define PATH_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h> // bool
#include <stddef.h> // size_t, ptrdiff_t

#ifdef _WIN32
    #include <minwindef.h> // MAX_PATH
    #define PATH_MAXIMUM MAX_PATH
#else
    #include <limits.h> // PATH_MAX
    #define PATH_MAXIMUM PATH_MAX
#endif

/**
 * @brief Representation of a path on the file system.
 */
typedef struct
{
    size_t count;
    char raw[PATH_MAXIMUM];
} path_t;

/**
 * @brief Initialise a new path.
 * @returns A new path storing the current working directory.
 */
path_t path_init(void);

/**
 * @brief Initialise a new path from a given raw value.
 * @param raw Raw string value with which to initialise the path.
 * @returns A new path storing the given raw parameter.
 * @exception If the path can not be constructed, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_from(const char *raw);

/**
 * @brief Determine if the given path exists on the filesystem.
 * @param path Path to evaluate.
 * @returns True if the path exists on the filesystem, else false.
 */
bool path_exists(const path_t *path);

/**
 * @brief Obtain the absolute value of the given path.
 * @param path Path from which to obtain the absolute value.
 * @returns A new path storing the absolute value of the given path parameter.
 * @exception If the absolute path value can not be obtained, an `IOError` is printed to `stderr` and the programme exits.
 */
path_t pasb(const path_t *path);

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
 * @exception If the path can not be appended, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_append(const path_t *destination, const path_t *source);

/**
 * @brief Append a given c-string to a given path.
 * @param destination Destination path to which to append.
 * @param source C-string from which to append.
 * @returns A new path constructed of the given source and the given destination.
 * @exception If the path can not be appended, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_append_as(const path_t *destination, const char *source);

/**
 * @brief Append a given path to a given string.
 * @param destination Destination string to which to append.
 * @param source Path from which to append.
 * @returns A new path constructed of the given source and the given destination.
 * @exception If the path can not be appended, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_append_to(const char *destination, const path_t *source);

/**
 * @brief Obtain the parent directory of the given path.
 * @param path Path from which a parent directory is obtained.
 * @returns A new path constructed from the parent directory of the given path.
 * @exception If the parent path can be obtained, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_get_parent(const path_t *path);

/**
 * @brief Obtain the root of the filesystem.
 * @param path Path from which to obtain its root.
 * @returns A new path containing the root of the filesystem. If the root can not be obtained the original unchanged path is returned.
 * @exception If the absolute path can not be obtained, an `IOError` is printed to `stderr` and the programme exits.
 * @exception If the root path can not be obtained, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_get_root(const path_t *path);

/**
 * @brief Obtain the filename of the given path.
 * @param path Path from which to obtain the filename.
 * @returns The filename of the given path.
 * @exception If the filename can not be obtained, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_filename(const path_t *path);

/**
 * @brief Obtain the extension of the given path.
 * @param path Path from which to obtain the file extension.
 * @returns The extension of the given path.
 */
const char *path_extension(const path_t *path);

#if defined(__cplusplus)
}
#endif

#endif // PATH_H

#ifdef PATH_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h> // fprintf, stderr, snprintf, sprintf
#include <stdlib.h> // exit, realpath
#include <string.h> // strlen, strerror
#include <errno.h> // errno
#include <ctype.h> // isspace

#ifdef _WIN32
    #include <winnls.h> // CP_UTF8
    #include <consoleapi.h> // ConsoleCP, ConsoleOutputCP
    #include <consoleapi2.h> // SetConsoleCP, SetConsoleOutputCP
    #include <fileapi.h> // GetCurrentDirectory, GetFullPathName, GetFileAttributes, INVALID_FILE_ATTRIBUTES
    #include <errhandlingapi.h> // GetLastError
    #include <winbase.h> // FormatMessage, FORMAT_MESSAGE_FROM_SYSTEM, FORMAT_MESSAGE_IGNORE_INSERTS, ERROR_MR_MID_NOT_FOUND
#else
    #include <sys/stat.h> // struct stat, stat
    #include <unistd.h> // getcwd
#endif

#ifdef _WIN32
/**
 * @brief OS-dependent path separator.
 */
#define PATH_SEPARATOR '\\'
#else
/**
 * @brief OS-dependent path separator.
 */
#define PATH_SEPARATOR '/'
#endif // _WIN32

#ifdef _WIN32

#ifndef WIN32_PATH_ERROR_MESSAGE_SIZE
#define WIN32_PATH_ERROR_MESSAGE_SIZE (4 * 1024)
#endif // WIN32_PATH_ERROR_MESSAGE_SIZE

/**
 * @brief Format a `Windows` error code as a string.
 * @param error Error code to format.
 * @returns The given error code formatted as a string.
 */
static char *__path_error_message_windows(DWORD error)
{
    static char win32_error_message[WIN32_PATH_ERROR_MESSAGE_SIZE] = {0};
    DWORD error_message_size = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, LANG_USER_DEFAULT, win32_error_message, WIN32_PATH_ERROR_MESSAGE_SIZE, NULL);
    if (error_message_size == 0)
    {
        if (GetLastError() != ERROR_MR_MID_NOT_FOUND)
        {
            if (sprintf(win32_error_message, "Could not get error message for 0x%lX", error) <= 0) return NULL;
            return win32_error_message;
        }
        else if (sprintf(win32_error_message, "Invalid windows error code (0x%lX)", error) <= 0) return NULL;
        return win32_error_message;

    }
    while (error_message_size > 1 && isspace((unsigned char)win32_error_message[error_message_size - 1])) { win32_error_message[--error_message_size] = '\0'; }
    return win32_error_message;
}
#endif // _WIN32

/**
 * @brief Initialise a new path.
 * @returns A new path storing the current working directory.
 */
path_t path_init(void)
{
#ifdef _WIN32
    if (GetConsoleOutputCP() != CP_UTF8) SetConsoleOutputCP(CP_UTF8);
    if (GetConsoleCP() != CP_UTF8) SetConsoleCP(CP_UTF8);
#endif // _WIN32
    path_t result = {0};
#ifdef _WIN32
    DWORD count = GetCurrentDirectory(sizeof(result.raw), result.raw);
    if (count == 0)
    {
        fprintf(stderr, "IOError: Can not get current directory: %s\n", __path_error_message_windows(GetLastError()));
        exit(1);
    }
    else if (count >= sizeof(result.raw))
    {
        fprintf(stderr, "IOError: Current directory is too long.\n");
        exit(1);
    }
    result.count = (size_t)count;
#else
    if (NULL == getcwd(result.raw, sizeof(result.raw)))
    {
        fprintf(stderr, "IOError: Can not get current directory: %s\n", strerror(errno));
        exit(1);
    }
    result.count = strlen(result.raw);
#endif
    return result;
}

/**
 * @brief Construct a new path with a given raw string and size.
 * @param raw The raw string representation of the path.
 * @param size The length of the string excluding the NULL terminator.
 * @returns A path containing the given raw string and size.
 * @exception If the given size is greater than the maximum allowed size, a `ValueError` is printed to `stderr` and the programme exits.
 * @exception If the given parts can not be written to a path, a `ValueError` is printed to `stderr` and the programme exits.
 * @exception If the amount written to the path is not equal to the given size, a `ValueError` is printed to `stderr` and the programme exits.
 */
static path_t __path_with_size(const char *raw, size_t size)
{
    path_t result = {0};
    if (size >= sizeof(result.raw))
    {
        fprintf(stderr, "ValueError: Raw path exceeds maximum path length.\n");
        exit(1);
    }

    int n = snprintf(result.raw, sizeof(result.raw), "%s", raw);

    if (n < 0)
    {
        fprintf(stderr, "ValueError: Can not write raw string to path.\n");
        exit(1);
    }
    else if ((size_t)n != size)
    {
        fprintf(stderr, "ValueError: Can not guarantee the size of the raw path.\n");
        exit(1);
    }

    result.count = size;

    return result;
}

/**
 * @brief Construct a new path of the given raw string until the given size.
 * @param raw The raw string representation of the path.
 * @param size The total length of characters to set within the path excluding the NULL terminator.
 * @return A path containing the specified prefix.
 * @exception If the given size is greater than the maximum allowed size, a `ValueError` is printed to `stderr` and the programme exits.
 * @exception If the given parts can not be written to a path, a `ValueError` is printed to `stderr` and the programme exits.
 * @exception If the amount written to the path is not equal to the given size, a `ValueError` is printed to `stderr` and the programme exits.
 */
static path_t __path_with_prefix(const char *raw, size_t size)
{
    path_t result = {0};

    if (size >= sizeof(result.raw))
    {
        fprintf(stderr, "ValueError: The given size has exceeded the maximum allowed.\n");
        exit(1);
    }

    int n = snprintf(result.raw, sizeof(result.raw), "%.*s", (int)size, raw);

    if (n < 0)
    {
        fprintf(stderr, "ValueError: Can not write raw string to path.\n");
        exit(1);
    }
    else if ((size_t)n != size)
    {
        fprintf(stderr, "ValueError: The amount written to the path does not match the provided size.\n");
        exit(1);
    }

    result.count = size;

    return result;
}

/**
 * @brief Initialise a new path from a given raw value.
 * @param raw Raw string value with which to initialise the path.
 * @returns A new path storing the given raw parameter.
 * @exception If the path can not be constructed, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_from(const char *raw)
{
#ifdef _WIN32
    if (GetConsoleOutputCP() != CP_UTF8) SetConsoleOutputCP(CP_UTF8);
    if (GetConsoleCP() != CP_UTF8) SetConsoleCP(CP_UTF8);
#endif // _WIN32
    return __path_with_size(raw, strlen(raw));
}

/**
 * @brief Determine if the given path exists on the filesystem.
 * @param path Path to evaluate.
 * @returns True if the path exists on the filesystem, else false.
 */
bool path_exists(const path_t *path)
{
#ifdef _WIN32
    DWORD attributes = GetFileAttributes(passtr(path));
    return (attributes != INVALID_FILE_ATTRIBUTES);
#else
    struct stat stat_buffer = {0};
    return stat(passtr(path), &stat_buffer) == 0;
#endif // _WIN32
}

/**
 * @brief Obtain the absolute value of the given path.
 * @param path Path from which to obtain the absolute value.
 * @returns A new path storing the absolute value of the given path parameter.
 * @exception If the absolute path value can not be obtained, an `IOError` is printed to `stderr` and the programme exits.
 */
path_t pasb(const path_t *path)
{
    path_t output = {0};
#ifdef _WIN32
    DWORD count = GetFullPathName(path->raw, PATH_MAXIMUM, output.raw, NULL);
    if (count == 0)
    {
        fprintf(stderr, "IOError: Can not get absolute path: %s\n", __path_error_message_windows(GetLastError()));
        exit(1);
    }
    else if (count >= PATH_MAXIMUM)
    {
        fprintf(stderr, "IOError: The absolute path exceeds the maximum path size allowed.\n");
        exit(1);
    }
    output.count = (size_t)count;
#else
    if (NULL == realpath(path->raw, output.raw))
    {
        fprintf(stderr, "IOError: Can not obtain absolute path: %s\n", strerror(errno));
        exit(1);
    }
    output.count = strlen(output.raw);
#endif
    return output;
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
 * @exception If the path can not be appended, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_append(const path_t *destination, const path_t *source)
{
    size_t count = destination->count + 1 + source->count;

    if (count >= PATH_MAXIMUM)
    {
        fprintf(stderr, "ValueError: The length of the appended path has exceeded the maximum size allowed.\n");
        exit(1);
    }

    path_t output = *destination;

    output.raw[destination->count] = PATH_SEPARATOR;

    int n = snprintf(output.raw + destination->count + 1, sizeof(output.raw) - destination->count - 1, "%s", source->raw);

    if ((n < 0) || ((size_t)n != source->count))
    {
        fprintf(stderr, "ValueError: Path can not be appended.\n");
        exit(1);
    }

    output.count = count;

    return output;
}

/**
 * @brief Append a given c-string to a given path.
 * @param destination Destination path to which to append.
 * @param source C-string from which to append.
 * @returns A new path constructed of the given source and the given destination.
 * @exception If the path can not be appended, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_append_as(const path_t *destination, const char *source)
{
    path_t source_path = path_from(source);
    return path_append(destination, &source_path);
}

/**
 * @brief Append a given path to a given string.
 * @param destination Destination string to which to append.
 * @param source Path from which to append.
 * @returns A new path constructed of the given source and the given destination.
 * @exception If the path can not be appended, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_append_to(const char *destination, const path_t *source)
{
    path_t destination_path = path_from(destination);
    return path_append(&destination_path, source);
}

/**
 * @brief Find the last occurrence of a character within a string.
 * @param string String within which to search.
 * @param character Character for which to search.
 * @returns The index of the last occurrence of the given character.
 * @returns If the given character does not occur within the string, `-1` is returned.
 */
static ptrdiff_t __find_last_character(const char *string, char character)
{
    size_t count = strlen(string);

    for (size_t index = count; index > 0; index--)
    {
        if (string[index - 1] != character) continue;
        return (ptrdiff_t)(index - 1);
    }

    return -1;
}

/**
 * @brief Find the first occurrence of a character within a string.
 * @param string String within which to search.
 * @param character Character for which to search.
 * @returns The index of the first occurrence of the given character.
 * @returns If the given character does not occur within the string, `-1` is returned.
 */
static ptrdiff_t __find_first_character(const char *string, char character)
{
    size_t count = strlen(string);

    for (size_t index = 0; index < count; index++)
    {
        if (string[index] != character) continue;
        return (ptrdiff_t)index;
    }

    return -1;
}

/**
 * @brief Obtain the parent directory of the given path.
 * @param path Path from which a parent directory is obtained.
 * @returns A new path constructed from the parent directory of the given path.
 * @exception If the parent path can not be obtained, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_get_parent(const path_t *path)
{
    ptrdiff_t separator = __find_last_character(path->raw, PATH_SEPARATOR);

    if (separator < 0) return __path_with_size(".", 1);
    else if (separator == 0) return __path_with_prefix(path->raw, 1);

    return __path_with_prefix(path->raw, (size_t)separator);
}

/**
 * @brief Obtain the root of the filesystem.
 * @param path Path from which to obtain its root.
 * @returns A new path containing the root of the filesystem. If the root can not be obtained the original unchanged path is returned.
 * @exception If the absolute path can not be obtained, an `IOError` is printed to `stderr` and the programme exits.
 * @exception If the root path can not be obtained, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_get_root(const path_t *path)
{
#ifdef _WIN32
    path_t absolute = pasb(path);
    ptrdiff_t separator = __find_first_character(absolute.raw, PATH_SEPARATOR);
    if (separator < 0) return *path;
    return __path_with_prefix(absolute.raw, (size_t)separator);
#else
    (void)path;
    return __path_with_size("/", 1);
#endif
}

/**
 * @brief Obtain the filename of the given path.
 * @param path Path from which to obtain the filename.
 * @returns The filename of the given path.
 * @exception If the filename can not be obtained, a `ValueError` is printed to `stderr` and the programme exits.
 */
path_t path_filename(const path_t *path)
{
    ptrdiff_t separator = __find_last_character(path->raw, PATH_SEPARATOR);
    const char *filename = path->raw;

    if (separator >= 0) filename += (size_t)separator + 1;

    ptrdiff_t extension = __find_last_character(filename, '.');

    if (extension <= 0) return __path_with_prefix(filename, strlen(filename));

    return __path_with_prefix(filename, (size_t)extension);
}

/**
 * @brief Obtain the extension of the given path.
 * @param path Path from which to obtain the file extension.
 * @returns The extension of the given path.
 */
const char *path_extension(const path_t *path)
{
    ptrdiff_t separator = __find_last_character(path->raw, PATH_SEPARATOR);
    const char *filename = path->raw;

    if (separator >= 0) filename += (size_t)separator + 1;

    ptrdiff_t extension = __find_last_character(filename, '.');

    if (extension <= 0 || filename[extension + 1] == '\0') return NULL;

    return filename + extension + 1;
}

#if defined(__cplusplus)
}
#endif

#endif // PATH_IMPLEMENTATION