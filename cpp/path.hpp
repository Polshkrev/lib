#ifndef PATH_HPP
#define PATH_HPP

#include <string> // std::string

namespace polutils
{
    namespace fayl
    {
        /*
        * @brief Representation of a path on the file system.
        */
        struct path_t
        {
            /*
            * @brief Initialize a new path.
            * @returns A new path storing the current working directory.
            */
            explicit path_t(void) noexcept;

            /*
            * @brief Initialize a new path from a given raw value.
            * @param raw Raw string path value to initialize the path.
            * @returns A new path storing the given raw parametre.
            */
            explicit path_t(const std::string &path) noexcept;
            
            /*
            * @brief Determine if the path exists on the filesystem.
            * @returns True if the path is evaluated to exist on the filesystem, else false.
            */
            bool exists(void) const noexcept;

            /*
            * @brief Obtain the absolute value of the given path.
            * @returns A new path storing the absolute value of the given path parametre.
            * @exception If the given path does not exist, a `FileNotFoundError` is printed to `stderr` and the programme exits.
            * @exception If the absolute path value can not be obtained, an `IOError` is printed to `stderr` and the programme exits.
            */
            path_t absolute(void) const;

            /*
            * @brief Obtain the string representation of the given path value.
            * @returns A string representation of the given path value.
            */
            const char *to_string(void) const noexcept;

            /*
            * @brief Append a path to another.
            * @param child Source path from which to append.
            * @returns A new path constructed of the original path and the given chid.
            */
            path_t operator/(const path_t &child) const noexcept;

            /*
            * @brief Append a given string to a given path.
            * @param child String from which to append.
            * @returns A new path constructed of the original path and the given chid.
            */
            path_t operator/(const std::string &child) const noexcept;
            private:
                std::string __path;
        };
    }
}

#endif // PATH_HPP

#ifdef PATH_IMPLEMENTATION

namespace
{
#ifdef _WIN32
    /*
    * @brief OS-dependant path seperator.
    */
    #define PATH_SEPERATOR "\\"
#else
    /*
    * @brief OS-dependant path seperator.
    */
    #define PATH_SEPERATOR "/"
#endif // _WIN32
}

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// #include <minwinbase.h> // DWORD
// #include <winbase.h> // CopyFile
// #include <fileapi.h> // GetFullPathName, GetFileAttributes, INVALID_FILE_ATTRIBUTES
// #include <errhandlingapi.h> // GetLastError
#else
#include <sys/stat.h> // struct stat, stat, S_ISDIR
#endif // _WIN32

#define EXCEPTIONS_IMPLEMENTATION
#include "exceptions.hpp"

#define BUFFER_IMPLEMENTATION
#include "../c/buffer.h"

namespace polutils
{
    namespace fayl
    {
        /*
        * @brief Initialize a new path.
        * @returns A new path storing the current working directory.
        */
        path_t::path_t(void) noexcept
        {
            __path = ".";
        }

        /*
        * @brief Initialize a new path from a given raw value.
        * @param raw Raw string path value to initialize the path.
        * @returns A new path storing the given raw parametre.
        */
        path_t::path_t(const std::string &path) noexcept
        {
            __path = path;
        }

        /*
        * @brief Determine if the path exists on the filesystem.
        * @returns True if the path is evaluated to exist on the filesystem, else false.
        */
        bool path_t::exists(void) const noexcept
        {
        #ifdef _WIN32
            DWORD dwAttributes = GetFileAttributes(to_string());
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

        /*
        * @brief Obtain the absolute value of the given path.
        * @returns A new path storing the absolute value of the given path parametre.
        * @exception If the given path does not exist, a `FileNotFoundError` is thrown.
        * @exception If the absolute path value can not be obtained, an `IOError` is thrown.
        */
        path_t path_t::absolute(void) const
        {
            if (!exists())
            {
                throw FileNotFoundError(buffer_sprintf("File `%s` does not exist.", to_string()));
            }
            // size_t checkpoint = buffer_save();
            char *buffer = static_cast<char *>(buffer_allocate(MAX_PATH));
        #ifdef _WIN32
            if (GetFullPathName(to_string(), MAX_PATH, buffer, NULL) == 0)
            {
                throw IOError(buffer_sprintf("Can not get absolute path: %ld\n", GetLastError()));
            }
        #else
            if (NULL == realpath(to_string(), buffer))
            {
                throw IOError("Can not get absolute path: %s\n", strerror(errno));
            }
        #endif // _WIN32
            // buffer_rewind(checkpoint);
            return path_t(buffer);
        }

        /*
        * @brief Obtain the string representation of the given path value.
        * @returns A string representation of the given path value.
        */
        const char *path_t::to_string(void) const noexcept
        {
            return __path.c_str();
        }

        /*
        * @brief Append a path to another.
        * @param child Source path from which to append.
        * @returns A new path constructed of the original path and the given chid.
        */
        path_t path_t::operator/(const path_t &child) const noexcept
        {
            return path_t(buffer_sprintf("%s%c%s", to_string(), *PATH_SEPERATOR, child.to_string()));
        }

        /*
        * @brief Append a given string to a given path.
        * @param child String from which to append.
        * @returns A new path constructed of the original path and the given chid.
        */
        path_t path_t::operator/(const std::string &child) const noexcept
        {
            return path_t(buffer_sprintf("%s%c%s", to_string(), *PATH_SEPERATOR, child.c_str()));
        }
    }
}

#endif // PATH_IMPLEMENTATION