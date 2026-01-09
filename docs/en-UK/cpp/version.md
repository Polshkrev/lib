# Version
This is a semantic versioning library. This library can be used to store versioning objects for feature flags. Or it can used with the flag module to easily create a professional looking version flag.
## Table Of Contents
1. [Usage](#usage)
    1. [Construction](#construction)
    2. [Modify](#modify)
## Usage
### Construction
There are many ways to construct a version object as there are many constructor overloads. An `operator<<` overload is available for printing.
### Modify
To set any of the properties in the version object, each property has a specific setter accociated with it.
To change any of the properties within the object the following functions are available:
- Release: Increment the object's major value and set its minor and patch values to 0.
- Update: Increment the object's minor value and set the object's patch version to 0 while not affecting the object's major value.
- Patch: Increment the object's patch value while not affecting the object's major or minor values.
- Publish: Set the object's major value to 1. If the object is evaluated to have already been published, an error is raised and no values are changed.