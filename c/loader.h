#ifndef LOADER_H
#define LOADER_H

#if defined(__cplusplus)
extern "C" {
#endif

#define PATH_IMPLEMENTATION
#include "path.h" // path_t, path_exists, path_delete

/*
* @brief Load a dynamically linked library.
* @param path Path where the dynamically linked library is located on the filesystem.
* @returns A handle to dynamically linked library.
* @exception If the given path does not exist on the file system, a `FileNotFoundError` is printed and the programme exists. When the programme exists, the given path is deallocated.
* @exception If the library can not be loaded, a `FileNotFoundError` is printed and the programme exists. When the programme exists, the given path is deallocated.
*/
void *library_load(path_t *path);

/*
* @brief Load a function from a given library handle with a given name.
* @param library Handle to the library containing the function.
* @param name Name of the function to find.
* @returns A handle to the function found within the given library handle.
* @exception If the function can not be loaded, a `ValueError` is printed and the programme exists. When the programme exists, the given library is closed.
*/
void *library_function(void *library, const char *name);

/*
* @brief Deallocate the given library handle.
* @param library Handle to the library to which to deallocate.
* @returns True if the given library handle was successfully deallocated, else false.
*/
bool library_delete(void *library);

#if defined(__cplusplus)
}
#endif

#endif // LOADER_H

#ifdef LOADER_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h> // HANDLE, FARPROC, LoadLibrary, GetProcAddress, FreeLibrary, GetLastError
#else
#include <dlfcn.h> // dlopen, dlsym, dlclose, dlerror
#endif // _WIN32

/*
* @brief Load a dynamically linked library.
* @param path Path where the dynamically linked library is located on the filesystem.
* @returns A handle to dynamically linked library.
* @exception If the given path does not exist on the file system, a `FileNotFoundError` is printed and the programme exists. When the programme exists, the given path is deallocated.
* @exception If the library can not be loaded, a `FileNotFoundError` is printed and the programme exists. When the programme exists, the given path is deallocated.
*/
void *library_load(path_t *path)
{
    if (!path_exists(path))
    {
        fprintf(stderr, "FileNotFoundError: Can not load library from path '%s'.", passtr(path));
        path_delete(path);
        exit(1);
    }
#ifdef _WIN32
    HANDLE lib = LoadLibrary(passtr(path));
    if (NULL == lib)
    {
        fprintf(stderr, "FileNotFoundError: Can not load library from path '%s'.", passtr(path));
        path_delete(path);
        exit(1);
    }
#else
    void *lib = dlopen(passtr(path), RTLD_NOW);
    if (NULL == lib)
    {
        fprintf(stderr, "FileNotFoundError: Can not load library from path '%s': %s.", passtr(path), dlerror())
        path_delete(path);
        exit(1);
    }
#endif // _WIN32
    return lib;
}

/*
* @brief Load a function from a given library handle with a given name.
* @param library Handle to the library containing the function.
* @param name Name of the function to find.
* @returns A handle to the function found within the given library handle.
* @exception If the function can not be loaded, a `ValueError` is printed and the programme exists. When the programme exists, the given library is closed.
*/
void *library_function(void *library, const char *name)
{
#ifdef _WIN32
    void *function = (void*)GetProcAddress(library, name);
    if (NULL == function)
    {
        library_delete(library);
        fprintf(stderr, "ValueError: Can not load funtion '%s'.", name);
        exit(1); // ! This does not free the path value.
    }
#else
    void *function = (void*)dlsym(library, name);
    if (NULL == function)
    {
        fprintf(stderr, "ValueError: Can not load function '%s': %s.", name, dlerror());
        library_delete(library);
        exit(1); // ! This does not free the path value.
    }
#endif // _WIN32
    return function;
}

/*
* @brief Deallocate the given library handle.
* @param library Handle to the library to which to deallocate.
* @returns True if the given library handle was successfully deallocated, else false.
*/
bool library_delete(void *library)
{
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