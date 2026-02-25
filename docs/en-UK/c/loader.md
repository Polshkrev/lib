# Loader
A platform agnostic dynamically linked library loader.

## Table of Contents
1. [Usage](#usage)
    1. [Load](#load)
    2. [Access](#access)
    3. [Destruction](#destruction)
## Usage
### Load
To load a library, the `library_load` function should be used. This function will return a handle to the dynamically linked library.
### Access
To acccess a function within the dynamically linked library, the function `library_function` will need to be called.
This function takes in the handle to the library obtained through the `library_load` function and the name of the function to be loaded. This function will return a handle to the loaded function.
### Destruction
To deallocate the library, the function `library_delete` will need to be called. This wil return a boolean success sentinel value.