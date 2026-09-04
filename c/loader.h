#ifndef LOADER_H
#define LOADER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdlib.h> // NULL
#include <stdbool.h> // bool

#ifdef _WIN32
    #include <minwindef.h> // HANDLE, DWORD, FARPROC
    /**
     * @brief Platform specific type to standardize a loaded library.
     */
    typedef HANDLE library_t;
    /**
     * @brief Platform specific type to standardize a loaded function.
     */
    typedef FARPROC function_t;
#else
    /**
     * @brief Platform specific type to standardize a loaded library.
     */
    typedef void *library_t;
    /**
     * @brief Platform specific type to standardize a loaded function.
     */
    typedef void *function_t;
#endif // _WIN32

/**
 * @brief Load a dynamically linked library.
 * @param path Path where the dynamically linked library is located on the filesystem.
 * @returns A handle to dynamically linked library.
 * @returns If the given path does not exist on the file system, `NULL` is returned.
 */
library_t library_load(const char *path);

/**
 * @brief Load a function from a given library handle with a given name.
 * @param library Handle to the library containing the function.
 * @param name Name of the function to find.
 * @returns A handle to the function found within the given library handle.
 * @returns If the function can not be loaded, `NULL` is returned.
 */
function_t library_function(library_t library, const char *name);

/**
 * @brief Obtain the — platform specific — error message when encurring a failure state.
 * @returns A string representation of a platform specific error.
 */
const char *library_error(void);

/**
 * @brief Deallocate the given library handle.
 * @param library Handle to the library to which to deallocate.
 * @returns True if the given library handle was successfully deallocated, else false.
 */
bool library_close(library_t library);

#if defined(__cplusplus)
}
#endif

#endif // LOADER_H

#ifdef LOADER_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h> // snprintf
#include <ctype.h> // isspace

#ifdef _WIN32
    #include <libloaderapi.h> // LoadLibrary, GetProcAddress, FreeLibrary
    #include <fileapi.h> // GetFileAttributes, INVALID_FILE_ATTRIBUTES
    #include <windef.h> // ! NEEDED FOR WINBASE.H
    #include <winbase.h> // FORMAT_MESSAGE_FROM_SYSTEM, FORMAT_MESSAGE_IGNORE_INSERTS, ERROR_MR_MID_NOT_FOUND
    #include <errhandlingapi.h> // GetLastError
#else
    #include <dlfcn.h> // dlopen, dlsym, dlclose, dlerror
    #include <sys/stat.h> // stat
    #include <errno.h> // errno, ENOENT
#endif // _WIN32

/**
 * @brief Determine if the given path exists on the filesystem.
 * @param path Path to evaluate.
 * @returns True if the path is evaluated to exist on the filesystem, else false.
 */
static bool _path_exists(const char *path)
{
#ifdef _WIN32
    DWORD attributes = GetFileAttributes(path);
    return (attributes != INVALID_FILE_ATTRIBUTES);
#else
    struct stat stat_buffer = {0};
    return stat(path, &stat_buffer) == 0;
#endif // _WIN32
}

/**
 * @brief Load a dynamically linked library.
 * @param path Path where the dynamically linked library is located on the filesystem.
 * @returns A handle to dynamically linked library.
 * @returns If the given path does not exist on the file system, `NULL` is returned.
 */
library_t library_load(const char *path)
{
    if (!_path_exists(path)) return NULL;
#ifdef _WIN32
    return LoadLibrary(path);
#else
    return dlopen(path, RTLD_NOW);
#endif // _WIN32
}

/**
 * @brief Load a function from a given library handle with a given name.
 * @param library Handle to the library containing the function.
 * @param name Name of the function to find.
 * @returns A handle to the function found within the given library handle.
 * @returns If the function can not be loaded, `NULL` is returned.
 */
function_t library_function(library_t library, const char *name)
{
#ifdef _WIN32
    return GetProcAddress(library, name);
#else
    return dlsym(library, name);
#endif // _WIN32
}

#ifdef _WIN32
#ifndef WIN32_ERROR_MESSAGE_SIZE
#define WIN32_ERROR_MESSAGE_SIZE (4 * 1024)
#endif // WIN32_ERROR_MESSAGE_SIZE

/**
 * @brief Format a windows error code as a string.
 * @param error Error code to format.
 * @returns The given error code formatted as a string.
 */
static char *__win32_error_message(DWORD error)
{
    static char win32_error_message[WIN32_ERROR_MESSAGE_SIZE] = {0};
    DWORD error_message_size = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, LANG_USER_DEFAULT, win32_error_message, WIN32_ERROR_MESSAGE_SIZE, NULL);
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
    while (error_message_size > 1 && isspace(win32_error_message[error_message_size - 1])) { win32_error_message[--error_message_size] = '\0'; }
    return win32_error_message;
}
#endif // _WIN32

/**
 * @brief Obtain the — platform specific — error message when encurring a failure state.
 * @returns A string representation of a platform specific error.
 */
const char *library_error(void)
{
#ifndef _WIN32
    return dlerror();
#else
    return __win32_error_message(GetLastError());
#endif // _WIN32
}

/**
 * @brief Deallocate the given library handle.
 * @param library Handle to the library to which to deallocate.
 * @returns True if the given library handle was successfully deallocated, else false.
 */
bool library_close(library_t library)
{
    if (NULL == library) return false;
#ifdef _WIN32
    return FreeLibrary(library);
#else
    return dlclose(library) == 0;
#endif // _WIN32
}

#if defined(__cplusplus)
}
#endif


#endif // LOADER_IMPLEMENTATION