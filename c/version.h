#ifndef VERSION_H
#define VERSION_H

#if defined(__cplusplus)
extern "C"{
#endif

#include <stddef.h> // size_t
#include <stdbool.h> // bool
#include <stdio.h> // FILE, fprintf, stderr

/*
* @brief Representation of a semantic-versioning object.
*/
typedef struct
{
    const char *name;
    const char *description;
    size_t major;
    size_t minor;
    size_t patch;
} version_t;

/*
* @brief Initialize a new version object. Each of the version properties are set to zero and the name is set to NULL.
* @returns A new version object.
* @throws AllocationError If the version object can not be heap allocated, an `AllocationError` is printed to `stderr` and the programme exits.
*/
version_t *version_init(void);

/*
* @brief Alternatively initialize a new version object from a given major, minor, and patch numbers.
* @param major Major release number.
* @param minor Minor release number.
* @param patch Patch fix number.
* @returns A new version object with each of the properties set to the given parametres.
* @throws AllocationError If the version object can not be heap allocated, an `AllocationError` is printed to `stderr` and the programme exits.
*/
version_t *version_convert(size_t major, size_t minor, size_t patch);

/*
* @brief Initialize a new version object with a given name.
* @param name Constant string to pass to the version object.
* @returns A new version with the given name and each of its release properties set to zero.
* @throws AllocationError If the version object can not be heap allocated, an `AllocationError` is printed to `stderr` and the programme exits.
*/
version_t *version_init_with_name(const char *name);

/*
* @brief Initialize a version object using only the string properties.
* @param name Name to set for the version object.
* @param description Description, or changlog, of a version object.
* @returns A new version with its string realted properties set as the given arguments and its numeric properties set to 0.
* @throws AllocationError If the version object can not be heap allocated, an `AllocationError` is printed to `stderr` and the programme exits.
*/
version_t *version_init_strings(const char *name, const char *description);

/*
* @brief Full initialize a version object.
* @param name Name to set for the version object.
* @param description Description, or changlog, of a version object.
* @param major Major release number.
* @param minor Minor release number.
* @param patch Patch fix number.
* @returns A new version initialized with all its properties set to the given parametres.
* @throws AllocationError If the version object can not be heap allocated, an `AllocationError` is printed to `stderr` and the programme exits.
*/
version_t *version_init_full(const char *name, const char *description, size_t major, size_t minor, size_t patch);

/*
* @brief Set the major value of a version object.
* @param version Object to update.
* @param major Value from which to set.
*/
void version_set_major(version_t *version, size_t major);

/*
* @brief Set the minor value of a version object.
* @param version Object to update.
* @param minor Value from which to set.
*/
void version_set_minor(version_t *version, size_t minor);

/*
* @brief Set the patch value of a version object.
* @param version Object to update.
* @param patch Value from which to set.
*/
void version_set_patch(version_t *version, size_t patch);

/*
* @brief Publish a new version. Sets the given version's major release number to 1.
* @param version Version to publish.
* @throws ValueError If the given version object is evaluated to already be published, a `ValueError` is printed to `stderr` and the programme exits.
*/
void version_publish(version_t *version);

/*
* @brief Release a given version. Increment the given version object's major release number.
* @param version Version object to upgrade.
*/
void version_release(version_t *version);

/*
* @brief Update a given version. Increment the given version object's minor release number.
* @param version Version object to update.
*/
void version_update(version_t *version);

/*
* @brief Fix a given version. Increment the given version object's patch release number.
* @param version Version object to patch.
*/
void version_fix(version_t *version);

/*
* @brief Check is a given version is publicly available.
* @param version Version object to check.
* @returns False if the given version object's major release number is less than 1, else true.
*/
bool version_is_public(const version_t *version);

/*
* @brief Compare the major release of a given version object.
* @param version Version object to compare.
* @param major Major release number to compare.
* @returns False if the given version object's major version is not equal to the given major parametre, else true.
*/
bool version_compare_major(const version_t *version, size_t major);

/*
* @brief Compare the minor release of a given version object.
* @param version Version object to compare.
* @param minor Minor release number to compare.
* @returns False if the given version object's minor version is not equal to the given minor parametre, else true.
*/
bool version_compare_minor(const version_t *version, size_t minor);

/*
* @brief Compare the patch release of a given version object.
* @param version Version object to compare.
* @param patch Patch release number to compare.
* @returns False if the given version object's patch version is not equal to the given patch parametre, else true.
*/
bool version_compare_patch(const version_t *version, size_t patch);

/*
* @brief Compare one given version object to another.
* @param version Version object to compare.
* @param other Version object to compare.
* @returns False if all of one given version object's properties are not equal to each other, else true.
*/
bool version_comapre(const version_t *version, const version_t *other);

/*
* @brief Print a given version in a given output stream. If the version's name is not given or NULL, then the name is not printed.
* @param stream Output stream to print the given version.
* @param version Version object to print.
*/
void version_print(FILE *stream, const version_t *version);

/*
* @brief Deallocate a version object.
* @param version Object to deallocate.
*/
void version_delete(version_t *version);

#ifdef defined(__cplusplus)
}
#endif

#endif // VERSION_H

#ifdef VERSION_IMPLEMENTATION

#if defined(__cplusplus)
extern "C"{
#endif

#include <stdlib.h> // malloc, NULL, exit
#include <string.h> // memset

/*
* @brief Initialize a new version object. Each of the version properties are set to zero and the name is set to NULL.
* @returns A new version object.
* @throws AllocationError If the version object can not be heap allocated, an `AllocationError` is printed to `stderr` and the programme exits.
*/
version_t *version_init(void)
{
    version_t *version = (version_t *)malloc(sizeof(version));
    if (NULL == version)
    {
        fprintf(stderr, "AllocationError: Can not allocate enough memory to initialize the version object.");
        exit(1);
    }
    memset(version, 0, sizeof(*version));
    return version;
}

/*
* @brief Alternatively initialize a new version object from a given major, minor, and patch numbers.
* @param major Major release number.
* @param minor Minor release number.
* @param patch Patch fix number.
* @returns A new version object with each of the properties set to the given parametres.
* @throws AllocationError If the version object can not be heap allocated, an `AllocationError` is printed to `stderr` and the programme exits.
*/
version_t *version_convert(size_t major, size_t minor, size_t patch)
{
    version_t *version = version_init();
    version_set_major(version, major);
    version_set_major(version, minor);
    version_set_major(version, patch);
    return version;
}

/*
* @brief Initialize a new version object with a given name.
* @param name Constant string to pass to the version object.
* @returns A new version with the given name and each of its release properties set to zero.
* @throws AllocationError If the version object can not be heap allocated, an `AllocationError` is printed to `stderr` and the programme exits.
*/
version_t *version_init_with_name(const char *name)
{
    version_t *version = version_init();
    version->name = name;
    return version;
}

/*
* @brief Initialize a version object using only the string properties.
* @param name Name to set for the version object.
* @param description Description, or changlog, of a version object.
* @returns A new version with its string realted properties set as the given arguments and its numeric properties set to 0.
* @throws AllocationError If the version object can not be heap allocated, an `AllocationError` is printed to `stderr` and the programme exits.
*/
version_t *version_init_strings(const char *name, const char *description)
{
    version_t *version = version_init_with_name(name);
    version->description = description;
    return version;
}

/*
* @brief Full initialize a version object.
* @param name Name to set for the version object.
* @param description Description, or changlog, of a version object.
* @param major Major release number.
* @param minor Minor release number.
* @param patch Patch fix number.
* @returns A new version initialized with all its properties set to the given parametres.
* @throws AllocationError If the version object can not be heap allocated, an `AllocationError` is printed to `stderr` and the programme exits.
*/
version_t *version_init_full(const char *name, const char *description, size_t major, size_t minor, size_t patch)
{
    version_t *version = version_convert(major, minor, patch);
    version->name = name;
    version->description = description;
    return version;
}

/*
* @brief Set the major value of a version object.
* @param version Object to update.
* @param major Value from which to set.
*/
void version_set_major(version_t *version, size_t major)
{
    version->major = major;
}

/*
* @brief Set the minor value of a version object.
* @param version Object to update.
* @param minor Value from which to set.
*/
void version_set_minor(version_t *version, size_t minor)
{
    version->minor = minor;
}

/*
* @brief Set the patch value of a version object.
* @param version Object to update.
* @param patch Value from which to set.
*/
void version_set_patch(version_t *version, size_t patch)
{
    version->patch = patch;
}

/*
* @brief Internal way of printing a given version object for error reporting.
* @param version Version object to print.
*/
void _version_error_print(FILE *stream, const version_t *version)
{
    if (NULL == version->name)
    {
        fprintf(stream, "%d.%d.%d", (int)version->major, (int)version->minor, (int)version->patch);
        return;
    }
    fprintf(stream, "%s: %d.%d.%d", version->name, (int)version->major, (int)version->minor, (int)version->patch);
}

/*
* @brief Publish a new version. Sets the given version's major release number to 1.
* @param version Version to publish.
* @throws ValueError If the given version object is evaluated to already be published, a `ValueError` is printed to `stderr` and the programme exits.
*/
void version_publish(version_t *version)
{
    if (version_is_public(version))
    {
        fprintf(stderr, "ValueError: Version - ");
        _version_error_print(stderr, version);
        fprintf(stderr, " is already public.");
        exit(1);
    }
    version_set_major(version, 1);
    version_set_minor(version, 0);
    version_set_patch(version, 0);
}

/*
* @brief Release a given version. Increment the given version object's major release number.
* @param version Version object to upgrade.
*/
void version_release(version_t *version)
{
    version->major++;
    version_set_minor(version, 0);
    version_set_patch(version, 0);
}

/*
* @brief Update a given version. Increment the given version object's minor release number.
* @param version Version object to update.
*/
void version_update(version_t *version)
{
    version->minor++;
    version_set_patch(version, 0);
}

/*
* @brief Fix a given version. Increment the given version object's patch release number.
* @param version Version object to patch.
*/
void version_fix(version_t *version)
{
    version->patch++;
}

/*
* @brief Check is a given version is publicly available.
* @param version Version object to check.
* @returns False if the given version object's major release number is less than 1, else true.
*/
bool version_is_public(const version_t *version)
{
    return version_compare_major(version, 1);
}

/*
* @brief Compare the major release of a given version object.
* @param version Version object to compare.
* @param major Major release number to compare.
* @returns False if the given version object's major version is not greater than or equal to the given major parametre, else true.
*/
bool version_compare_major(const version_t *version, size_t major)
{
    return version->major >= major;
}

/*
* @brief Compare the minor release of a given version object.
* @param version Version object to compare.
* @param minor Minor release number to compare.
* @returns False if the given version object's minor version is not greater than or equal to the given minor parametre, else true.
*/
bool version_compare_minor(const version_t *version, size_t minor)
{
    return version->minor >= minor;
}

/*
* @brief Compare the patch release of a given version object.
* @param version Version object to compare.
* @param patch Patch release number to compare.
* @returns False if the given version object's patch version is not greater than or equal to the given patch parametre, else true.
*/
bool version_compare_patch(const version_t *version, size_t patch)
{
    return version->patch >= patch;
}

/*
* @brief Compare one given version object to another.
* @param version Version object to compare.
* @param other Version object to compare.
* @returns False if all of one given version object's properties are not equal to each other, else true.
*/
bool version_comapre(const version_t *version, const version_t *other)
{
    return (version_compare_major(version, other->major)) && (version_compare_minor(version, other->minor)) && (version_compare_patch(version, other->patch));
}

/*
* @brief Print a given version in a given output stream. If the version's name is not given or NULL, then the name is not printed.
* @param stream Output stream to print the given version.
* @param version Version object to print.
*/
void version_print(FILE *stream, const version_t *version)
{
    if ((NULL == version->name) && (NULL == version->description))
    {
        fprintf(stream, "%d.%d.%d\n", (int)version->major, (int)version->minor, (int)version->patch);
        return;
    }
    else if ((NULL == version->name) && (!(NULL == version->description)))
    {
        fprintf(stream, "%d.%d.%d - %s\n", (int)version->major, (int)version->minor, (int)version->patch, version->description);
        return;
    }
    else if ((!(NULL == version->name)) && (NULL == version->description))
    {
        fprintf(stream, "%s: %d.%d.%d\n", version->name, (int)version->major, (int)version->minor, (int)version->patch);
        return;

    }
    fprintf(stream, "%s: %d.%d.%d - %s\n", version->name, (int)version->major, (int)version->minor, (int)version->patch, version->description);
}

/*
* @brief Deallocate a version object.
* @param version Object to deallocate.
*/
void version_delete(version_t *version)
{
    if (!version)
    {
        return;
    }
    free(version);
}

#ifdef defined(__cplusplus)
}
#endif

#endif // VERSION_IMPLEMENTATION