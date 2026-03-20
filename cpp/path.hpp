#ifndef PATH_HPP
#define PATH_HPP

#include <string> // std::string

#include "printable.hpp"

namespace polutils
{
    namespace fayl
    {
        /**
         * @brief Initialize a new path.
         * @returns A new path storing the current working directory.
         */
        struct path_t : public printable_t
        {
            /**
             * @brief Initialize a new path.
             * @returns A new path storing the current working directory.
             */
            explicit path_t(void) noexcept;

            /**
             * @brief Initialize a new path from a given raw value.
             * @param raw Raw string path value to initialize the path.
             * @returns A new path storing the given raw parametre.
             */
            explicit path_t(const std::string &path) noexcept;
            
            /**
             * @brief Determine if the path exists on the filesystem.
             * @returns True if the path is evaluated to exist on the filesystem, else false.
             */
            bool exists(void) const noexcept;

            /**
             * @brief Obtain the absolute value of the given path.
             * @returns A new path storing the absolute value of the given path parametre.
             * @exception If the given path does not exist, a `FileNotFoundError` is thrown.
             * @exception If the absolute path value can not be obtained, an `IOError` is thrown.
             */
            path_t absolute(void) const;

            /**
             * @brief Obtain the parent directory of the given path.
             * @returns A new path constructed from the parent directory of the given path.
             * @exception If the given path does not exist, a `FileNotFoundError` is thrown.
             * @exception If the absolute path value can not be obtained, an `IOError` is thrown.
             */
            path_t parent(void) const;

            /**
             * @brief Obtain the root of the filesystem.
             * @returns A new path containing the root of the filesystem. If the root can not be obtained the original unchanged path is returned.
             * @exception If the given path does not exist, a `FileNotFoundError` is thrown.
             * @exception If the absolute path value can not be obtained, an `IOError` is thrown.
             */
            path_t root(void) const;

            /**
             * @brief Obtain the string representation of the given path value.
             * @returns A string representation of the given path value.
             */
            const char *to_string(void) const noexcept;

            /**
             * @brief Append a path to another.
             * @param child Source path from which to append.
             * @returns A new path constructed of the original path and the given chid.
             */
            path_t operator/(const path_t &child) const noexcept;

            /**
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
}

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// #include <minwinbase.h> // DWORD
// #include <winbase.h> // CopyFile
// #include <fileapi.h> // GetFullPathName, GetFileAttributes, INVALID_FILE_ATTRIBUTES
// #include<errhandlingapi.h> // GetLastError
#else
#include <sys/stat.h> // struct stat, stat, S_ISDIR
#endif // _WIN32

#define EXCEPTIONS_IMPLEMENTATION
#include "exceptions.hpp"

namespace
{
    char __path_buffer[_MAX_PATH] = {0};
}

namespace
{
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
}

namespace polutils
{
    namespace fayl
    {
        /**
         * @brief Initialize a new path.
         * @returns A new path storing the current working directory.
         */
        path_t::path_t(void) noexcept
        {
            __path = ".";
        }

        /**
         * @brief Initialize a new path from a given raw value.
         * @param raw Raw string path value to initialize the path.
         * @returns A new path storing the given raw parametre.
         */
        path_t::path_t(const std::string &path) noexcept
        {
            __path = path;
        }

        /**
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

        /**
         * @brief Obtain the absolute value of the given path.
         * @returns A new path storing the absolute value of the given path parametre.
         * @exception If the given path does not exist, a `FileNotFoundError` is thrown.
         * @exception If the absolute path value can not be obtained, an `IOError` is thrown.
         */
        path_t path_t::absolute(void) const
        {
            if (!exists())
            {
                throw FileNotFoundError("File `%s` does not exist.", to_string());
            }
        #ifdef _WIN32
            if (GetFullPathName(to_string(), MAX_PATH, __path_buffer, NULL) == 0)
            {
                throw IOError("Can not get absolute path: %ld\n", GetLastError());
            }
        #else
            if (NULL == realpath(to_string(), buffer))
            {
                throw IOError(buffer_sprintf("Can not get absolute path: %s\n", strerror(errno)));
            }
        #endif // _WIN32
            return path_t(__path_buffer);
        }
        
        /**
         * @brief Obtain the parent directory of the given path.
         * @returns A new path constructed from the parent directory of the given path.
         * @exception If the given path does not exist, a `FileNotFoundError` is thrown.
         * @exception If the absolute path value can not be obtained, an `IOError` is thrown.
         */
        path_t path_t::parent(void) const
        {
            path_t abs = absolute();
            ssize_t last_stroke = _find_last_stroke(abs.to_string());
            if (last_stroke < 0)
            {
                return *this;
            }
            for (ssize_t i = 0; i < last_stroke; ++i)
            {
                __path_buffer[i] = abs.__path[i];
            }
            __path_buffer[last_stroke] = '\0';
            return path_t(__path_buffer);
        }

        /**
         * @brief Obtain the root of the filesystem.
         * @param path Path from which to obtain its root.
         * @returns A new path containing the root of the filesystem. If the root can not be obtained the original unchanged path is returned.
         * @exception If the given path does not exist, a `FileNotFoundError` is thrown.
         * @exception If the absolute path value can not be obtained, an `IOError` is thrown.
         */
        path_t path_t::root(void) const
        {
        #ifdef _WIN32
            path_t abs = absolute();
            ssize_t first_stroke = _find_first_stroke(abs.to_string());
            if (first_stroke < 0)
            {
                return *this;
            }
            for (ssize_t i = 0; i < first_stroke; ++i)
            {
                __path_buffer[i] = abs.__path[i];
            }
            __path_buffer[first_stroke] = '\0';
            return path_t(__path_buffer);
        #else
            return path_t("/");
        #endif // _WIN32
        }

        /**
         * @brief Obtain the string representation of the given path value.
         * @returns A string representation of the given path value.
         */
        const char *path_t::to_string(void) const noexcept
        {
            return __path.c_str();
        }

        /**
         * @brief Append a path to another.
         * @param child Source path from which to append.
         * @returns A new path constructed of the original path and the given chid.
         */
        path_t path_t::operator/(const path_t &child) const noexcept
        {
            std::sprintf(__path_buffer, "%s%c%s", to_string(), PATH_SEPERATOR, child.to_string());
            return path_t(__path_buffer);
        }

        /**
         * @brief Append a given string to a given path.
         * @param child String from which to append.
         * @returns A new path constructed of the original path and the given chid.
         */
        path_t path_t::operator/(const std::string &child) const noexcept
        {
            std::sprintf(__path_buffer, "%s%c%s", to_string(), PATH_SEPERATOR, child.c_str());
            return path_t(__path_buffer);
        }
    }
}

#endif // PATH_IMPLEMENTATION