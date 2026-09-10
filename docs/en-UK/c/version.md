# Version
This is a semantic versioning library. This library can be used to store versioning objects for feature flags. Or it can used with the flag module to easily create a professional looking version flag.
## Table Of Contents
1. [Usage](#usage)
    1. [Construction](#construction)
    2. [Modify](#modify)
    3. [Compare](#compare)
    4. [Destruction](#destruction)
## Usage
### Construction
There are many ways to construct a version object. The default is to use the `version_init` function. All of the constructor functions will return a heap-allocated pointer to a version object. the `version_init` function sets the string properties of the new version object to `NULL` and each of the number properties to 0. The naming convention of each of the constructor functions is as follows:
- A function with the "convert" keyword will construct the object with the number properties set to the given parametres.
- A function with the "strings" keyword is self-explanatory as it constructs each of the string properties in the version object.
- Any additional properties that are set in the constructor are named explicitly.
### Modify
To set any of the properties in the version object, each property has a specific setter accociated with it.
To change any of the properties within the object the following functions are available:
- Release: Increment the object's major value and set its minor and patch values to 0.
- Update: Increment the object's minor value and set the object's patch version to 0 while not affecting the object's major value.
- Patch: Increment the object's patch value while not affecting the object's major or minor values.
- Publish: Set the object's major value to 1. If the object is evaluated to have already been published, an error is raised and no values are changed.
### Compare
Each of the object's number properties have a comparison function. To compare a version object to another a `version_compare` function is available. An `is_public` function is available and is used in `version_publish`.
### Destruction
To deallocate any of the constructed version objects, you need to call `version_delete`.