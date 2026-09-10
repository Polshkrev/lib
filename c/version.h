#ifndef VERSION_H
#define VERSION_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h> // NULL
#include <stdint.h> // uint8_t, UINT8_MAX
#include <stdbool.h> // bool
#include <stdio.h> // FILE, stderr, fprintf

/**
 * @brief Representation of a semantic-versioning object.
 */
typedef struct
{
    const char *name;
    const char *description;
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} version_t;

/**
 * @brief Construct a new version object. Each of the version properties are set to zero and the name is set to NULL.
 * @returns A new version object.
 */
version_t version_init(void);

/**
 * @brief Construct a new version object from a given major, minor, and patch numbers.
 * @param major Major release number.
 * @param minor Minor release number.
 * @param patch Patch fix number.
 * @returns A new version object with each of the properties set to the given parameters.
 */
version_t version_convert(uint8_t major, uint8_t minor, uint8_t patch);

/**
 * @brief Construct a new version object with a given name.
 * @param name Constant string to pass to the version object.
 * @returns A new version with the given name and each of its release properties set to zero.
 */
version_t version_init_with_name(const char *name);

/**
 * @brief Construct a version object using only the string properties.
 * @param name Name to set for the version object.
 * @param description Description, or changlog, of a version object.
 * @returns A new version with its string realted properties set as the given arguments and its numeric properties set to zero.
 */
version_t version_init_strings(const char *name, const char *description);

/**
 * @brief Full initialize a version object.
 * @param name Name to set for the version object.
 * @param description Description, or changlog, of a version object.
 * @param major Major release number.
 * @param minor Minor release number.
 * @param patch Patch fix number.
 * @returns A new version initialized with all its properties set to the given parameters.
 */
version_t version_init_full(const char *name, const char *description, uint8_t major, uint8_t minor, uint8_t patch);

/**
 * @brief Set the major value of a version object.
 * @param version Object to update.
 * @param major Value from which to set.
 */
void version_set_major(version_t *version, uint8_t major);

/**
 * @brief Set the minor value of a version object.
 * @param version Object to update.
 * @param minor Value from which to set.
 */
void version_set_minor(version_t *version, uint8_t minor);

/**
 * @brief Set the patch value of a version object.
 * @param version Object to update.
 * @param patch Value from which to set.
 */
void version_set_patch(version_t *version, uint8_t patch);

/**
 * @brief Publish a new version. Sets the given version's major release number to one.
 * @param version Version to publish.
 * @exception If the given version object is evaluated to already be published, a `ValueError` is printed to `stderr` and the programme exits.
 */
void version_publish(version_t *version);

/**
 * @brief Release a given version. Increment the given version object's major release number.
 * @param version Version object to upgrade.
 * @exception If the version's major property is equal to the maximum `uint8_t`, a `ValueError` is printed to `stderr` and the programme exits.
 */
void version_release(version_t *version);

/**
 * @brief Update a given version. Increment the given version object's minor release number.
 * @param version Version object to update.
 * @exception If the version's minor property is equal to the maximum `uint8_t`, a `ValueError` is printed to `stderr` and the programme exits.
 */
void version_update(version_t *version);

/**
 * @brief Fix a given version. Increment the given version object's patch release number.
 * @param version Version object to patch.
 * @exception If the version's patch property is equal to the maximum `uint8_t`, a `ValueError` is printed to `stderr` and the programme exits.
 */
void version_fix(version_t *version);

/**
 * @brief Check is a given version is publicly available.
 * @param version Version object to check.
 * @returns False if the given version object's major release number is less than one, else true.
 */
bool version_is_public(const version_t *version);

/**
 * @brief Compare the major release of a given version object.
 * @param version Version object to compare.
 * @param major Major release number to compare.
 * @returns False if the given version object's major version is not equal to the given major parametre, else true.
 */
bool version_compare_major(const version_t *version, uint8_t major);

/**
 * @brief Compare the minor release of a given version object.
 * @param version Version object to compare.
 * @param minor Minor release number to compare.
 * @returns False if the given version object's minor version is not equal to the given minor parametre, else true.
 */
bool version_compare_minor(const version_t *version, uint8_t minor);

/**
 * @brief Compare the patch release of a given version object.
 * @param version Version object to compare.
 * @param patch Patch release number to compare.
 * @returns False if the given version object's patch version is not equal to the given patch parametre, else true.
 */
bool version_compare_patch(const version_t *version, uint8_t patch);

/**
 * @brief Compare one given version object to another.
 * @param version Version object to compare.
 * @param other Version object to compare.
 * @returns False if all of one given version object's properties are not equal to each other, else true.
 */
bool version_compare(const version_t *version, const version_t *other);

/**
 * @brief Print a given version in a given output stream. If the version's name is not given or NULL, then the name is not printed.
 * @param stream Output stream to print the given version.
 * @param version Version object to print.
 */
void version_print(FILE *stream, const version_t *version);

#if defined(__cplusplus)
}
#endif

#endif // VERSION_H

#ifdef VERSION_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdlib.h> // exit
#include <inttypes.h> // PRIu8

/**
 * @brief Construct a new version object. Each of the version properties are set to zero and the name is set to NULL.
 * @returns A new version object.
 */
version_t version_init(void)
{
    return version_init_full(NULL, NULL, 0, 0, 0);
}

/**
 * @brief Construct a new version object from a given major, minor, and patch numbers.
 * @param major Major release number.
 * @param minor Minor release number.
 * @param patch Patch fix number.
 * @returns A new version object with each of the properties set to the given parameters.
 */
version_t version_convert(uint8_t major, uint8_t minor, uint8_t patch)
{
    return version_init_full(NULL, NULL, major, minor, patch);
}

/**
 * @brief Initialize a new version object with a given name.
 * @param name Constant string to pass to the version object.
 * @returns A new version with the given name and each of its release properties set to zero.
 */
version_t version_init_with_name(const char *name)
{
    return version_init_full(name, NULL, 0, 0, 0);
}

/**
 * @brief Construct a version object using only the string properties.
 * @param name Name to set for the version object.
 * @param description Description, or changlog, of a version object.
 * @returns A new version with its string realted properties set as the given arguments and its numeric properties set to zero.
 */
version_t version_init_strings(const char *name, const char *description)
{
    return version_init_full(name, description, 0, 0, 0);
}

/**
 * @brief Full initialize a version object.
 * @param name Name to set for the version object.
 * @param description Description, or changlog, of a version object.
 * @param major Major release number.
 * @param minor Minor release number.
 * @param patch Patch fix number.
 * @returns A new version initialized with all its properties set to the given parameters.
 */
version_t version_init_full(const char *name, const char *description, uint8_t major, uint8_t minor, uint8_t patch)
{
    return (version_t)
    {
        .name = name,
        .description = description,
        .major = major,
        .minor = minor,
        .patch = patch
    };
}

/**
 * @brief Set the major value of a version object.
 * @param version Object to update.
 * @param major Value from which to set.
 */
void version_set_major(version_t *version, uint8_t major)
{
    version->major = major;
}

/**
 * @brief Set the minor value of a version object.
 * @param version Object to update.
 * @param minor Value from which to set.
 */
void version_set_minor(version_t *version, uint8_t minor)
{
    version->minor = minor;
}

/**
 * @brief Set the patch value of a version object.
 * @param version Object to update.
 * @param patch Value from which to set.
 */
void version_set_patch(version_t *version, uint8_t patch)
{
    version->patch = patch;
}

/**
 * @brief Generic print function for a given version.
 * @param stream Stream to which to print the given version.
 * @param version Version to print.
 * @param newline Flag to add a newline to the output.
 */
static void _version_print(FILE *stream, const version_t *version, bool newline)
{
    if (version->name != NULL)
    {
        fprintf(stream, "%s: ", version->name);
    }

    fprintf(stream, "%"PRIu8".%"PRIu8".%"PRIu8, version->major, version->minor, version->patch);

    if (version->description != NULL)
    {
        fprintf(stream, " - %s", version->description);
    }

    if (newline)
    {
        fprintf(stream, "\n");
    }
}

/**
 * @brief Publish a new version. Sets the given version's major release number to 1.
 * @param version Version to publish.
 * @exception If the given version object is evaluated to already be published, a `ValueError` is printed to `stderr` and the programme exits.
 */
void version_publish(version_t *version)
{
    if (version_is_public(version))
    {
        fprintf(stderr, "ValueError: Version - ");
        _version_print(stderr, version, false);
        fprintf(stderr, " is already public.");
        exit(1);
    }
    version_set_major(version, 1);
    version_set_minor(version, 0);
    version_set_patch(version, 0);
}

/**
 * @brief Release a given version. Increment the given version object's major release number.
 * @param version Version object to upgrade.
 * @exception If the version's major property is equal to the maximum `uint8_t`, a `ValueError` is printed to `stderr` and the programme exits.
 */
void version_release(version_t *version)
{
    if (version->major == UINT8_MAX)
    {
        fprintf(stderr, "ValueError: The major version has reached the maximum allowed by type.\n");
        exit(1);
    }
    version->major++;
    version_set_minor(version, 0);
    version_set_patch(version, 0);
}

/**
 * @brief Update a given version. Increment the given version object's minor release number.
 * @param version Version object to update.
 * @exception If the version's minor property is equal to the maximum `uint8_t`, a `ValueError` is printed to `stderr` and the programme exits.
 */
void version_update(version_t *version)
{
    if (version->minor == UINT8_MAX)
    {
        fprintf(stderr, "ValueError: The minor version has reached the maximum allowed by type.\n");
        exit(1);
    }
    version->minor++;
    version_set_patch(version, 0);
}

/**
 * @brief Fix a given version. Increment the given version object's patch release number.
 * @param version Version object to patch.
 * @exception If the version's patch property is equal to the maximum `uint8_t`, a `ValueError` is printed to `stderr` and the programme exits.
 */
void version_fix(version_t *version)
{
    if (version->patch == UINT8_MAX)
    {
        fprintf(stderr, "ValueError: The patch version has reached the maximum allowed by type.\n");
        exit(1);
    }
    version->patch++;
}

/**
 * @brief Check is a given version is publicly available.
 * @param version Version object to check.
 * @returns False if the given version object's major release number is less than one, else true.
 */
bool version_is_public(const version_t *version)
{
    return version_compare_major(version, 1);
}

/**
 * @brief Compare the major release of a given version object.
 * @param version Version object to compare.
 * @param major Major release number to compare.
 * @returns False if the given version object's major version is not greater than or equal to the given major parametre, else true.
 */
bool version_compare_major(const version_t *version, uint8_t major)
{
    return version->major >= major;
}

/**
 * @brief Compare the minor release of a given version object.
 * @param version Version object to compare.
 * @param minor Minor release number to compare.
 * @returns False if the given version object's minor version is not greater than or equal to the given minor parametre, else true.
 */
bool version_compare_minor(const version_t *version, uint8_t minor)
{
    return version->minor >= minor;
}

/**
 * @brief Compare the patch release of a given version object.
 * @param version Version object to compare.
 * @param patch Patch release number to compare.
 * @returns False if the given version object's patch version is not greater than or equal to the given patch parametre, else true.
 */
bool version_compare_patch(const version_t *version, uint8_t patch)
{
    return version->patch >= patch;
}

/**
 * @brief Compare one given version object to another.
 * @param version Version object to compare.
 * @param other Version object to compare.
 * @returns False if all of one given version object's properties are not equal to each other, else true.
 */
bool version_compare(const version_t *version, const version_t *other)
{
    return (version_compare_major(version, other->major)) && (version_compare_minor(version, other->minor)) && (version_compare_patch(version, other->patch));
}

/**
 * @brief Print a given version in a given output stream. If the version's name is not given or NULL, then the name is not printed.
 * @param stream Output stream to print the given version.
 * @param version Version object to print.
 */
void version_print(FILE *stream, const version_t *version)
{
    _version_print(stream, version, true);
}

#if defined(__cplusplus)
}
#endif

#endif // VERSION_IMPLEMENTATION