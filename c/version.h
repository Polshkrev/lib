#ifndef VERSION_H_
#define VERSION_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
*/
version_t version_init();

/*
* @brief Alternatively initialize a new version object from a given major, minor, and patch numbers.
* @param major Major release number.
* @param minor Minor release number.
* @param patch Patch fix number.
* @returns A new version object with each of the properties set to the given parametres.
*/
version_t version_convert(const size_t major, const size_t minor, const size_t patch);

/*
* @brief Initialize a new version object with a given name.
* @param name Constant string to pass to the version object.
* @returns A new version with the given name and each of its release properties set to zero.
*/
version_t version_init_with_name(const char *name);

/*
* @brief Initialize a version object using only the string properties.
* @param name Name to set for the version object.
* @param description Description, or changlog, of a version object.
* @returns A new version with its string realted properties set as the given arguments
* and its numeric properties set to 0.
*/
version_t version_strings_init(const char *name, const char *description);

/*
* @brief Publish a new version. Sets the given version's major release number to 1. If the version object's major release number is already greater than 1, an error is raised and the programme exits.
* @param version Version to publish.
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
bool version_is_public(const version_t version);

/*
* @brief Compare the major release of a given version object.
* @param version Version object to compare.
* @param major Major release number to compare.
* @returns False if the given version object's major version is not equal to the given major parametre, else true.
*/
bool version_compare_major(const version_t version, const size_t major);

/*
* @brief Compare the minor release of a given version object.
* @param version Version object to compare.
* @param minor Minor release number to compare.
* @returns False if the given version object's minor version is not equal to the given minor parametre, else true.
*/
bool version_compare_minor(const version_t version, const size_t minor);

/*
* @brief Compare the patch release of a given version object.
* @param version Version object to compare.
* @param patch Patch release number to compare.
* @returns False if the given version object's patch version is not equal to the given patch parametre, else true.
*/
bool version_compare_patch(const version_t version, const size_t patch);

/*
* @brief Compare one given version object to another.
* @param version Version object to compare.
* @param other Version object to compare.
* @returns False if all of one given version object's properties are not equal to each other, else true.
*/
bool version_comapre(const version_t version, const version_t other);

/*
* @brief Print a given version in a given output stream. If the version's name is not given or NULL, then the name is not printed.
* @param stream Output stream to print the given version.
* @param version Version object to print. 
*/
void version_print(FILE *stream, const version_t version);

#endif // VERSION_H_

#ifdef VERSION_IMPLEMENTATION

/*
* @brief Initialize a new version object. Each of the version properties are set to zero and the name is set to NULL.
* @returns A new version object.
*/
version_t version_init()
{
    version_t version = {0};
    return version;
}

/*
* @brief Initialize a new version object with a given name.
* @param name Constant string to pass to the version object.
* @returns A new version with the given name and each of its release properties set to zero.
*/
version_t version_init_with_name(const char *name)
{
    version_t version = {0};
    version.name = name;
    return version;
}

/*
* @brief Initialize a version object using only the string properties.
* @param name Name to set for the version object.
* @param description Description, or changlog, of a version object.
* @returns A new version with its string realted properties set as the given arguments
* and its numeric properties set to 0.
*/
version_t version_strings_init(const char *name, const char *description)
{
    version_t version = {
        .name = name,
        .description = description
    };
    return version;
}

/*
* @brief Alternatively initialize a new version object from a given major, minor, and patch numbers.
* @param major Major release number.
* @param minor Minor release number.
* @param patch Patch fix number.
* @returns A new version object with each of the properties set to the given parametres.
*/
version_t version_convert(const size_t major, const size_t minor, const size_t patch)
{
    version_t version = version_init();
    version.major = major;
    version.minor = minor;
    version.patch = patch;
    return version;
}

/*
* @brief Internal way of printing a given version object for error reporting.
* @param version Version object to print.
*/
void _version_error_print(FILE *stream, version_t version)
{
    if (NULL == version.name)
    {
        fprintf(stream, "%d.%d.%d", (int)version.major, (int)version.minor, (int)version.patch);
        return;
    }
    fprintf(stream, "%s: %d.%d.%d", version.name, (int)version.major, (int)version.minor, (int)version.patch);
}

/*
* @brief Publish a new version. Sets the given version's major release number to 1. If the version object's major release number is already greater than 1, an error is raised and the programme exits.
* @param version Version to publish.
*/
void version_publish(version_t *version)
{
    if (version_is_public(*version))
    {
        fprintf(stderr, "VersionError: Version - ");
        _version_error_print(stderr, *version);
        fprintf(stderr, " is already public.");
        exit(1);
    }
    version_release(version);
}

/*
* @brief Release a given version. Increment the given version object's major release number.
* @param version Version object to upgrade.
*/
void version_release(version_t *version)
{
    version->major++;
    version->minor = 0;
    version->patch = 0;
}

/*
* @brief Update a given version. Increment the given version object's minor release number.
* @param version Version object to update.
*/
void version_update(version_t *version)
{
    version->minor++;
    version->patch = 0;
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
* @brief Compare the major release of a given version object.
* @param version Version object to compare.
* @param major Major release number to compare.
* @returns False if the given version object's major version is not equal to the given major parametre, else true.
*/
bool version_compare_major(const version_t version, const size_t major)
{
    return version.major == major;
}

/*
* @brief Compare the minor release of a given version object.
* @param version Version object to compare.
* @param minor Minor release number to compare.
* @returns False if the given version object's minor version is not equal to the given minor parametre, else true.
*/
bool version_compare_minor(const version_t version, const size_t minor)
{
    return version.minor == minor;
}

/*
* @brief Compare the patch release of a given version object.
* @param version Version object to compare.
* @param patch Patch release number to compare.
* @returns False if the given version object's patch version is not equal to the given patch parametre, else true.
*/
bool version_compare_patch(const version_t version, const size_t patch)
{
    return version.patch == patch;
}

/*
* @brief Check is a given version is publicly available.
* @param version Version object to check.
* @returns False if the given version object's major release number is less than 1, else true.
*/
bool version_is_public(const version_t version)
{
    if (version.major < 1)
    {
        return false;
    }
    return true;
}

/*
* @brief Compare one given version object to another.
* @param version Version object to compare.
* @param other Version object to compare.
* @returns False if all of one given version object's properties are not equal to each other, else true.
*/
bool version_comapre(const version_t version, const version_t other)
{
    return (version_compare_major(version, other.major)) && (version_compare_minor(version, other.minor)) && (version_compare_patch(version, other.patch));
}

/*
* @brief Print a given version in a given output stream. If the version's name is not given or NULL, then the name is not printed.
* @param stream Output stream to print the given version.
* @param version Version object to print. 
*/
void version_print(FILE *stream, const version_t version)
{
    if ((NULL == version.name) && (NULL == version.description))
    {
        fprintf(stream, "%d.%d.%d\n", (int)version.major, (int)version.minor, (int)version.patch);
        return;
    }
    else if ((NULL == version.name) && (!(NULL == version.description)))
    {
        fprintf(stream, "%d.%d.%d - %s\n", (int)version.major, (int)version.minor, (int)version.patch, version.description);
    }
    else if ((!(NULL == version.name)) && (NULL == version.description))
    {
        fprintf(stream, "%s: %d.%d.%d\n", version.name, (int)version.major, (int)version.minor, (int)version.patch);

    }
    fprintf(stream, "%s: %d.%d.%d - %s\n", version.name, (int)version.major, (int)version.minor, (int)version.patch, version.description);
}

#endif // VERSION_IMPLEMENTATION