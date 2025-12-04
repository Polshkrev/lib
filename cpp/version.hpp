#ifndef VERSION_HPP
#define VERSION_HPP

#include <cstddef> // size_t
#include <string> // string
#include <ostream> // ostream

#include "exception.hpp"

namespace polutils
{
    /*
    * @brief Representation of a semantic-versioning object.
    */
    struct version_t
    {
        std::size_t major;
        std::size_t minor;
        std::size_t patch;
        std::string name;
        std::string description;
        /*
        * @brief Initialize a new version object.
        */
        explicit version_t() noexcept;

        /*
        * @brief Initialize a new version object with a given name.
        * @param name Name to give to the object.
        */
        version_t(const std::string &name) noexcept;

        /*
        * @brief Initialize a new version object with a given name.
        * @param description Description, or changelog, of the version.
        * @param name Name to give to the object.
        */
        version_t(const std::string &name, const std::string &description) noexcept;

        /*
        * @brief Initialize a new version with given major, minor, and patch version numbers.
        * @param major Major version number.
        * @param minor Minor version number.
        * @param patch Patch version number.
        */
        version_t(std::size_t major, std::size_t minor, std::size_t patch) noexcept;

        /*
        * @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
        * @param name Name with which to construct the version object.
        * @param major Major version number.
        * @param minor Minor version number.
        * @param patch Patch version number.
        */
        version_t(const std::string &name, std::size_t major, std::size_t minor, std::size_t patch) noexcept;

        /*
        * @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
        * @param name Name with which to construct the version object.
        * @param description Description, or changelog, of the version.
        * @param major Major version number.
        * @param minor Minor version number.
        * @param patch Patch version number.
        */
        version_t(const std::string &name, const std::string &description, std::size_t major, std::size_t minor, std::size_t patch) noexcept;

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
        * @brief Publish a new version. Sets the given version's major release number to 1.
        * @throws ValueError If the given version object is evaluated to already be published, a `ValueError` is printed to `stderr` and the programme exits.
        */
        void publish();

        /*
        * @brief Compare the major release of a given version object.
        * @param major Major release number to compare.
        * @returns False if the given version object's major version is not equal to the given major parametre, else true.
        */
        bool compare_major(std::size_t major) const noexcept;

        /*
        * @brief Compare the minor release of a given version object.
        * @param minor Minor release number to compare.
        * @returns False if the given version object's minor version is not equal to the given minor parametre, else true.
        */
        bool compare_minor(std::size_t minor) const noexcept;

        /*
        * @brief Compare the patch release of a given version object.
        * @param patch Patch release number to compare.
        * @returns False if the given version object's patch version is not equal to the given patch parametre, else true.
        */
        bool compare_patch(std::size_t patch) const noexcept;

        /*
        * @brief Compare one given version object to another.
        * @param version Version object to compare.
        * @returns False if all of one given version object's properties are not equal to each other, else true.
        */
        bool compare(const version_t &version) const noexcept;

        /*
        * @brief Check is a given version is publicly available.
        * @returns False if the given version object's major release number is less than 1, else true.
        */
        bool is_public() const noexcept;

        /*
        * @brief Obtain a string representation of the version object.
        * @returns A string representing a symantic version.
        */
        const char *to_string() const noexcept;
    };

    struct ValueError : public Exception
    {
        explicit ValueError(const std::string &message);
    };

    void operator<<(std::ostream &stream, const version_t &version) noexcept;
}

#endif // VERSION_HPP

#ifdef VERSION_IMPLEMENTATION

#include <sstream> // stringstream

#define EXCEPTION_IMPLEMENTATION
#include "exception.hpp"

namespace polutils
{
    /*
    * @brief Initialize a new version object.
    */
    version_t::version_t() noexcept : name(std::string()), description(std::string()), major(0), minor(0), patch(0) {}

    /*
    * @brief Initialize a new version object with a given name.
    * @param description Description, or changelog, of the version.
    * @param name Name to give to the object.
    */
    version_t::version_t(const std::string &name, const std::string &description) noexcept : name(name.data()), description(description.data()), major(0), minor(0), patch(0) {}

    /*
    * @brief Initialize a new version object with a given name.
    * @param name Name to give to the object.
    */
    version_t::version_t(const std::string &name) noexcept : name(name.data()), description(description.data()), major(0), minor(0), patch(0) {}

    /*
    * @brief Initialize a new version with given major, minor, and patch version numbers.
    * @param major Major version number.
    * @param minor Minor version number.
    * @param patch Patch version number.
    */
    version_t::version_t(std::size_t major, std::size_t minor, std::size_t patch) noexcept : name(std::string()), major(major), minor(minor), patch(patch) {}

    /*
    * @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
    * @param name Name with which to construct the version object.
    * @param major Major version number.
    * @param minor Minor version number.
    * @param patch Patch version number.
    */
    version_t::version_t(const std::string &name, std::size_t major, std::size_t minor, std::size_t patch) noexcept : name(name.data()), major(major), minor(minor), patch(patch) {}

    /*
    * @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
    * @param name Name with which to construct the version object.
    * @param description Description, or changelog, of the version.
    * @param major Major version number.
    * @param minor Minor version number.
    * @param patch Patch version number.
    */
    version_t::version_t(const std::string &name, const std::string &description, std::size_t major, std::size_t minor, std::size_t patch) noexcept : name(name.data()), description(description.data()), major(major), minor(minor), patch(patch) {}

    /*
    * @brief Release the version object.
    */
    void version_t::release() noexcept
    {
        major++;
        minor = 0;
        patch = 0;
    }

    /*
    * @brief Update the version object.
    */
    void version_t::update() noexcept
    {
        minor++;
        patch = 0;
    }

    /*
    * @brief Patch the version object.
    */
    void version_t::fix() noexcept
    {
        patch++;
    }

    /*
    * @brief Publish a new version. Sets the given version's major release number to 1.
    * @throws ValueError If the given version object is evaluated to already be published, a `ValueError` is printed to `stderr` and the programme exits.
    */
    void version_t::publish()
    {
        if (is_public())
        {
            throw ValueError("Version is already public.");
        }
        major = 1;
        minor = 0;
        patch = 0;
    }

    /*
    * @brief Compare the major release of a given version object.
    * @param major Major release number to compare.
    * @returns False if the given version object's major version is not equal to the given major parametre, else true.
    */
    bool version_t::compare_major(std::size_t major) const noexcept
    {
        return this->major >= major;
    }

    /*
    * @brief Compare the minor release of a given version object.
    * @param minor Minor release number to compare.
    * @returns False if the given version object's minor version is not equal to the given minor parametre, else true.
    */
    bool version_t::compare_minor(std::size_t minor) const noexcept
    {
        return this->minor >= minor;
    }

    /*
    * @brief Compare the patch release of a given version object.
    * @param patch Patch release number to compare.
    * @returns False if the given version object's patch version is not equal to the given patch parametre, else true.
    */
    bool version_t::compare_patch(std::size_t patch) const noexcept
    {
        return this->patch >= patch;
    }

    /*
    * @brief Compare one given version object to another.
    * @param version Version object to compare.
    * @returns False if all of one given version object's properties are not equal to each other, else true.
    */
    bool version_t::compare(const version_t &version) const noexcept
    {
        return (compare_major(version.major) && compare_minor(version.minor) && compare_patch(version.patch));
    }

    bool version_t::is_public() const noexcept
    {
        return compare_major(1);
    }

    /*
    * @brief Obtain a string representation of the version object.
    * @returns A string representing a symantic version.
    */
    const char *version_t::to_string() const noexcept
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
    void version_t::operator<<(std::ostream &stream) const noexcept
    {
        std::stringstream full = std::stringstream();
        full << to_string();
        full << "\n";
        stream << full.str();
    }

    ValueError::ValueError(const std::string &message) : Exception(message.data())
    {
        _assign_name("ValueError");
    }

    void operator<<(std::ostream &stream, const version_t &version) noexcept
    {
        std::stringstream full = std::stringstream();
        full << version.to_string();
        full << "\n";
        stream << full.str();
    }
}

#endif // VERSION_IMPLEMENTATION