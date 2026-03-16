#ifndef VERSION_HPP
#define VERSION_HPP

#include <cstddef> // size_t
#include <string> // string
#include <ostream> // ostream

namespace polutils
{
    /**
     * @brief Representation of a semantic-versioning object.
     */
    struct version_t
    {
        /**
         * @brief Initialize a new version object.
         */
        explicit version_t() noexcept;

        /**
         * @brief Initialize a new version object with a given name.
         * @param name Name to give to the object.
         */
        version_t(const std::string &name) noexcept;

        /**
         * @brief Initialize a new version object with a given name.
         * @param description Description, or changelog, of the version.
         * @param name Name to give to the object.
         */
        version_t(const std::string &name, const std::string &description) noexcept;

        /**
         * @brief Construct a new version with given major, minor, and patch version numbers.
         * @param major Major version number.
         * @param minor Minor version number.
         * @param patch Patch version number.
         */
        version_t(std::size_t major, std::size_t minor, std::size_t patch) noexcept;

        /**
         * @brief Construct a new version object with a given name, major, minor, and patch version numbers.
         * @param name Name with which to construct the version object.
         * @param major Major version number.
         * @param minor Minor version number.
         * @param patch Patch version number.
         */
        version_t(const std::string &name, std::size_t major, std::size_t minor, std::size_t patch) noexcept;

        /**
         * @brief Construct a new version object with a given name, major, minor, and patch version numbers.
         * @param name Name with which to construct the version object.
         * @param description Description, or changelog, of the version.
         * @param major Major version number.
         * @param minor Minor version number.
         * @param patch Patch version number.
         */
        version_t(const std::string &name, const std::string &description, std::size_t major, std::size_t minor, std::size_t patch) noexcept;

        /**
         * @brief Operator << overload.
         * @param stream Stream to push.
         */
        void operator<<(std::ostream &stream) const noexcept;

        /**
         * @brief Obtain the major value of the version.
         * @returns The major value of the version.
         */
        std::size_t major() const noexcept;

        /**
         * @brief Obtain the minor value of the version.
         * @returns The minor value of the version.
         */
        std::size_t minor() const noexcept;

        /**
         * @brief Obtain the patch value of the version.
         * @returns The patch value of the version.
         */
        std::size_t patch() const noexcept;

        /**
         * @brief Obtain the name value of the version.
         * @returns The name value of the version.
         */
        std::string name() const noexcept;

        /**
         * @brief Obtain the description value of the version.
         * @returns The description value of the version.
         */
        std::string description() const noexcept;

        /**
         * @brief Set the major value of the version.
         * @param major The value to set to the major value.
         */
        void set_major(std::size_t major) noexcept;

        /**
         * @brief Set the minor value of the version.
         * @param minor The value to set to the minor value.
         */
        void set_minor(std::size_t minor) noexcept;

        /**
         * @brief Set the patch value of the version.
         * @param patch The value to set to the patch value.
         */
        void set_patch(std::size_t patch) noexcept;

        /**
         * @brief Set the name value of the version.
         * @param name The value to set to the name value.
         */
        void set_name(const std::string &name) noexcept;

        /**
         * @brief Set the description value of the version.
         * @param description The value to set to the description value.
         */
        void set_description(const std::string &description) noexcept;

        /**
         * @brief Release the version object.
         */
        void release() noexcept;

        /**
         * @brief Update the version object.
         */
        void update() noexcept;

        /**
         * @brief Patch the version object.
         */
        void fix() noexcept;

        /**
         * @brief Publish a new version. Sets the given version's major release number to one.
         * @exception If the given version object is evaluated to already be published, a `ValueError` is thrown.
         */
        void publish();

        /**
         * @brief Compare the major release of a given version object.
         * @param major Major release number to compare.
         * @returns False if the given version object's major version is not equal to the given major parametre, else true.
         */
        bool compare_major(std::size_t major) const noexcept;

        /**
         * @brief Compare the minor release of a given version object.
         * @param minor Minor release number to compare.
         * @returns False if the given version object's minor version is not equal to the given minor parametre, else true.
         */
        bool compare_minor(std::size_t minor) const noexcept;

        /**
         * @brief Compare the patch release of a given version object.
         * @param patch Patch release number to compare.
         * @returns False if the given version object's patch version is not equal to the given patch parametre, else true.
         */
        bool compare_patch(std::size_t patch) const noexcept;

        /**
         * @brief Compare one given version object to another.
         * @param version Version object to compare.
         * @returns False if all of one given version object's properties are not equal to each other, else true.
         */
        bool compare(const version_t &version) const noexcept;

        /**
         * @brief Check is a given version is publicly available.
         * @returns False if the given version object's major release number is less than 1, else true.
         */
        bool is_public() const noexcept;

        /**
         * @brief Obtain a string representation of the version object.
         * @returns A string representing a symantic version.
         */
        std::string to_string() const noexcept;
        private:
            std::size_t __major;
            std::size_t __minor;
            std::size_t __patch;
            std::string __name;
            std::string __description;
    };

    /**
     * @brief Operator << overload.
     * @param stream Stream to push.
     */
    void operator<<(std::ostream &stream, const version_t &version) noexcept;
}

#endif // VERSION_HPP

#ifdef VERSION_IMPLEMENTATION

#include <sstream> // stringstream

#define EXCEPTIONS_IMPLEMENTATION
#include "exceptions.hpp" // ValueError

namespace polutils
{
    /**
     * @brief Initialize a new version object.
     */
    version_t::version_t() noexcept : __major(0), __minor(0), __patch(0), __name(std::string()), __description(std::string())  {}

    /**
     * @brief Initialize a new version object with a given name.
     * @param description Description, or changelog, of the version.
     * @param name Name to give to the object.
     */
    version_t::version_t(const std::string &name, const std::string &description) noexcept : __major(0), __minor(0), __patch(0), __name(name.data()), __description(description.data()) {}

    /**
     * @brief Initialize a new version object with a given name.
     * @param name Name to give to the object.
     */
    version_t::version_t(const std::string &name) noexcept : __major(0), __minor(0), __patch(0), __name(name.data()), __description(std::string()) {}

    /**
     * @brief Initialize a new version with given major, minor, and patch version numbers.
     * @param major Major version number.
     * @param minor Minor version number.
     * @param patch Patch version number.
     */
    version_t::version_t(std::size_t major, std::size_t minor, std::size_t patch) noexcept : __major(major), __minor(minor), __patch(patch), __name(std::string()), __description(std::string()) {}

    /**
     * @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
     * @param name Name with which to construct the version object.
     * @param major Major version number.
     * @param minor Minor version number.
     * @param patch Patch version number.
     */
    version_t::version_t(const std::string &name, std::size_t major, std::size_t minor, std::size_t patch) noexcept : __major(major), __minor(minor), __patch(patch), __name(name.data()), __description(std::string()) {}

    /**
     * @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
     * @param name Name with which to construct the version object.
     * @param description Description, or changelog, of the version.
     * @param major Major version number.
     * @param minor Minor version number.
     * @param patch Patch version number.
     */
    version_t::version_t(const std::string &name, const std::string &description, std::size_t major, std::size_t minor, std::size_t patch) noexcept : __major(major), __minor(minor), __patch(patch), __name(name.data()), __description(description.data()) {}

    /**
     * @brief Obtain the major value of the version.
     * @returns The major value of the version.
     */
    std::size_t version_t::major() const noexcept
    {
        return __major;
    }

    /**
     * @brief Obtain the minor value of the version.
     * @returns The minor value of the version.
     */
    std::size_t version_t::minor() const noexcept
    {
        return __minor;
    }

    /**
     * @brief Obtain the patch value of the version.
     * @returns The patch value of the version.
     */
    std::size_t version_t::patch() const noexcept
    {
        return __patch;
    }
    
    /**
     * @brief Obtain the name value of the version.
     * @returns The name value of the version.
     */
    std::string version_t::name() const noexcept
    {
        return __name;
    }

    /**
     * @brief Obtain the major value of the version.
     * @returns The major value of the version.
     */
    std::string version_t::description() const noexcept
    {
        return __description;
    }

    /**
     * @brief Set the major value of the version.
     * @param major The value to set to the major value.
     */
    void version_t::set_major(std::size_t major) noexcept
    {
        __major = major;
    }

    /**
     * @brief Set the minor value of the version.
     * @param minor The value to set to the minor value.
     */
    void version_t::set_minor(std::size_t minor) noexcept
    {
        __minor = minor;
    }

    /**
     * @brief Set the patch value of the version.
     * @param patch The value to set to the patch value.
     */
    void version_t::set_patch(std::size_t patch) noexcept
    {
        __patch = patch;
    }

    /**
     * @brief Set the name value of the version.
     * @param name The value to set to the name value.
     */
    void version_t::set_name(const std::string &name) noexcept
    {
        __name = name;
    }

    /**
     * @brief Set the description value of the version.
     * @param description The value to set to the description value.
     */
    void version_t::set_description(const std::string &description) noexcept
    {
        __description = description;
    }

    /**
     * @brief Release the version object.
     */
    void version_t::release() noexcept
    {
        __major++;
        __minor = 0;
        __patch = 0;
    }

    /**
     * @brief Update the version object.
     */
    void version_t::update() noexcept
    {
        __minor++;
        __patch = 0;
    }

    /**
     * @brief Patch the version object.
     */
    void version_t::fix() noexcept
    {
        __patch++;
    }

    /**
     * @brief Publish a new version. Sets the given version's major release number to one.
     * @exception If the given version object is evaluated to already be published, a `ValueError` is thrown.
     */
    void version_t::publish()
    {
        if (is_public())
        {
            throw ValueError("Version is already public.");
        }
        __major = 1;
        __minor = 0;
        __patch = 0;
    }

    /**
     * @brief Compare the major release of a given version object.
     * @param major Major release number to compare.
     * @returns False if the given version object's major version is not equal to the given major parametre, else true.
     */
    bool version_t::compare_major(std::size_t major) const noexcept
    {
        return __major >= major;
    }

    /**
     * @brief Compare the minor release of a given version object.
     * @param minor Minor release number to compare.
     * @returns False if the given version object's minor version is not equal to the given minor parametre, else true.
     */
    bool version_t::compare_minor(std::size_t minor) const noexcept
    {
        return __minor >= minor;
    }

    /**
     * @brief Compare the patch release of a given version object.
     * @param patch Patch release number to compare.
     * @returns False if the given version object's patch version is not equal to the given patch parametre, else true.
     */
    bool version_t::compare_patch(std::size_t patch) const noexcept
    {
        return __patch >= patch;
    }

    /**
     * @brief Compare one given version object to another.
     * @param version Version object to compare.
     * @returns False if all of one given version object's properties are not equal to each other, else true.
     */
    bool version_t::compare(const version_t &version) const noexcept
    {
        return (compare_major(version.major()) && compare_minor(version.minor()) && compare_patch(version.patch()));
    }

    /**
     * @brief Determine if the version is public.
     * @returns True if the version's major value is greater than or equal to one, else false.
     */
    bool version_t::is_public() const noexcept
    {
        return compare_major(1);
    }

    /**
     * @brief Obtain a string representation of the version object.
     * @returns A string representing a symantic version.
     */
    std::string version_t::to_string() const noexcept
    {
        std::stringstream result = std::stringstream();
        if (__name.empty() && __description.empty())
        {
            result << __major;
            result << ".";
            result << __minor;
            result << ".";
            result << __patch;
            // std::cout << result.str() << std::endl;
            return result.str();
        }
        else if ((__name.empty()) && (!(__description.empty())))
        {
            result << __major;
            result << ".";
            result << __minor;
            result << ".";
            result << __patch;
            result << " - ";
            result << __description;
            // std::cout << result.str() << std::endl;
            return result.str();
        }
        else if ((!(__name.empty())) && (__description.empty()))
        {
            result << __name;
            result << ": ";
            result << __major;
            result << ".";
            result << __minor;
            result << ".";
            result << __patch;
            // std::cout << result.str() << std::endl;
            return result.str();
        }
        result << __name;
        result << ": ";
        result << __major;
        result << ".";
        result << __minor;
        result << ".";
        result << __patch;
        result << " - ";
        result << __description;
        // std::cout << result.str() << "hello" << std::endl;
        return result.str();
    }

    /**
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

    /**
     * @brief Operator << overload.
     * @param stream Stream to push.
     */
    void operator<<(std::ostream &stream, const version_t &version) noexcept
    {
        std::stringstream full = std::stringstream();
        full << version.to_string();
        full << "\n";
        stream << full.str();
    }
}

#endif // VERSION_IMPLEMENTATION