# Path
A C++ implementation inspired by [Python's `pathlib`](https://docs.python.org/3/library/pathlib.html).

The `path_t` type provides a representation of a file system path and operations for constructing, modifying, and inspecting it.
`path_t` inherits from `printable_t`, providing a common interface for obtaining a string representation of the path through `to_string()`.

## Table Of Contents
1. [Usage](#usage)
1. [Construction](#construction)
2. [Modification](#modification)
3. [Inspection](#inspection)

## Usage
### Construction
To construct a new path object, there are two constructor overloads available. The default constructor creates a path representing the current working directory, while the other constructor takes a string representing the path.

### Modification
To modify a path, the `/` operator is overloaded to append another path to the current path. It accepts another path object or a string. The operator returns a new path without modifying the original.

### Inspection
A path can be inspected to determine its properties and components. The library provides operations for checking whether a path exists, obtaining its absolute path, retrieving its parent and root paths, and accessing its `filename` and `extension`.
The `filename` operation returns the final component of the path without its extension, while `extension` returns the extension of the file, i.e `c`, `h`, `md`, et cetera.
The `to_string` operation provides access to the path as a null-terminated C string through the `printable_t` interface.
