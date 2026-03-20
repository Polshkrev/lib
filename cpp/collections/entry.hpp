#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <cstddef> // std::size_t

#include <string> // std::string

#define PATH_IMPLEMENTATION
#include "../path.hpp"

#include "printable.hpp"


namespace polutils
{
    namespace fayl
    {
        /**
         * @brief Finite type of an entry.
         */
        enum class type_t
        {
            NONE,
            DIRECTORY,
            FILE,
        };

        /**
         * @brief Represent a file type as a string.
         * @returns A string representation of the given file type.
         */
        const char *type_to_string(type_t type);

        /**
         * @brief Entry on the filesystem.
         */
        struct entry_t : public printable_t
        {
            /**
             * @brief Construct a new entry.
             */
            entry_t(void) noexcept;

            /**
             * @brief Construct a new entry with a given path value.
             * @param path Path to link to the entry.
             */
            entry_t(const path_t &path) noexcept;

            /**
             * @brief Create a file on the filesystem.
             * @exception If the file already exists, a `FileExistsError` is thrown.
             * @exception If the file's type is not a `type_t::FILE`, an `IOError` is thrown.
             * @exception If the file can not be opened, an `IOError` is thrown.
             */
            void touch(void) const;

            /**
             * @brief Create a directory on the filesystem.
             * @exception If the directory already exists, a `FileExistsError` is thrown.
             * @exception If the entry is not of type `type_t::DIRECTORY`, an `IOError` is thrown.
             * @exception If the directory can not be made, an `IOError` is thrown.
             */
            void mkdir(void) const;
            
            /**
             * @brief Read an entry's content based on its stored path value. Instead of returning a string, this function sets the internal content property of the entry.
             * @exception If the file does not exist on the filesystem, a `FileNotFoundError` is thrown.
             * @exception If the file can not be open, or fails in any way, an `IOError` is thrown.
             */
            void read(void);

            /**
             * @brief Write the given content to the internal path.
             * @param content Content to write to the internal path.
             * @exception If the file can not be read, an `IOError` is thrown.
             */
            void write(const std::string &content) const;

            /**
             * @brief Write the internal content to the given path.
             * @param path Path to which to write the internal content.
             * @exception If the file can not be read, an `IOError` is thrown.
             */
            void write(const printable_t &path) const;

            /**
             * @brief Move a file on the filesystem to a given destination.
             * @param destination Destination to which to move the entry.
             * @exception If the source path does not exist on the filesystem, a `FileNotFoundError` is thrown.
             * @exception If the destination path already exists on the filesystem, a `FileExistsError` is thrown.
             * @exception If the type of either the destination or soure is not of type `type_t::FILE`, then an `IOError` is thrown.
             * @exception If the source file can not be removed, an `IOError` is thrown.
             */
            void move(const entry_t &destination) const;

            /**
             * @brief Copy an entry to a given destination.
             * @param destination Destination to which to copy the entry.
             * @exception If the source path does not exist on the filesystem, a `FileNotFoundError` is thrown.
             * @exception If the destination path already exists on the filesystem, a `FileExistsError` is thrown.
             * @exception If the type of either the destination or soure is not of type `type_t::FILE`, then an `IOError` is thrown.
             */
            void copy(const entry_t &destination) const;

            /**
             * @brief Remove an entry on the filesystem.
             * @exception If the internal path does not exist, a `FileNotFoundError` is thrown.
             * @exception If the file can not be removed, an `IOError` is thrown.
             */
            void remove(void) const;

            /**
             * @brief Obtain the path property linked to the entry.
             * @returns The path value linked to the entry.
             */
            path_t path(void) const noexcept;

            /**
             * @brief Assign the type given to the file entry.
             * @param type Type to set to the entry.
             */
            void set_type(type_t type) noexcept;

            /**
             * @brief Obtain the type of the entry.
             * @returns The type of the entry.
             */
            type_t type() const noexcept;

            /**
             * @brief Determine if the entry is of a given type.
             * @param type Type to check against the entry type.
             * @returns True if the entry is of the given type, else false.
             */
            bool is(type_t type) const noexcept;

            /**
             * @brief Obtain the size of the file.
             * @returns The size of the file.
             */
            std::size_t size(void) const noexcept;

            /**
             * @brief Determine if the file if empty.
             * @returns True if the contents of the file are empty, else false.
             */
            bool is_empty(void) const noexcept;

            /**
             * @brief Obtain a string representation of the entry.
             * @returns A string representation of the entry.
             */
            const char *to_string(void) const noexcept;
            private:
                /**
                 * @brief Internal path of the entry.
                 */
                path_t __path;

                /**
                 * @brief Content of the file entry.
                 */
                std::string __content;

                /**
                 * @brief Type of the file entry.
                 */
                type_t __type;
        };
    }
}

#endif // ENTRY_HPP

#ifdef ENTRY_IMPLEMENTATION

#include <sstream> // std::stringstream
#include <fstream> // std::ifstream

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#endif // _WIN32

namespace
{
    /**
     * @brief Write the given content to the given path.
     * @param path Path to which to write.
     * @param content Content to write to the given path.
     * @exception If the file can not be read, an `IOError` is thrown.
     */
    void write_content_to_path(const polutils::printable_t &path, const std::string &content)
    {
        std::ofstream file;
        file.open(path.to_string());
        if (!file.is_open())
        {
            throw polutils::IOError("Can not write to file '%s'.", path.to_string());
        }
        file << content;
        file.close();
    }
}

namespace polutils
{
    namespace fayl
    {

        /**
         * @brief Represent a file type as a string.
         * @returns A string representation of the given file type.
         */
        const char *type_to_string(type_t type)
        {
            switch (type)
            {
                case type_t::FILE:
                {
                    return "File";
                } break;
                case type_t::DIRECTORY:
                {
                    return "Directory";
                } break;
                default:
                {
                    return "None";
                } break;
            }
        }

        /**
         * @brief Construct a new entry.
         */
        entry_t::entry_t(void) noexcept : __path(path_t()), __content(std::string()), __type(type_t::FILE){}

        /**
         * @brief Construct a new entry with a given path value.
         * @param path Path to link to the entry.
         */       
        entry_t::entry_t(const path_t &path) noexcept : __path(path), __content(std::string()), __type(type_t::FILE) {}

        /**
         * @brief Create a file on the filesystem.
         * @exception If the file already exists, a `FileExistsError` is thrown.
         * @exception If the file's type is not a `type_t::FILE`, an `IOError` is thrown.
         * @exception If the file can not be opened, an `IOError` is thrown.
         */
        void entry_t::touch() const
        {
            if (__path.exists())
            {
                throw FileExistsError("File '%s' already exists.", __path.to_string());
            }
            else if (__type != type_t::FILE)
            {
                throw IOError("Can not touch a directory: %s.", __path.to_string());
            }
            FILE *file = fopen(__path.to_string(), "w");
            if (nullptr == file)
            {
                throw IOError("Can not open file: %s.", __path.to_string());
            }
            fclose(file);
        }

        /**
         * @brief Create a directory on the filesystem.
         * @exception If the directory already exists, a `FileExistsError` is thrown.
         * @exception If the entry is not of type `type_t::DIRECTORY`, an `IOError` is thrown.
         * @exception If the directory can not be made, an `IOError` is thrown.
         */
        void entry_t::mkdir() const
        {
            if (__path.exists())
            {
                throw FileExistsError("Directory '%s' already exists.", __path.to_string());
            }
            else if (__type != type_t::DIRECTORY)
            {
                throw IOError("Can not touch a directory: %s.", __path.to_string());
            }
        #ifdef _WIN32
            int result = _mkdir(__path.to_string());
        #else
            int result = mkdir(passtr(entry->path), 0755);
        #endif
            if (result != 0)
            {
                throw IOError("Can not make directory: %s", __path.to_string());
            }
        }

        /**
         * @brief Read an entry's content based on its stored path value. Instead of returning a string, this function sets the internal content property of the entry.
         * @exception If the file does not exist on the filesystem, a `FileNotFoundError` is thrown.
         * @exception If the file can not be open, or fails in any way, an `IOError` is thrown.
         * @exception If the absolute path value can not be obtained, an `IOError` is thrown.
         */
        void entry_t::read(void)
        {
            if (!__path.exists())
            {
                throw FileNotFoundError("Path '%s' does not exist.", __path.to_string());
            }
            std::ifstream file{};
            file.open(__path.to_string());
            if (file.fail())
            {
                throw IOError("Can not open file: '%s'.", __path.to_string());
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            __content = buffer.str();
        }

        /**
         * @brief Write the given content to the internal path.
         * @param content Content to write to the internal path.
         * @exception If the file can not be read, an `IOError` is thrown.
         */
        void entry_t::write(const std::string &content) const
        {
            write_content_to_path(__path, content);
        }

        /**
         * @brief Write the internal content to the given path.
         * @param path Path to which to write the internal content.
         * @exception If the file can not be read, an `IOError` is thrown.
         */
        void entry_t::write(const printable_t &path) const
        {
            write_content_to_path(path, __content);
        }

        /**
         * @brief Move a file on the filesystem to a given destination.
         * @param destination Destination to which to move the entry.
         * @exception If the source path does not exist on the filesystem, a `FileNotFoundError` is thrown.
         * @exception If the destination path already exists on the filesystem, a `FileExistsError` is thrown.
         * @exception If the type of either the destination or soure is not of type `type_t::FILE`, then an `IOError` is thrown.
         * @exception If the source file can not be removed, an `IOError` is thrown.
         */
        void entry_t::move(const entry_t &destination) const
        {
            copy(destination);
            remove();
        }

        /**
         * @brief Copy an entry to a given destination.
         * @param destination Destination to which to copy the entry.
         * @exception If the source path does not exist on the filesystem, a `FileNotFoundError` is thrown.
         * @exception If the destination path already exists on the filesystem, a `FileExistsError` is thrown.
         * @exception If the type of either the destination or soure is not of type `type_t::FILE`, then an `IOError` is thrown.
         */
        void entry_t::copy(const entry_t &destination) const
        {
            if (!__path.exists())
            {
                throw FileNotFoundError("Can not find file %s.\n", __path.to_string());
            }
            else if (destination.path().exists())
            {
                throw FileExistsError("File '%s' already exists.\n", destination.path().to_string());
            }
            else if ((__type != type_t::FILE) || destination.type() != type_t::FILE)
            {
                throw IOError("Can not copy a directory: '%s' to '%s'.", __path.to_string(), destination.path().to_string());
            }
            write(destination.path());
        }

        /**
         * @brief Remove an entry on the filesystem.
         * @exception If the internal path does not exist, a `FileNotFoundError` is thrown.
         * @exception If the file can not be removed, an `IOError` is thrown.
         */
        void entry_t::remove(void) const
        {
            if (!__path.exists())
            {
                throw FileNotFoundError("Can not find file '%s'", __path.to_string());
            }
            else if (std::remove(__path.to_string()) != 0)
            {
                throw IOError("Can not remove file '%s'.", __path.to_string());
            }
        }

        /**
         * @brief Assign the type given to the file entry.
         * @param type Type to set to the entry.
         */
        void entry_t::set_type(type_t type) noexcept
        {
            __type = type;
        }

        /**
         * @brief Determine if the entry is of a given type.
         * @param type Type to check against the entry type.
         * @returns True if the entry is of the given type, else false.
         */
        bool entry_t::is(type_t type) const noexcept
        {
            return __type == type;
        }

        /**
         * @brief Obtain the path property linked to the entry.
         * @returns The path value linked to the entry.
         */
        path_t entry_t::path(void) const noexcept
        {
            return __path;
        }

        /**
         * @brief Obtain the type of the entry.
         * @returns The type of the entry.
         */
        type_t entry_t::type() const noexcept
        {
            return __type;
        }

        /**
         * @brief Obtain the size of the file.
         * @returns The size of the file.
         */
        std::size_t entry_t::size(void) const noexcept
        {
            return __content.length();
        }

        /**
         * @brief Determine if the file if empty.
         * @returns True if the contents of the file are empty, else false.
         */
        bool entry_t::is_empty(void) const noexcept
        {
            return __content.empty();
        }

        /**
         * @brief Obtain a string representation of the entry.
         * @returns A string representation of the entry.
         */
        const char *entry_t::to_string(void) const noexcept
        {
            return __content.c_str();
        }
    }
}

#endif // ENTRY_IMPLEMENTATION