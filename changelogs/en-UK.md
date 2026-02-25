# Changelog
## v0.13.0 - 2026-02-25
`Added`
- C
    - `collections`
        - Added `files` module.
        - `entry`
            - Added `entry_size` function.
        - `string_builder`
            - Added `string_builder_fit` function.

`Changed`
- C
    - The `entry` module has been moved to `collections`.
    - The `buffer` module has been moved to `collections`
    - `collections`
        - Each creation function for `entry_t` have been marked as non-const.
        - Each removal function for `entry_t` have been marked as non-const.
        - Each function for `entry_t` has been properly error handled, i.e. crash states have been added where applicable.
## v0.12.0 - 2026-02-24
`Added`
- C
    - Added `entry_t` type.
    - Added `file_type_t` enum.
    - `Docs`
        - Added documentation for `entry_t`.

`Changed`
- C
    - `loader_load` no longer depends on a `path_t` to ne passed as an argument. Instead a regular c-string will suffice.The previous behaviour is the same.
## v0.11.0 - 2026-01-30
`Added`
- C
    - Added `library_load` function.
    - Added `library_function` function.
    - Added `library_delete` function.
## v0.10.0 - 2026-01-08
`Added`
- C
    - Added `path` module.
- CPP
    - Added `path` module.
    - `Exceptions`
        - `IOError` has been added.
## v0.9.0 - 2026-01-08
`Added`
- C
    - Added `buffer` module.
    - `Docs`
        - Added `buffer` documentation.
## v0.8.0 - 2026-01-06
`Added`
- CPP
    - Added the `dynamic array` module.
    - `Array`
        - Added the `length` method.
        - Added the `is_empty` method.
    - `Docs`
        - Added the `dynamic array` documentation.

`Changed`
- CPP
    - `Array`
        - All the private member variables are now protected due to implementation changes.
## v0.7.0 - 2026-01-06
`Added`
- CPP
    - Added the `sized` interface.
    - `Docs`
        - Added the `sized` documentation.
## v0.6.0 - 2026-01-03
`Added`
- C
    - Added the `string builder` module.
    - `Docs`
        - Added `string builder` documentation.
## v0.5.0 - 2026-01-02
`Added`
- CPP
    - Added the `array` module.
    - Added the `collection` module.
    - An `AllocationError` has been added.
    - An `IndexError` has been added.
    - An `OutOfRangeError` has been added.
    - `Docs`
        - Added the `array` module documentation.
        - Added the `collection` module documentation.
## v0.4.0 - 2025-12-23
`Added`
- C
    - Added the `string view` module.
    - `Docs`
        - Added the `string view` module documentation.

## v0.3.0 - 2025-12-14
`Added`
- C
    - Added the `flag` module.
    - `Docs`
        - Added `flag` module documentation.

`Changed`
- CPP
    - `ValueError`, `FileNotFoundError`, and `UnreachableError` have been moved to a seperate file: `exceptions.hpp`.
    - `Version`
        - `to_string` now returns a `std::string`.

## v0.2.0 - 2025-12-09
`Added`
- C
    - Added the `logger` module.
    - `Docs`
        - Added the `logger` module documentation.
- CPP
    - Added the `logger` module.
    - `Exception`
        - `UnreachableError` has been added.
        - `FileNotFoundError` has been added.
    - `Docs`
        - Added the `logger` module documentation.

`Changed`
- C
    - `version_init` has been renamed to `version_new` to more closely fit with this package's nomenclature.

## v0.1.0 - 2025-12-06
`Added`
- C
    - Added the `version` module.
    - `Docs`
        - Added documentation for the `version` module.
- CPP
    - Added the `exception` module.
    - Added the `version` module.
    - `Docs`
        - Added documentation for the `version` module.
        - Added documentation for the `exception` module.