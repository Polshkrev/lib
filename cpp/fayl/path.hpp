#ifndef PATH_HPP
#define PATH_HPP

#include "doc.hpp" // ! NAMESPACE DOCUMENTATION HEADER

#include <cstddef> // size_t, ptrdiff_t

#include "../printable.hpp" // printable_t

#include <string> // std::string

#ifdef _WIN32
    #include <minwindef.h> // MAX_PATH
    #define PATH_MAXIMUM MAX_PATH
#else
    #include <limits.h> // PATH_MAX
    #define PATH_MAXIMUM PATH_MAX
#endif

namespace polutils
{
    namespace fayl
    {
        /**
         * @brief Representation of a filesystem path.
         */
        struct path_t : public printable_t
        {
            /**
             * @brief Initialize a new path.
             * @returns A new path storing the current working directory.
             * @exception If the current directory can not be obtained, an `IOError` is thrown.
             */
            explicit path_t(void);

            /**
             * @brief Initialise a new path from a given raw value.
             * @param path Raw string value with which to initialise the path.
             * @returns A new path storing the given raw parameter.
             * @exception If the path can not be constructed, a `ValueError` is thrown.
             */
            explicit path_t(const char *path);

            /**
             * @brief Initialise a new path from a given raw value.
             * @param path Raw string value with which to initialise the path.
             * @returns A new path storing the given raw parameter.
             * @exception If the path can not be constructed, a `ValueError` is thrown.
             */
            explicit path_t(const std::string &path);

            /**
             * @brief Determine if the path exists on the filesystem.
             * @returns True if the path exists on the filesystem, else false.
             */
            bool exists(void) const noexcept;

            /**
             * @brief Obtain the absolute value of the path.
             * @returns A new path storing the absolute value of the path parameter.
             * @exception If the absolute path value can not be obtained, an `IOError` is thrown.
             */
            path_t absolute(void) const;

            /**
             * @brief Obtain the filename of the path.
             * @returns The filename of the path.
             * @exception If the filename can not be obtained, a `ValueError` is thrown.
             */
            path_t filename(void) const;

            /**
             * @brief Obtain the extension of the path.
             * @returns The extension of the given path.
             * @returns If the path extension can not be obtained, a `nullptr` is returned.
             */
            const char *extension(void) const noexcept;

            /**
             * @brief Obtain the parent directory of the path.
             * @returns A new path constructed from the parent directory of the path.
             * @exception If the parent path can not be obtained, a `ValueError` is thrown.
             */
            path_t parent(void) const;

            /**
             * @brief Obtain the root of the filesystem.
             * @returns A new path containing the root of the filesystem. If the root can not be obtained the original unchanged path is returned.
             * @exception If the absolute path can not be obtained, an `IOError` is thrown.
             * @exception If the root path can not be obtained, a `ValueError` is thrown.
             */
            path_t root(void) const;

            /**
             * @brief Obtain the string representation of the path.
             * @returns A string representation of the path.
             */
            const char *to_string(void) const noexcept override;

            /**
             * @brief Append a path to another.
             * @param child child path from which to append.
             * @returns A new path constructed of the given child appended to the original path.
             * @exception If the path can not be appended, a `ValueError` is thrown.
             */
            path_t operator/(const path_t &child) const;

            /**
             * @brief Append a given c-string to a path.
             * @param child C-string from which to append.
             * @returns A new path constructed of the given child and the original path.
             * @exception If the path can not be appended, a `ValueError` is thrown.
             */
            path_t operator/(const std::string &child) const;

            /**
             * @brief Append a given c-string to a path.
             * @param child C-string from which to append.
             * @returns A new path constructed of the given child and the original path.
             * @exception If the path can not be appended, a `ValueError` is thrown.
             */
            path_t operator/(const char *child) const;

            private:
                /**
                 * @brief Construct a new path with a given raw string and size.
                 * @param raw The raw string representation of the path.
                 * @param size The length of the string excluding the NULL terminator.
                 * @returns A path containing the given raw string and size.
                 * @exception If the given size is greater than the maximum allowed size, a `ValueError` is thrown.
                 * @exception If the given parts can not be written to a path, a `ValueError` is thrown.
                 * @exception If the amount written to the path is not equal to the given size, a `ValueError` is thrown.
                 */
                path_t __with_size(const char *raw, std::size_t size) const;

                /**
                 * @brief Construct a new path of the given raw string until the given size.
                 * @param raw The raw string representation of the path.
                 * @param size The total length of characters to set within the path excluding the NULL terminator.
                 * @return A path containing the specified prefix.
                 * @exception If the given size is greater than the maximum allowed size, a `ValueError` is thrown.
                 * @exception If the given parts can not be written to a path, a `ValueError` is thrown.
                 * @exception If the amount written to the path is not equal to the given size, a `ValueError` is thrown.
                 */
                path_t __with_prefix(const char *raw, std::size_t size) const;

                /**
                 * @brief The size of the current path buffer.
                 */
                std::size_t __count;

                /**
                 * @brief The current sized path buffer.
                 */
                char __path[PATH_MAXIMUM];
        };

        /**
         * @brief Append a given string to a given path.
         * @param operand String to which to append.
         * @param path Path from which to append to the string.
         * @returns A new path constructed of the string operand and the given chid path.
         */
        path_t operator/(const std::string &operand, const path_t &path);

        /**
         * @brief Append a given string to a given path.
         * @param operand String to which to append.
         * @param path Path from which to append to the string.
         * @returns A new path constructed of the string operand and the given chid path.
         */
        path_t operator/(const char *operand, const path_t &path);
    }
}

#endif // PATH_HPP

#ifdef PATH_IMPLEMENTATION

#include <cstdio> // std::snprintf, std::sprintf
#include <cstdlib> // std::realpath
#include <cstring> // std::strlen, std::strerror
#include <cctype> // std::isspace
#include <cerrno> // errno

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

#define EXCEPTIONS_IMPLEMENTATION
#include "../exceptions.hpp" // IOError, ValueError

namespace
{
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
}

namespace
{
    /**
     * @brief Find the last occurrence of a character within a string.
     * @param string String within which to search.
     * @param character Character for which to search.
     * @returns The index of the last occurrence of the given character.
     * @returns If the given character does not occur within the string, `-1` is returned.
     */
    static ptrdiff_t __find_last_character(const char *string, char character) noexcept
    {
        size_t count = std::strlen(string);

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
    static ptrdiff_t __find_first_character(const char *string, char character) noexcept
    {
        size_t count = std::strlen(string);

        for (size_t index = 0; index < count; index++)
        {
            if (string[index] != character) continue;
            return (ptrdiff_t)index;
        }

        return -1;
    }

    #ifdef _WIN32

    #ifndef WIN32_PATH_ERROR_MESSAGE_SIZE
    #define WIN32_PATH_ERROR_MESSAGE_SIZE (4 * 1024)
    #endif // WIN32_PATH_ERROR_MESSAGE_SIZE

    /**
     * @brief Format a `Windows` error code as a string.
     * @param error Error code to format.
     * @returns The given error code formatted as a string.
     */
    static char *__path_error_message_windows(DWORD error) noexcept
    {
        static char win32_error_message[WIN32_PATH_ERROR_MESSAGE_SIZE] = {0};
        DWORD error_message_size = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, LANG_USER_DEFAULT, win32_error_message, WIN32_PATH_ERROR_MESSAGE_SIZE, NULL);
        if (error_message_size == 0)
        {
            if (GetLastError() != ERROR_MR_MID_NOT_FOUND)
            {
                if (std::sprintf(win32_error_message, "Could not get error message for 0x%lX", error) <= 0) return NULL;
                return win32_error_message;
            }
            else if (std::sprintf(win32_error_message, "Invalid windows error code (0x%lX)", error) <= 0) return NULL;
            return win32_error_message;

        }
        while (error_message_size > 1 && std::isspace((unsigned char)win32_error_message[error_message_size - 1])) { win32_error_message[--error_message_size] = '\0'; }
        return win32_error_message;
    }
    #endif // _WIN32

}

namespace polutils
{
    /**
     * @brief Provides types and functions for working with filesystem paths, files, and directories.
     */
    namespace fayl
    {
        /**
         * @brief Construct a new path with a given raw string and size.
         * @param raw The raw string representation of the path.
         * @param size The length of the string excluding the NULL terminator.
         * @returns A path containing the given raw string and size.
         * @exception If the given size is greater than the maximum allowed size, a `ValueError` is thrown.
         * @exception If the given parts can not be written to a path, a `ValueError` is thrown.
         * @exception If the amount written to the path is not equal to the given size, a `ValueError` is thrown.
         */
        path_t path_t::__with_size(const char *raw, std::size_t size) const
        {
            path_t result{};

            if (size >= sizeof(result.__path)) throw ValueError("Raw path exceeds maximum path length.");

            const int written = std::snprintf(result.__path, sizeof(result.__path), "%s", raw);

            if (written < 0) throw ValueError("Can not write raw string to path.");
            else if (static_cast<std::size_t>(written) != size) throw ValueError("Can not guarantee the size of the raw path.");

            result.__count = size;

            return result;
        }

        /**
         * @brief Construct a new path of the given raw string until the given size.
         * @param raw The raw string representation of the path.
         * @param size The total length of characters to set within the path excluding the NULL terminator.
         * @return A path containing the specified prefix.
         * @exception If the given size is greater than the maximum allowed size, a `ValueError` is thrown.
         * @exception If the given parts can not be written to a path, a `ValueError` is thrown.
         * @exception If the amount written to the path is not equal to the given size, a `ValueError` is thrown.
         */
        path_t path_t::__with_prefix(const char *raw, std::size_t size) const
        {
            path_t result{};

            if (size >= sizeof(result.__path)) throw ValueError("The given size has exceeded the maximum allowed.");

            const int written = std::snprintf(result.__path, sizeof(result.__path), "%.*s", static_cast<int>(size), raw);

            if (written < 0) throw ValueError("Can not write raw string to path.");
            else if (static_cast<std::size_t>(written) != size) throw ValueError("The amount written to the path does not match the provided size.");

            result.__count = size;

            return result;
        }

        /**
         * @brief Initialize a new path.
         * @returns A new path storing the current working directory.
         * @exception If the current directory can not be obtained, an `IOError` is thrown.
         */
        path_t::path_t(void) : __count(0)
        {
        #ifdef _WIN32
            DWORD count = GetCurrentDirectory(sizeof(__path), __path);
            if (count == 0) throw IOError("Can not get current directory: %s\n", __path_error_message_windows(GetLastError()));
            else if (count >= sizeof(__path)) throw IOError("Current directory is too long.\n");
            __count = (size_t)count;
        #else
            if (NULL == getcwd(__path, sizeof(__path))) throw IOError("Can not get current directory: %s\n", std::strerror(errno));
            __count = std::strlen(__path);
        #endif
        }

        /**
         * @brief Initialise a new path from a given raw value.
         * @param path Raw string value with which to initialise the path.
         * @returns A new path storing the given raw parameter.
         * @exception If the path can not be constructed, a `ValueError` is thrown.
         */
        path_t::path_t(const char *path)
        {
        #ifdef _WIN32
            if (GetConsoleOutputCP() != CP_UTF8) SetConsoleOutputCP(CP_UTF8);
            if (GetConsoleCP() != CP_UTF8) SetConsoleCP(CP_UTF8);
        #endif // _WIN32
            *this = __with_size(path, std::strlen(path));
        }

        /**
         * @brief Initialise a new path from a given raw value.
         * @param path Raw string value with which to initialise the path.
         * @returns A new path storing the given raw parameter.
         * @exception If the path can not be constructed, a `ValueError` is thrown.
         */
        path_t::path_t(const std::string &path) : path_t(path.c_str()) {}

        /**
         * @brief Determine if the path exists on the filesystem.
         * @returns True if the path exists on the filesystem, else false.
         */
        bool path_t::exists(void) const noexcept
        {
        #ifdef _WIN32
            DWORD attributes = GetFileAttributes(to_string());
            return (attributes != INVALID_FILE_ATTRIBUTES);
        #else
            struct stat stat_buffer = {0};
            return stat(to_string(), &stat_buffer) == 0;
        #endif // _WIN32
        }

        /**
         * @brief Obtain the absolute value of the path.
         * @returns A new path storing the absolute value of the path parameter.
         * @exception If the absolute path value can not be obtained, an `IOError` is thrown.
         */
        path_t path_t::absolute(void) const
        {
            path_t output{};
        #ifdef _WIN32
            DWORD count = GetFullPathName(__path, PATH_MAXIMUM, output.__path, NULL);
            if (count == 0) throw IOError("Can not get absolute path: %s\n", __path_error_message_windows(GetLastError()));
            else if (count >= PATH_MAXIMUM) throw IOError("The absolute path exceeds the maximum path size allowed.\n");
            output.__count = (size_t)count;
        #else
            if (NULL == std::realpath(__path, output.__path)) throw IOError("Can not obtain absolute path: %s\n", std::strerror(errno));
            output.__count = std::strlen(output.__path);
        #endif
            return output;
        }

        /**
         * @brief Obtain the filename of the path.
         * @returns The filename of the path.
         * @exception If the filename can not be obtained, a `ValueError` is thrown.
         */
        path_t path_t::filename(void) const
        {
            ptrdiff_t separator = __find_last_character(__path, PATH_SEPARATOR);
            const char *filename = __path;

            if (separator >= 0) filename += (size_t)separator + 1;

            ptrdiff_t extension = __find_last_character(filename, '.');

            if (extension <= 0) return __with_prefix(filename, std::strlen(filename));

            return __with_prefix(filename, (size_t)extension);
        }

        /**
         * @brief Obtain the extension of the path.
         * @returns The extension of the given path.
         * @returns If the path extension can not be obtained, a `nullptr` is returned.
         */
        const char *path_t::extension(void) const noexcept
        {
            ptrdiff_t separator = __find_last_character(__path, PATH_SEPARATOR);
            const char *filename = __path;

            if (separator >= 0) filename += (size_t)separator + 1;

            ptrdiff_t extension = __find_last_character(filename, '.');

            if (extension <= 0 || filename[extension + 1] == '\0') return nullptr;

            return filename + extension + 1;
        }
        
        /**
         * @brief Obtain the parent directory of the path.
         * @returns A new path constructed from the parent directory of the path.
         * @exception If the parent path can not be obtained, a `ValueError` is thrown.
         */
        path_t path_t::parent(void) const
        {
            ptrdiff_t separator = __find_last_character(__path, PATH_SEPARATOR);

            if (separator < 0) return __with_size(".", 1);
            else if (separator == 0) return __with_prefix(__path, 1);

            return __with_prefix(__path, (size_t)separator);
        }

        /**
         * @brief Obtain the root of the filesystem.
         * @returns A new path containing the root of the filesystem. If the root can not be obtained the original unchanged path is returned.
         * @exception If the absolute path can not be obtained, an `IOError` is thrown.
         * @exception If the root path can not be obtained, a `ValueError` is thrown.
         */
        path_t path_t::root(void) const
        {
        #ifdef _WIN32
            path_t absolute_path = absolute();
            ptrdiff_t separator = __find_first_character(absolute_path.__path, PATH_SEPARATOR);
            if (separator < 0) return *this;
            return __with_prefix(absolute_path.__path, (size_t)separator);
        #else
            return __with_size("/", 1);
        #endif
        }

        /**
         * @brief Obtain the string representation of the path.
         * @returns A string representation of the path.
         */
        const char *path_t::to_string(void) const noexcept
        {
            return __path;
        }

        /**
         * @brief Append a path to another.
         * @param child child path from which to append.
         * @returns A new path constructed of the given child appended to the original path.
         * @exception If the path can not be appended, a `ValueError` is thrown.
         */
        path_t path_t::operator/(const path_t &child) const
        {
            size_t count = __count + 1 + child.__count;

            if (count >= PATH_MAXIMUM) throw ValueError("The length of the appended path has exceeded the maximum size allowed.\n");

            path_t output = *this;

            output.__path[__count] = PATH_SEPARATOR;

            int n = std::snprintf(output.__path + __count + 1, sizeof(output.__path) - this->__count - 1, "%s", child.__path);

            if ((n < 0) || ((size_t)n != child.__count)) throw ValueError("Path can not be appended.\n");

            output.__count = count;

            return output;
        }

        /**
         * @brief Append a given c-string to a path.
         * @param child C-string from which to append.
         * @returns A new path constructed of the given child and the original path.
         * @exception If the path can not be appended, a `ValueError` is thrown.
         */
        path_t path_t::operator/(const char *child) const
        {
            return ( *this / path_t(child) );
        }

        /**
         * @brief Append a given c-string to a path.
         * @param child C-string from which to append.
         * @returns A new path constructed of the given child and the original path.
         * @exception If the path can not be appended, a `ValueError` is thrown.
         */
        path_t path_t::operator/(const std::string &child) const
        {
            return ( *this / path_t(child.c_str()) );
        }

        /**
         * @brief Append a given string to a given path.
         * @param operand String to which to append.
         * @param path Path from which to append to the string.
         * @returns A new path constructed of the string operand and the given chid path.
         */
        path_t operator/(const std::string &operand, const path_t &path)
        {
            return ( path_t(operand.c_str()) / path);
        }

        /**
         * @brief Append a given string to a given path.
         * @param operand String to which to append.
         * @param path Path from which to append to the string.
         * @returns A new path constructed of the string operand and the given chid path.
         */
        path_t operator/(const char *operand, const path_t &path)
        {
            return ( path_t(operand) / path);
        }
    }
}

#endif // PATH_IMPLEMENTATION