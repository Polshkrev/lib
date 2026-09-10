# Entry
A C++ representation of an entry on the file system. An entry can represent a file, directory, or an unspecified file system entry.

## Table Of Contents
1. [Usage](#usage)
   1. [Construction](#construction)
   2. [Reading and Writing](#reading-and-writing)
   3. [Moving and Copying](#moving-and-copying)
   4. [Creation](#creation)
   5. [Access](#access)
2. [Types](#types)

## Usage
### Construction
An entry can be constructed with a path, a type, or both. If no type is provided, the entry is assigned `type_t::NONE`.
```cpp
polutils::fayl::entry_t file(polutils::fayl::path_t("example.txt"), polutils::fayl::type_t::FILE);
```

### Creation
`touch()` creates a file at the entry's path. The entry must have a type of `type_t::FILE`.
`mkdir()` creates a directory at the entry's path. The entry must have a type of `type_t::DIRECTORY`.
Both functions throw `FileExistsError` if the target already exists and an `IOError` if the operation fails.

### Reading And Writing
`read()` reads the contents of the file at the entry's path into the entry's internal content.
`write()` writes content to a path. It can either write explicitly provided content to the entry's path or write the entry's stored content to another path.
An entry can be removed from the file system using `remove()`. The entry must represent a file.

### Moving And Copying
`move()` moves the file at the entry's path to the path of another entry.
`copy()` copies a file to another entry.
Both the source and destination must have a type of `type_t::FILE`.

### Access
An entry provides access to its path, type, and stored content through `path()`, `type()`, and `content()`.
These properties can be modified with `set_path()`, `set_type()`, and `set_content()`.
`is()` can be used to determine whether an entry has a specific type.
`size()` returns the size of the file in bytes, while `is_empty()` determines whether the file has no contents.
`to_string()` returns a string representation of the entry.

## Types
The `type_t` enumeration defines the type of an entry.
| Type                | Description                       |
| ------------------- | --------------------------------- |
| `type_t::NONE`      | No type is specified.             |
| `type_t::DIRECTORY` | The entry represents a directory. |
| `type_t::FILE`      | The entry represents a file.      |

`type_to_string()` converts an entry type to its string representation.
