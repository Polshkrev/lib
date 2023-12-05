#ifndef VERSION_HPP_
#define VERSION_HPP_

#include <string>
#include <ostream>
#include <string_view>

/*
* @brief Representation of a semantic-versioning object.
*/
struct Version
{
    std::string name;
    std::size_t major;
    std::size_t minor;
    std::size_t patch;
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
    * @brief Update the name of the version object.
    * @param name Name with whitch to update the version object.
    */
    void update_name(const std::string_view name) noexcept;

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
};

void operator<<(std::ostream &stream, const Version &version) noexcept;

#endif // VERSION_HPP_

#ifdef VERSION_IMPLEMENTATION

/*
* @brief Initialize a new version object.
*/
Version::Version() noexcept : name(std::string_view()), major(0), minor(0), patch(0) {}

/*
* @brief Initialize a new version object with a given name.
* @param name Name to give to the object.
*/
Version::Version(const std::string_view name) noexcept : name(name), major(0), minor(0), patch(0) {}

/*
* @brief Initialize a new version with given major, minor, and patch version numbers.
* @param major Major version number.
* @param minor Minor version number.
* @param patch Patch version number.
*/
Version::Version(std::size_t major, std::size_t minor, std::size_t patch) noexcept : name(std::string_view()), major(major), minor(minor), patch(patch) {}

/*
* @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
* @param name Name with which to construct the version object.
* @param major Major version number.
* @param minor Minor version number.
* @param patch Patch version number.
*/
Version::Version(const std::string_view name, const std::size_t major, const std::size_t minor, const std::size_t patch) noexcept : name(name), major(major), minor(minor), patch(patch) {}

/*
* @brief Update the name of the version object.
* @param name Name with whitch to update the version object.
*/
void Version::update_name(const std::string_view name) noexcept
{
    this->name = name;
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
* @brief Operator << overload.
* @param stream Stream to push.
*/
void Version::operator<<(std::ostream &stream) const noexcept
{
    if (name.empty())
    {
        stream << major << "." << minor << "." << patch << "\n";
        return;
    }
    stream << name << ": " << major << "." << minor << "." << patch << "\n";
}

void operator<<(std::ostream &stream, const Version &version) noexcept
{
    if (version.name.empty())
    {
        stream << version.major << "." << version.minor << "." << version.patch << "\n";
        return;
    }
    stream << version.name << ": " << version.major << "." << version.minor << "." << version.patch << "\n";
}

#endif // VERSION_IMPLEMENTATION