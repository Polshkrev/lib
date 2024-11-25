#ifndef VERSION_HPP_
#define VERSION_HPP_

#include <string_view>
#include <string>
#include <ostream>
#include <sstream>

/*
* @brief Representation of a semantic-versioning object.
*/
struct Version
{
    std::size_t major;
    std::size_t minor;
    std::size_t patch;
    std::string name;
    std::string description;
    /*
    * @brief Initialize a new version object.
    */
    explicit Version() noexcept;

    /*
    * @brief Initialize a new version object with a given name.
    * @param name Name to give to the object.
    */
    Version(const std::string_view name) noexcept;

    /*
    * @brief Initialize a new version object with a given name.
    * @param description Description, or changelog, of the version.
    * @param name Name to give to the object.
    */
    Version(const std::string_view name, const std::string_view description) noexcept;

    /*
    * @brief Initialize a new version with given major, minor, and patch version numbers.
    * @param major Major version number.
    * @param minor Minor version number.
    * @param patch Patch version number.
    */
    Version(const std::size_t major, const std::size_t minor, const std::size_t patch) noexcept;

    /*
    * @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
    * @param name Name with which to construct the version object.
    * @param major Major version number.
    * @param minor Minor version number.
    * @param patch Patch version number.
    */
    Version(const std::string_view name, const std::size_t major, const std::size_t minor, const std::size_t patch) noexcept;

    /*
    * @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
    * @param name Name with which to construct the version object.
    * @param description Description, or changelog, of the version.
    * @param major Major version number.
    * @param minor Minor version number.
    * @param patch Patch version number.
    */
    Version(const std::string_view name, const std::string_view description, const std::size_t major, const std::size_t minor, const std::size_t patch) noexcept;

    /*
    * @brief Update the name of the version object.
    * @param name Name with whitch to update the version object.
    */
    void update_name(const std::string_view name) noexcept;

    /*
    * @brief Update the description of the version.
    * @param description New description from which to update.
    */
    void update_description(const std::string_view description) noexcept;

    /*
    * @brief Operator << overload.
    * @param stream Stream to push.
    */
    void operator<<(std::ostream &stream) const noexcept;

    /*
    * @brief Release the version object.
    */
    void release() noexcept;

    /*
    * @brief Update the version object.
    */
    void update() noexcept;

    /*
    * @brief Patch the version object.
    */
    void fix() noexcept;
    /*
    * @brief Obtain a string representation of the version object.
    * @returns A string representing a symantic version.
    */
    const char *to_string() const noexcept;
};

void operator<<(std::ostream &stream, const Version &version) noexcept;

#endif // VERSION_HPP_

#ifdef VERSION_IMPLEMENTATION

/*
* @brief Initialize a new version object.
*/
Version::Version() noexcept : name(std::string()), description(std::string()), major(0), minor(0), patch(0) {}

/*
* @brief Initialize a new version object with a given name.
* @param description Description, or changelog, of the version.
* @param name Name to give to the object.
*/
Version::Version(const std::string_view name, const std::string_view description) noexcept : name(name.data()), description(description.data()), major(0), minor(0), patch(0) {}

/*
* @brief Initialize a new version object with a given name.
* @param name Name to give to the object.
*/
Version::Version(const std::string_view name) noexcept : name(name.data()), description(description.data()), major(0), minor(0), patch(0) {}

/*
* @brief Initialize a new version with given major, minor, and patch version numbers.
* @param major Major version number.
* @param minor Minor version number.
* @param patch Patch version number.
*/
Version::Version(std::size_t major, std::size_t minor, std::size_t patch) noexcept : name(std::string()), major(major), minor(minor), patch(patch) {}

/*
* @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
* @param name Name with which to construct the version object.
* @param major Major version number.
* @param minor Minor version number.
* @param patch Patch version number.
*/
Version::Version(const std::string_view name, const std::size_t major, const std::size_t minor, const std::size_t patch) noexcept : name(name.data()), major(major), minor(minor), patch(patch) {}

/*
* @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
* @param name Name with which to construct the version object.
* @param description Description, or changelog, of the version.
* @param major Major version number.
* @param minor Minor version number.
* @param patch Patch version number.
*/
Version::Version(const std::string_view name, const std::string_view description, const std::size_t major, const std::size_t minor, const std::size_t patch) noexcept : name(name.data()), description(description.data()), major(major), minor(minor), patch(patch) {}

/*
* @brief Update the name of the version object.
* @param name Name with whitch to update the version object.
*/
void Version::update_name(const std::string_view name) noexcept
{
    this->name = name;
}

void Version::update_description(const std::string_view description) noexcept
{
    this->description = description;
}

/*
* @brief Release the version object.
*/
void Version::release() noexcept
{
    major++;
    minor = 0;
    patch = 0;
}

/*
* @brief Update the version object.
*/
void Version::update() noexcept
{
    minor++;
    patch = 0;
}

/*
* @brief Patch the version object.
*/
void Version::fix() noexcept
{
    patch++;
}

/*
* @brief Obtain a string representation of the version object.
* @returns A string representing a symantic version.
*/
const char *Version::to_string() const noexcept
{
    std::stringstream result = std::stringstream();
    if (name.empty() && description.empty())
    {
        result << major;
        result << ".";
        result << minor;
        result << ".";
        result << patch;
        return result.str().c_str();
    }
    else if ((name.empty()) && (!(description.empty())))
    {
        result << major;
        result << ".";
        result << minor;
        result << ".";
        result << patch;
        result << " - ";
        result << description;
        return result.str().c_str();
    }
    else if ((!(name.empty())) && (description.empty()))
    {
        result << name;
        result << ": ";
        result << major;
        result << ".";
        result << minor;
        result << ".";
        result << patch;
        return result.str().c_str();
    }
    result << name;
    result << ": ";
    result << major;
    result << ".";
    result << minor;
    result << ".";
    result << patch;
    result << " - ";
    result << description;
    return result.str().c_str();
}

/*
* @brief Operator << overload.
* @param stream Stream to push.
*/
void Version::operator<<(std::ostream &stream) const noexcept
{
    std::stringstream full = std::stringstream();
    full << to_string();
    full << "\n";
    stream << full.str();
}

void operator<<(std::ostream &stream, const Version &version) noexcept
{
    std::stringstream full = std::stringstream();
    full << version.to_string();
    full << "\n";
    stream << full.str();
}

#endif // VERSION_IMPLEMENTATION