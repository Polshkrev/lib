# Changelog
## 0.4.0 - 2025-12-23
`Added`
- C
    - Added the string view module.
    - `Docs`
        - Added the string view module documentation.

## 0.3.0 - 2025-12-14
`Added`
- C
    - Added the flag module.
    - `Docs`
        - Added flag module documentation.

`Changed`
- CPP
    - `ValueError`, `FileNotFoundError`, and `UnreachableError` have been moved to a seperate file: `exceptions.hpp`.
    - `Version`
        - `to_string` now returns a `std::string`.

## 0.2.0 - 2025-12-09
`Added`
- C
    - Added the logger module.
    - `Docs`
        - Added the logger module documentation.
- CPP
    - Added the logger module.
    - `Exception`
        - `UnreachableError` has been added.
        - `FileNotFoundError` has been added.
    - `Docs`
        - Added the logger module documentation.

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