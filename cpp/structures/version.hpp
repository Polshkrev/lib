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
    Version();

    /*
    * @brief Initialize a new version object with a given name.
    * @param name Name to give to the object.
    */
    Version(const std::string name);

    /*
    * @brief Initialize a new version with given major, minor, and patch version numbers.
    * @param major Major version number.
    * @param minor Minor version number.
    * @param patch Patch version number.
    */
    Version(std::size_t major, std::size_t minor, std::size_t patch);

    /*
    * @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
    * @param name Name with which to construct the version object.
    * @param major Major version number.
    * @param minor Minor version number.
    * @param patch Patch version number.
    */
    Version(const std::string name, std::size_t major, std::size_t minor, std::size_t patch);

    /*
    * @brief Update the name of the version object.
    * @param name Name with whitch to update the version object.
    */
    void update_name(const std::string name);

    /*
    * @brief Operator << overload.
    * @param stream Stream to push.
    */
    void operator<<(std::ostream &stream);

    /*
    * @brief Release the version object.
    */
    void release();

    /*
    * @brief Update the version object.
    */
    void update();

    /*
    * @brief Patch the version object.
    */
    void fix();
};

void operator<<(std::ostream &stream, Version &version);

#endif // VERSION_HPP_

#ifdef VERSION_IMPLEMENTATION

/*
* @brief Initialize a new version object.
*/
Version::Version()
{
    this->name = std::string();
    this->major = 0;
    this->minor = 0;
    this->patch = 0;
}

/*
* @brief Initialize a new version object with a given name.
* @param name Name to give to the object.
*/
Version::Version(const std::string name)
{
    this->name = name;
    this->major = 0;
    this->minor = 0;
    this->patch = 0;
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
    this->major++;
    this->minor = 0;
    this->patch = 0;
}

/*
* @brief Update the version object.
*/
void Version::update()
{
    this->minor++;
    this->patch = 0;
}

/*
* @brief Patch the version object.
*/
void Version::fix()
{
    this->patch++;
}

/*
* @brief Operator << overload.
* @param stream Stream to push.
*/
void Version::operator<<(std::ostream &stream)
{
    if (this->name.empty())
    {
        stream << this->major << "." << this->minor << "." << this->patch << "\n";
        return;
    }
    stream << this->name << ": " << this->major << "." << this->minor << "." << this->patch << "\n";
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