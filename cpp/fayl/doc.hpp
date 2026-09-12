#ifndef DOC_HPP_
#define DOC_HPP_
namespace polutils
{
    /**
     * @brief File system utilities.
     *
     * The `fayl` namespace provides low-level utilities for interacting with the file system.
     *
     * The namespace contains types for representing paths, file system entries, directories, and collections of files.
     * These utilities provide a small abstraction over platform-specific file system operations.
     *
     * Types:
     *
     * The primary types provided by the namespace are:
     *
     * - `path_t` — Representation of a file system path.
     * 
     * - `entry_t` — Representation of a file system entry.
     * 
     * - `directory_t` — Representation of a directory and its entries.
     * 
     * - `files_t` — Collection of files associated with a root path.
     *
     * Paths:
     *
     * `path_t` provides operations for constructing and inspecting file system paths including:
     *
     * - Determining whether a path exists.
     * 
     * - Obtaining absolute paths.
     * 
     * - Extracting filenames and extensions.
     * 
     * - Obtaining parent and root paths.
     * 
     * - Appending path components.
     *
     * Entries:
     *
     * `entry_t` represents an individual file system entry.
     * An entry may represent a file, directory, or unspecified entry.
     *
     * Entries provide operations for:
     *
     * - Creating files and directories.
     * 
     * - Reading and writing file contents.
     * 
     * - Moving and copying entries.
     * 
     * - Removing entries.
     * 
     * - Determining entry size and emptiness.
     */
    namespace fayl {}
}
#endif // DOC_HPP_