#ifndef VERSION_HPP_
#define VERSION_HPP_

#include <string>

/*
* @brief Representation of a semantic-versioning object.
*/
struct Version
{
    std::size_t major;
    std::size_t minor;
    std::size_t patch;
    std::string name;
    /*
    * @brief Initialize a new version object.
    */
    explicit Version() noexcept;

    /*
    * @brief Initialize a new version object with a given name.
    * @param name Name to give to the object.
    */
    Version(const std::string name) noexcept;

    /*
    * @brief Initialize a new version with given major, minor, and patch version numbers.
    * @param major Major version number.
    * @param minor Minor version number.
    * @param patch Patch version number.
    */
    Version(std::size_t major, std::size_t minor, std::size_t patch) noexcept;

    /*
    * @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
    * @param name Name with which to construct the version object.
    * @param major Major version number.
    * @param minor Minor version number.
    * @param patch Patch version number.
    */
    Version(const std::string name, std::size_t major, std::size_t minor, std::size_t patch) noexcept;

    /*
    * @brief Update the name of the version object.
    * @param name Name with whitch to update the version object.
    */
    void update_name(const std::string name) noexcept;

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

void operator<<(std::ostream &stream, Version &version);

#endif // VERSION_HPP_

#ifdef VERSION_IMPLEMENTATION

/*
* @brief Initialize a new version object.
*/
Version::Version()
{
    name = std::string();
    major = 0;
    minor = 0;
    patch = 0;
}

/*
* @brief Initialize a new version object with a given name.
* @param name Name to give to the object.
*/
Version::Version(const std::string name)
{
    this->name = name;
    major = 0;
    minor = 0;
    patch = 0;
}

/*
* @brief Initialize a new version with given major, minor, and patch version numbers.
* @param major Major version number.
* @param minor Minor version number.
* @param patch Patch version number.
*/
Version::Version(std::size_t major, std::size_t minor, std::size_t patch)
{
    this->major = major;
    this->minor = minor;
    this->patch = patch;
}

/*
* @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
* @param name Name with which to construct the version object.
* @param major Major version number.
* @param minor Minor version number.
* @param patch Patch version number.
*/
Version::Version(const std::string name, std::size_t major, std::size_t minor, std::size_t patch)
{
    this->name = name;
    this->major = major;
    this->minor = minor;
    this->patch = patch;
}

/*
* @brief Update the name of the version object.
* @param name Name with whitch to update the version object.
*/
void Version::update_name(const std::string name)
{
    this->name = name;
}

/*
* @brief Release the version object.
*/
void Version::release()
{
    major++;
    minor = 0;
    patch = 0;
}

/*
* @brief Update the version object.
*/
void Version::update()
{
    minor++;
    patch = 0;
}

/*
* @brief Patch the version object.
*/
void Version::fix()
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

void operator<<(std::ostream &stream, Version &version)
{
    if (version.name.empty())
    {
        stream << version.major << "." << version.minor << "." << version.patch << "\n";
        return;
    }
    stream << version.name << ": " << version.major << "." << version.minor << "." << version.patch << "\n";
}

#endif // VERSION_IMPLEMENTATION