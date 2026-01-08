# Changelog
## 0.9.0 - 2026-01-08
`Added`
- C
    - Added `buffer` module.
    - `Docs`
        - Added `buffer` documentation.
## 0.8.0 - 2026-01-06
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
## 0.7.0 - 2026-01-06
`Added`
- CPP
    - Added the `sized` interface.
    - `Docs`
        - Added the `sized` documentation.
## 0.6.0 - 2026-01-03
`Added`
- C
    - Added the `string builder` module.
    - `Docs`
        - Added `string builder` documentation.
## 0.5.0 - 2026-01-02
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
## 0.4.0 - 2025-12-23
`Added`
- C
    - Added the `string view` module.
    - `Docs`
        - Added the `string view` module documentation.

## 0.3.0 - 2025-12-14
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

## 0.2.0 - 2025-12-09
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

## 0.1.0 - 2025-12-06
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