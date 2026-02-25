# Entry
Implementation of a directory entry on a filesystem.
This is a C port of the golang [Directory](https://github.com/Polshkrev/gopolutils/blob/main/docs/en-UK/fayl/entry.md) module.
## Table of Contents
1. [Usage](#usage)
    1. [Construction](#construction)
    2. [Functionality](#functionality)
### Usage
### Construction
To construct a new entry, the `entry_init` constructor is defined that takes in a [path_t](/docs/en-UK/c/path.md), and returns a pointer to a new `entry_t` whose type is set to the `FILE_TYPE` enum and whose content is an empty initialized [string builder](/docs/en-UK/c/collections/string_builder.md).
### Functionality
To create a file there are three methods defined: 
- `entry_touch`: To create an entry of type `FILE_TYPE`.
- `entry_make_directory`: To create an entry of type `DIRECTORY_TYPE`.
- `entry_create`: A generic wrapper over the previous aforementioned methods that dispatched based on the internal `file_type_t` of the entry.

Each of the aforemnetioned methods can fail, if the entry path already exists, or if the entry is not of the corresponding type.

To remove an entry, there are a very similar set of methods defined:
- `entry_remove_file`: To remove an entry of type `FILE_TYPE`.
- `entry_remove_directory`: To create an entry of type `DIRECTORY_TYPE`.
- `entry_remove`: A generic wrapper over the previous aforementioned methods that dispatched based on the internal `file_type_t` of the entry.

Each of the aforemnetioned methods can fail, if the entry path does not exist, if the entry is not of the corresponding type, or if the entry's type can not be determined.

Each of the creation methods and removal methods are concurrently implemented and utilize concurrent-safe collections.