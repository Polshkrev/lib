#ifndef ENTRY_HPP
#define ENTRY_HPP

#include "doc.hpp" // ! NAMESPACE DOCUMENTATION HEADER

#include <cstddef> // std::size_t

#include <string> // std::string

#define PATH_IMPLEMENTATION
#include "path.hpp" // path_t

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
        constexpr const char *type_to_string(type_t type);

        /**
         * @brief Entry on the filesystem.
         */
        struct entry_t : public printable_t
        {
            /**
             * @brief Construct a new entry based on the current working directory.
             */
            entry_t(void);

            /**
             * @brief Construct a new entry with a given path value.
             * @param path Path to link to the entry.
             */
            entry_t(const path_t &path) noexcept;

            /**
             * @brief Construct an entry based on a given type.
             * @param type Type of the entry to construct.
             */
            entry_t(type_t type);

            /**
             * @brief Construct an entry based on its given path and type.
             * @param path Path of the entry.
             * @param type Type of the entry.
             */
            entry_t(const path_t &path, type_t type) noexcept;

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
             * @exception If the file can not be written, an `IOError` is thrown.
             */
            void write(const std::string &content) const;

            /**
             * @brief Write the internal content to the given path.
             * @param path Path to which to write the internal content.
             * @exception If the file can not be written, an `IOError` is thrown.
             */
            void write(const printable_t &path) const;

            /**
             * @brief Move a file on the filesystem to a given destination.
             * @param destination Destination to which to move the entry.
             * @exception If the file can not be moved, an `IOError` is thrown.
             */
            void move(const entry_t &destination) const;

            /**
             * @brief Copy an entry to a given destination.
             * @param destination Destination to which to copy the entry.
             * @exception If the source path does not exist on the filesystem, a `FileNotFoundError` is thrown.
             * @exception If the destination path already exists on the filesystem, a `FileExistsError` is thrown.
             * @exception If the type of either the destination or source is not of type `type_t::FILE`, then an `IOError` is thrown.
             * @exception If the file can not be copied, an `IOError` is thrown.
             */
            void copy(const entry_t &destination) const;

            /**
             * @brief Remove an entry on the filesystem.
             * @exception If the entry is not of type `type_t::FILE`, an `IOError` is thrown.
             * @exception If the internal path does not exist, a `FileNotFoundError` is thrown.
             * @exception If the file can not be removed, an `IOError` is thrown.
             */
            void remove(void) const;

            /**
             * @brief Obtain the path property linked to the entry.
             * @returns The path value linked to the entry.
             */
            const path_t &path(void) const noexcept;

            /**
             * @brief Set the path of the entry.
             * @param path Path to set for the entry.
             */
            void set_path(const path_t &path) noexcept;

            /**
             * @brief Assign the type given to the file entry.
             * @param type Type to set to the entry.
             */
            void set_type(type_t type) noexcept;

            /**
             * @brief Obtain the type of the entry.
             * @returns The type of the entry.
             */
            type_t type(void) const noexcept;

            /**
             * @brief Obtain the content of the entry.
             * @returns The content of the entry as a string.
             */
            const std::string &content(void) const noexcept;

            /**
             * @brief Set the content of the entry.
             * @param content Content to set for the entry.
             */
            void set_content(const std::string &content) noexcept;

            /**
             * @brief Determine if the entry is of a given type.
             * @param type Type to check against the entry type.
             * @returns True if the entry is of the given type, else false.
             */
            bool is(type_t type) const noexcept;

            /**
             * @brief Obtain the size of the file.
             * @returns The size of the file.
             * @exception If the entry is not a file, a `ValueError` is thrown.
             */
            std::size_t size(void) const;

            /**
             * @brief Determine if the file is empty.
             * @returns True if the contents of the file are empty, else false.
             */
            bool is_empty(void) const;

            /**
             * @brief Obtain a string representation of the entry.
             * @returns A string representation of the entry.
             */
            const char *to_string(void) const noexcept;
            private:

                /**
                 * @brief Completely construct a new entry based on a given path, content, and type.
                 * @param path Path of the entry.
                 * @param content Content of the entry.
                 * @param type Type of the entry.
                 */
                entry_t(const path_t &path, const std::string &content, type_t type) noexcept;

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

#include <cstdio>    // std::remove, std::rename
#include <fstream>   // std::ifstream, std::ofstream
#include <utility> // std::move

#ifdef _WIN32
    #include <fileapi.h> // GetFileAttributesEx, CopyFile
    #include <minwinbase.h> // GetFileExInfoStandard
    #include <winnt.h> // ULARGE_INTEGER
    #include <direct.h> // _mkdir
#else
    #include <fcntl.h> // open
    #include <sys/stat.h> // mkdir
    #include <unistd.h> // close, read, write
#endif // _WIN32

namespace
{
    /**
     * @brief Write the given content to the given path.
     * @param path Path to which to write.
     * @param content Content to write to the given path.
     * @exception If the file can not be written, an `IOError` is thrown.
     */
    void write_content_to_path(const polutils::printable_t &path, const std::string &content)
    {
        std::ofstream file(path.to_string(), std::ios::binary);
        if (!file.is_open())
        {
            throw polutils::IOError("Can not open file '%s' for writing.", path.to_string());
        }

        file << content;

        if (file.fail())
        {
            throw polutils::IOError("Can not write to file '%s'.", path.to_string());
        }
    }

    /**
     * @brief Obtain the file type of the given path. If the path does not exist, it can not be analysed.
     * @param path Path to analyse.
     * @returns The file type of the given path.
     */
    polutils::fayl::type_t _get_file_type(const char *path)
    {
    #ifdef _WIN32
        DWORD attribute = GetFileAttributes(path);
        if (attribute == INVALID_FILE_ATTRIBUTES) return polutils::fayl::type_t::NONE;
        else if (attribute & FILE_ATTRIBUTE_DIRECTORY) return polutils::fayl::type_t::DIRECTORY;
        return polutils::fayl::type_t::FILE;
    #else // _WIN32
        struct stat statbuf;
        if (lstat(path, &statbuf) < 0) return polutils::fayl::type_t::NONE;
        else if (S_ISREG(statbuf.st_mode)) return polutils::fayl::type_t::FILE;
        else if (S_ISDIR(statbuf.st_mode)) return polutils::fayl::type_t::DIRECTORY;
        return polutils::fayl::type_t::NONE;
    #endif // _WIN32
    }
    
    #ifndef __IO_ENTRY_BUFFER_CAPACITY
    /**
     * @brief Buffer capacaity of the IO operations within the entry functions.
     */
    #define __IO_ENTRY_BUFFER_CAPACITY (10 * 1024)
    #endif // __IO_ENTRY_BUFFER_CAPACITY
    
    /**
     * @brief Copy a file from one path to another.
     * @param source Path of the source file.
     * @param destination Path of the destination file.
     * @exception If the file can not be copied, an `IOError` is thrown.
     */
    void copy_file(const char *source, const char *destination)
    {
    #ifdef _WIN32

        if (!CopyFile(source, destination, TRUE))
        {
            throw polutils::IOError("Can not copy file '%s' to '%s'.", source, destination);
        }
    #else

        int source_file = open(source, O_RDONLY);

        if (source_file == -1) throw polutils::IOError("Can not open file '%s'.", source);

        int destination_file = open(destination, O_WRONLY | O_CREAT | O_EXCL, 0644);

        if (destination_file == -1)
        {
            close(source_file);
            throw polutils::IOError("Can not create file '%s'.", destination);
        }

        char buffer[__IO_ENTRY_BUFFER_CAPACITY];
        ssize_t count;

        while ((count = read(source_file, buffer, sizeof(buffer))) > 0)
        {
            ssize_t written = 0;

            while (written < count)
            {
                ssize_t result = write(destination_file, buffer + written, count - written);

                if (result == -1)
                {
                    close(source_file);
                    close(destination_file);
                    throw polutils::IOError("Can not write file '%s'.", destination);
                }

                written += result;
            }
        }

        close(source_file);
        close(destination_file);

        if (count == -1) throw polutils::IOError("Can not read file '%s'.", source);

    #endif // _WIN32
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
        constexpr const char *type_to_string(type_t type)
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
         * @brief Construct a new entry based on the current working directory.
         */
        entry_t::entry_t(void) : entry_t(path_t(), std::string(), type_t::DIRECTORY) {}

        /**
         * @brief Construct a new entry with a given path value.
         * @param path Path to link to the entry.
         */       
        entry_t::entry_t(const path_t &path) noexcept : entry_t(path, std::string(), _get_file_type(path.to_string())) {}

        /**
         * @brief Construct an entry based on a given type.
         * @param type Type of the entry to construct.
         */
        entry_t::entry_t(type_t type) : entry_t(path_t(), std::string(), type) {}

        /**
         * @brief Construct an entry based its given path and type.
         * @param path Path of the entry.
         * @param type Type of the entry.
         */
        entry_t::entry_t(const path_t &path, type_t type) noexcept : entry_t(path, std::string(), type) {}

        /**
         * @brief Completely construct a new entry based on a given path, content, and type.
         * @param path Path of the entry.
         * @param content Content of the entry.
         * @param type Type of the entry.
         */
        entry_t::entry_t(const path_t &path, const std::string &content, type_t type) noexcept : __path(path), __content(content), __type(type) {}

        /**
         * @brief Create a file on the filesystem.
         * @exception If the file already exists, a `FileExistsError` is thrown.
         * @exception If the file's type is not a `type_t::FILE`, an `IOError` is thrown.
         * @exception If the file can not be opened, an `IOError` is thrown.
         */
        void entry_t::touch(void) const
        {
            if (__path.exists())
            {
                throw FileExistsError("File '%s' already exists.", __path.to_string());
            }
            else if (__type != type_t::FILE)
            {
                throw IOError("Can not create file '%s'.", __path.to_string());
            }

            std::ofstream file(__path.to_string(), std::ios::binary);

            if (!file.is_open())
            {
                throw IOError("Can not create file '%s'.", __path.to_string());
            }
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
                throw IOError("Can not make a directory from a file: %s.", __path.to_string());
            }
        #ifdef _WIN32
            // direct.h
            int result = _mkdir(__path.to_string());
        #else
            int result = ::mkdir(__path.to_string(), 0755);
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
         */
        void entry_t::read(void)
        {
            if (__type != type_t::FILE)
            {
                throw IOError("Can not read directory: '%s'.", __path.to_string());
            }

            std::ifstream file(__path.to_string(), std::ios::binary);

            if (!file.is_open())
            {
                throw FileNotFoundError("Can not open file '%s'.", __path.to_string());
            }

            std::size_t file_size = size();

            std::string content(file_size, '\0');

            if (file_size > 0)
            {
                file.read(content.data(), static_cast<std::streamsize>(file_size));

                if (!file)
                {
                    throw IOError("Can not read file '%s'.", __path.to_string());
                }
            }

            __content = std::move(content);
        }

        /**
         * @brief Write the given content to the internal path.
         * @param content Content to write to the internal path.
         * @exception If the file can not be written, an `IOError` is thrown.
         */
        void entry_t::write(const std::string &content) const
        {
            write_content_to_path(__path, content);
        }

        /**
         * @brief Write the internal content to the given path.
         * @param path Path to which to write the internal content.
         * @exception If the file can not be written, an `IOError` is thrown.
         */
        void entry_t::write(const printable_t &path) const
        {
            write_content_to_path(path, __content);
        }

        /**
         * @brief Move a file on the file system to a given destination.
         * @param destination Destination to which to move the entry.
         * @exception If the source path does not exist on the file system, a `FileNotFoundError` is thrown.
         * @exception If the destination path already exists on the file system, a `FileExistsError` is thrown.
         * @exception If the entry is not a file, an `IOError` is thrown.
         * @exception If the destination is of a directory, an `IOError` is thrown.
         * @exception If the file can not be moved, an `IOError` is thrown.
         */
        void entry_t::move(const entry_t &destination) const
        {
            if (!__path.exists())
            {
                throw FileNotFoundError("'%s' does not exist.", __path.to_string());
            }
            else if (destination.path().exists())
            {
                throw FileExistsError("File '%s' already exists.", destination.path().to_string());
            }
            else if (!is(type_t::FILE))
            {
                throw IOError("Can not move a directory: '%s'.", __path.to_string());
            }
            else if (destination.is(type_t::DIRECTORY))
            {
                throw IOError("Can not move file '%s' to directory '%s'.", destination.path().to_string(), type_to_string(destination.type()));
            }
            else if (std::rename(__path.to_string(), destination.path().to_string()) != 0)
            {
                throw IOError("Can not move file '%s' to '%s'.", __path.to_string(), destination.path().to_string());
            }
        }

        /**
         * @brief Copy an entry to a given destination.
         * @param destination Destination to which to copy the entry.
         * @exception If the source path does not exist on the filesystem, a `FileNotFoundError` is thrown.
         * @exception If the destination path already exists on the filesystem, a `FileExistsError` is thrown.
         * @exception If the type of either the destination or source is not of type `type_t::FILE`, then an `IOError` is thrown.
         * @exception If the file can not be copied, an `IOError` is thrown.
         */
        void entry_t::copy(const entry_t &destination) const
        {
            if (!__path.exists())
            {
                throw FileNotFoundError("Can not find file '%s'.", __path.to_string());
            }
            else if (destination.path().exists())
            {
                throw FileExistsError("File '%s' already exists.", destination.path().to_string());
            }
            else if (!is(type_t::FILE) || !destination.is(type_t::FILE))
            {
                throw IOError("Can not copy a directory: '%s' to '%s'.", __path.to_string(), destination.path().to_string());
            }
            copy_file(__path.to_string(), destination.path().to_string());
        }

        /**
         * @brief Remove an entry on the filesystem.
         * @exception If the entry is not of type `type_t::FILE`, an `IOError` is thrown.
         * @exception If the internal path does not exist, a `FileNotFoundError` is thrown.
         * @exception If the file can not be removed, an `IOError` is thrown.
         */
        void entry_t::remove(void) const
        {
            if (!is(type_t::FILE))
            {
                throw IOError("Can not remove a directory '%s'.", __path.to_string());
            }
            else if (!__path.exists())
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
         * @brief Set the path of the entry.
         * @param path Path to set for the entry.
         */
        void entry_t::set_path(const path_t &path) noexcept
        {
            __path = path;
        }

        /**
         * @brief Set the content of the entry.
         * @param content Content to set for the entry.
         */
        void entry_t::set_content(const std::string &content) noexcept
        {
            __content = content;
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
        const path_t &entry_t::path(void) const noexcept
        {
            return __path;
        }

        /**
         * @brief Obtain the type of the entry.
         * @returns The type of the entry.
         */
        type_t entry_t::type(void) const noexcept
        {
            return __type;
        }

        /**
         * @brief Obtain the content of the entry.
         * @returns The content of the entry as a string.
         */
        const std::string &entry_t::content(void) const noexcept
        {
            return __content;
        }

        /**
         * @brief Obtain the size of the file.
         * @returns The size of the file.
         * @exception If the entry is not a file, a `ValueError` is thrown.
         */
        std::size_t entry_t::size(void) const
        {
            if (!is(type_t::FILE))
            {
                throw ValueError("Can not obtain the size of a directory.");
            }
        #ifdef _WIN32
            WIN32_FILE_ATTRIBUTE_DATA attributes;

            if (!GetFileAttributesEx(__path.to_string(), GetFileExInfoStandard, &attributes))
            {
                throw FileNotFoundError("Path '%s' does not exist.", __path.to_string());
            }

            ULARGE_INTEGER size;
            size.LowPart = attributes.nFileSizeLow;
            size.HighPart = attributes.nFileSizeHigh;

            return static_cast<std::size_t>(size.QuadPart);
        #else
            struct stat attributes;

            if (::stat(__path.to_string(), &attributes) != 0)
            {
                throw FileNotFoundError("Path '%s' does not exist.", __path.to_string());
            }
            return static_cast<std::size_t>(attributes.st_size);
        #endif
        }


        /**
         * @brief Determine if the file is empty.
         * @returns True if the contents of the file are empty, else false.
         */
        bool entry_t::is_empty(void) const
        {
            return size() == 0;
        }

        /**
         * @brief Obtain a string representation of the entry.
         * @returns A string representation of the entry.
         */
        const char *entry_t::to_string(void) const noexcept
        {
            return __path.to_string();
        }
    }
}

#endif // ENTRY_IMPLEMENTATION