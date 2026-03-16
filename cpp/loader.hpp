#ifndef LOADER_HPP
#define LOADER_HPP

#include <string> // std::string
namespace polutils
{
    /**
     * @brief Load a dynamically linked library.
     * @param path Path where the dynamically linked library is located on the filesystem.
     * @returns A handle to dynamically linked library.
     * @exception If the given path does not exist on the file system, a `FileNotFoundError` is thrown.
     * @exception If the library can not be loaded, a `FileNotFoundError` is thrown.
     */
    void *library_load(const std::string &path);

    /**
     * @brief Load a function from a given library handle with a given name.
     * @param library Handle to the library containing the function.
     * @param name Name of the function to find.
     * @returns A handle to the function found within the given library handle.
     * @exception If the function can not be loaded, a `ValueError` is thrown.
     */
    void *library_function(void *library, const std::string &name);

    /**
     * @brief Deallocate the given library handle.
     * @param library Handle to the library to which to deallocate.
     * @returns True if the given library handle was successfully deallocated, else false.
     */
    bool library_delete(void *library);
}

#endif // LOADER_HPP

#ifdef LOADER_IMPLEMENTATION

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h> // HANDLE, FARPROC, LoadLibrary, GetProcAddress, FreeLibrary, GetLastError
#else
#include <dlfcn.h> // dlopen, dlsym, dlclose, dlerror
#endif // _WIN32

#define EXCEPTIONS_IMPLEMENTATION
#include "exceptions.hpp" // FileNotFoundError, ValueError

#define BUFFER_IMPLEMENTATION
#include "../c/collections/buffer.h" // buffer_sprintf
namespace polutils
{
    /**
     * @brief Load a dynamically linked library.
     * @param path Path where the dynamically linked library is located on the filesystem.
     * @returns A handle to dynamically linked library.
     * @exception If the given path does not exist on the file system, a `FileNotFoundError` is printed and the programme exists.
     * @exception If the library can not be loaded, a `FileNotFoundError` is printed and the programme exists.
     */
    void *library_load(const std::string &path)
    {
    #ifdef _WIN32
        HANDLE lib = LoadLibrary(path.c_str());
        if (nullptr == lib)
        {
            throw FileNotFoundError(buffer_sprintf("Can not load library from path '%s'.", path.c_str()));
        }
    #else
        void *lib = dlopen(path.c_str(), RTLD_NOW);
        if (nullptr == lib)
        {
            throw FileNotFoundError(buffer_sprintf("Can not load library from path '%s': %s.", path.c_str(), dlerror()))
        }
    #endif // _WIN32
        return lib;
    }

    /**
     * @brief Load a function from a given library handle with a given name.
     * @param library Handle to the library containing the function.
     * @param name Name of the function to find.
     * @returns A handle to the function found within the given library handle.
     * @exception If the function can not be loaded, a `ValueError` is thrown.
     */
    void *library_function(void *library, const std::string &name)
    {
    #ifdef _WIN32
        void *function = (void*)GetProcAddress(static_cast<HMODULE>(library), name.c_str());
        if (nullptr == function)
        {
            library_delete(library);
            throw ValueError(buffer_sprintf("Can not load funtion '%s'.", name.c_str()));
        }
    #else
        void *function = (void*)dlsym(library, name.c_str());
        if (nullptr == function)
        {
            library_delete(library);
            throw ValueError(buffer_sprintf("ValueError: Can not load function '%s': %s.", name.c_str(), dlerror()));
        }
    #endif // _WIN32
        return function;
    }

    /**
     * @brief Deallocate the given library handle.
     * @param library Handle to the library to which to deallocate.
     * @returns True if the given library handle was successfully deallocated, else false.
     */
    bool library_delete(void *library)
    {
    #ifdef _WIN32
        return FreeLibrary(static_cast<HMODULE>(library));
    #else
        return dlclose(library) == 0;
    #endif // _WIN32
    }
}
#endif // LOADER_IMPLEMENTATION