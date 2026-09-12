#ifndef VERSION_HPP
#define VERSION_HPP

#include "doc.hpp" // ! NEEDED FOR NAMESPACE DOCUMENTATION

#include <cstdint> // std::uint8_t, std::uint16_t, UINT8_MAX
#include <istream> // std::istream
#include <ostream> // std::ostream
#include <string> // std::string

#include "printable.hpp" // printable_t

namespace polutils
{
    /**
     * @brief Representation of a semantic-versioning object.
     */
    struct version_t : public printable_t
    {
        /**
         * @brief Initialize a new version object.
         */
        explicit version_t(void) noexcept;

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
        version_t(std::uint8_t major, std::uint8_t minor, std::uint8_t patch) noexcept;

        /**
         * @brief Construct a new version object with a given name, major, minor, and patch version numbers.
         * @param name Name with which to construct the version object.
         * @param major Major version number.
         * @param minor Minor version number.
         * @param patch Patch version number.
         */
        version_t(const std::string &name, std::uint8_t major, std::uint8_t minor, std::uint8_t patch) noexcept;

        /**
         * @brief Construct a new version object with a given name, major, minor, and patch version numbers.
         * @param name Name with which to construct the version object.
         * @param description Description, or changelog, of the version.
         * @param major Major version number.
         * @param minor Minor version number.
         * @param patch Patch version number.
         */
        version_t(const std::string &name, const std::string &description, std::uint8_t major, std::uint8_t minor, std::uint8_t patch) noexcept;

        /**
         * @brief Obtain the major value of the version.
         * @returns The major value of the version.
         */
        std::uint8_t major(void) const noexcept;

        /**
         * @brief Obtain the minor value of the version.
         * @returns The minor value of the version.
         */
        std::uint8_t minor(void) const noexcept;

        /**
         * @brief Obtain the patch value of the version.
         * @returns The patch value of the version.
         */
        std::uint8_t patch(void) const noexcept;
        
        /**
         * @brief Obtain the name value of the version.
         * @returns The name value of the version.
         */
        const std::string &name(void) const noexcept;

        /**
         * @brief Obtain the description value of the version.
         * @returns The description value of the version.
         */
        const std::string &description(void) const noexcept;

        /**
         * @brief Set the major value of the version.
         * @param major The value to set to the major value.
         */
        void set_major(std::uint8_t major) noexcept;

        /**
         * @brief Set the minor value of the version.
         * @param minor The value to set to the minor value.
         */
        void set_minor(std::uint8_t minor) noexcept;

        /**
         * @brief Set the patch value of the version.
         * @param patch The value to set to the patch value.
         */
        void set_patch(std::uint8_t patch) noexcept;

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
         * @exception If the major value has reached the maximum `uint8_t`, a `ValueError` is thrown.
         */
        void release(void);

        /**
         * @brief Update the version object.
         * @exception If the minor value has reached the maximum `uint8_t`, a `ValueError` is thrown.
         */
        void update(void);

        /**
         * @brief Patch the version object.
         * @exception If the patch value has reached the maximum `uint8_t`, a `ValueError` is thrown.
         */
        void fix(void);

        /**
         * @brief Publish a new version. Sets the given version's major release number to one.
         * @exception If the given version object is evaluated to already be published, a `ValueError` is thrown.
         */
        void publish(void);

        /**
         * @brief Compare the major release of a given version object.
         * @param major Major release number to compare.
         * @returns False if the given version object's major version is not greater than or equal to the given major parametre, else true.
         */
        bool compare_major(std::uint8_t major) const noexcept;

        /**
         * @brief Compare the minor release of a given version object.
         * @param minor Minor release number to compare.
         * @returns False if the given version object's minor version is not greater than or equal to the given minor parametre, else true.
         */
        bool compare_minor(std::uint8_t minor) const noexcept;

        /**
         * @brief Compare the patch release of a given version object.
         * @param patch Patch release number to compare.
         * @returns False if the given version object's patch version is not greater than or equal to the given patch parametre, else true.
         */
        bool compare_patch(std::uint8_t patch) const noexcept;

        /**
         * @brief Compare one given version object to another.
         * @param version Version object to compare.
         * @returns False if all of one given version object's properties are not greater than or equal to each other, else true.
         */
        bool compare(const version_t &version) const noexcept;

        /**
         * @brief Determine if the version is public.
         * @returns True if the version's major value is greater than or equal to one, else false.
         */
        bool is_public(void) const noexcept;

        /**
         * @brief Obtain a string representation of the version object.
         * @returns A string representing a semantic version.
         */
        const char *to_string(void) const noexcept override;

        protected:
            /**
             * @brief Obtain a string representation of the version object.
             * @returns A string representing a semantic version.
             */
            std::string _string(void) const noexcept;

        private:
            /**
             * @brief Major value of the version.
             */
            std::uint8_t __major;
            /**
             * @brief Minor value of the version.
             */
            std::uint8_t __minor;
            /**
             * @brief Patch value of the version.
             */
            std::uint8_t __patch;
            /**
             * @brief Name value of the version.
             */
            std::string __name;
            /**
             * @brief Description value of the version.
             */
            std::string __description;
    };

    /**
     * @brief Operator `<<` overload.
     * @param stream Stream to push.
     * @param version Version to append to the stream.
     * @returns The given stream with the given version appended.
     */
    std::ostream &operator<<(std::ostream &stream, const version_t &version);

    /**
     * @brief Operator `>>` overload.
     * @param stream Stream from which to extract a version.
     * @param version Version object to populate.
     * @returns The given stream after extracting the version.
     */
    std::istream &operator>>(std::istream &stream, version_t &version);

    /**
     * @brief Determine if two versions are equal to each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is equal to the right.
     */
    bool operator==(const version_t &left, const version_t &right) noexcept;

    /**
     * @brief Determine if two versions are not equal to each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is not equal to the right.
     */
    bool operator!=(const version_t &left, const version_t &right) noexcept;

    /**
     * @brief Determine if two versions are less than each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is less than the right.
     */
    bool operator<(const version_t &left, const version_t &right) noexcept;

    /**
     * @brief Determine if two versions are less than or equal to each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is less than or equal to the right.
     */
    bool operator<=(const version_t &left, const version_t &right) noexcept;

    /**
     * @brief Determine if two versions are greater than each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is greater than the right.
     */
    bool operator>(const version_t &left, const version_t &right) noexcept;

    /**
     * @brief Determine if two versions are greater than or equal each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is greater than or equal to the right.
     */
    bool operator>=(const version_t &left, const version_t &right) noexcept;

}

#endif // VERSION_HPP

#ifdef VERSION_IMPLEMENTATION

#include <sstream> // std::stringstream

#define EXCEPTIONS_IMPLEMENTATION
#include "exceptions.hpp" // ValueError

namespace
{
    /**
     * @brief Generic print function for a version.
     * @param stream Stream to which to append a version string.
     * @param version Version to append to the given stream.
     * @param newline If true is passed in, a newline is appended.
     */
    void _version_print(std::ostream &stream, const polutils::version_t &version, bool newline) noexcept
    {
        if (!version.name().empty())
        {
            stream << version.name() << ": ";
        }

        stream << static_cast<std::uint16_t>(version.major()) << "." << static_cast<std::uint16_t>(version.minor()) << "." << static_cast<std::uint16_t>(version.patch());

        if (!version.description().empty())
        {
            stream << " - " << version.description();
        }

        if (newline)
        {
            stream << "\n";
        }
    }
}

namespace polutils
{
    /**
     * @brief Initialize a new version object.
     */
    version_t::version_t(void) noexcept : version_t(std::string(), std::string(), 0, 0, 0)  {}

    /**
     * @brief Initialize a new version object with a given name.
     * @param description Description, or changelog, of the version.
     * @param name Name to give to the object.
     */
    version_t::version_t(const std::string &name, const std::string &description) noexcept : version_t(name, description, 0, 0, 0) {}

    /**
     * @brief Initialize a new version object with a given name.
     * @param name Name to give to the object.
     */
    version_t::version_t(const std::string &name) noexcept : version_t(name, std::string(), 0, 0, 0) {}

    /**
     * @brief Initialize a new version with given major, minor, and patch version numbers.
     * @param major Major version number.
     * @param minor Minor version number.
     * @param patch Patch version number.
     */
    version_t::version_t(std::uint8_t major, std::uint8_t minor, std::uint8_t patch) noexcept : version_t(std::string(), std::string(), major, minor, patch) {}

    /**
     * @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
     * @param name Name with which to construct the version object.
     * @param major Major version number.
     * @param minor Minor version number.
     * @param patch Patch version number.
     */
    version_t::version_t(const std::string &name, std::uint8_t major, std::uint8_t minor, std::uint8_t patch) noexcept : version_t(name, std::string(), major, minor, patch) {}

    /**
     * @brief Initialize a new version object with a given name, major, minor, and patch version numbers.
     * @param name Name with which to construct the version object.
     * @param description Description, or changelog, of the version.
     * @param major Major version number.
     * @param minor Minor version number.
     * @param patch Patch version number.
     */
    version_t::version_t(const std::string &name, const std::string &description, std::uint8_t major, std::uint8_t minor, std::uint8_t patch) noexcept : __major(major), __minor(minor), __patch(patch), __name(name), __description(description) {}

    /**
     * @brief Obtain the major value of the version.
     * @returns The major value of the version.
     */
    std::uint8_t version_t::major(void) const noexcept
    {
        return __major;
    }

    /**
     * @brief Obtain the minor value of the version.
     * @returns The minor value of the version.
     */
    std::uint8_t version_t::minor(void) const noexcept
    {
        return __minor;
    }

    /**
     * @brief Obtain the patch value of the version.
     * @returns The patch value of the version.
     */
    std::uint8_t version_t::patch(void) const noexcept
    {
        return __patch;
    }
    
    /**
     * @brief Obtain the name value of the version.
     * @returns The name value of the version.
     */
    const std::string &version_t::name(void) const noexcept
    {
        return __name;
    }

    /**
     * @brief Obtain the description value of the version.
     * @returns The description value of the version.
     */
    const std::string &version_t::description(void) const noexcept
    {
        return __description;
    }

    /**
     * @brief Set the major value of the version.
     * @param major The value to set to the major value.
     */
    void version_t::set_major(std::uint8_t major) noexcept
    {
        __major = major;
    }

    /**
     * @brief Set the minor value of the version.
     * @param minor The value to set to the minor value.
     */
    void version_t::set_minor(std::uint8_t minor) noexcept
    {
        __minor = minor;
    }

    /**
     * @brief Set the patch value of the version.
     * @param patch The value to set to the patch value.
     */
    void version_t::set_patch(std::uint8_t patch) noexcept
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
     * @exception If the major value has reached the maximum `uint8_t`, a `ValueError` is thrown.
     */
    void version_t::release(void)
    {
        if (__major == UINT8_MAX)
        {
            throw ValueError("The major version has reached the maximum allowed by type.");
        }
        __major++;
        __minor = 0;
        __patch = 0;
    }

    /**
     * @brief Update the version object.
     * @exception If the minor value has reached the maximum `uint8_t`, a `ValueError` is thrown.
     */
    void version_t::update(void)
    {
        if (__minor == UINT8_MAX)
        {
            throw ValueError("The minor version has reached the maximum allowed by type.");
        }
        __minor++;
        __patch = 0;
    }

    /**
     * @brief Patch the version object.
     * @exception If the patch value has reached the maximum `uint8_t`, a `ValueError` is thrown.
     */
    void version_t::fix(void)
    {
        if (__patch == UINT8_MAX)
        {
            throw ValueError("The patch version has reached the maximum allowed by type.");
        }
        __patch++;
    }

    /**
     * @brief Publish a new version. Sets the given version's major release number to one.
     * @exception If the given version object is evaluated to already be published, a `ValueError` is thrown.
     */
    void version_t::publish(void)
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
     * @returns False if the given version object's major version is not greater than or equal to the given major parametre, else true.
     */
    bool version_t::compare_major(std::uint8_t major) const noexcept
    {
        return __major >= major;
    }

    /**
     * @brief Compare the minor release of a given version object.
     * @param minor Minor release number to compare.
     * @returns False if the given version object's minor version is not greater than or equal to the given minor parametre, else true.
     */
    bool version_t::compare_minor(std::uint8_t minor) const noexcept
    {
        return __minor >= minor;
    }

    /**
     * @brief Compare the patch release of a given version object.
     * @param patch Patch release number to compare.
     * @returns False if the given version object's patch version is not greater than or equal to the given patch parametre, else true.
     */
    bool version_t::compare_patch(std::uint8_t patch) const noexcept
    {
        return __patch >= patch;
    }

    /**
     * @brief Compare one given version object to another.
     * @param version Version object to compare.
     * @returns False if all of one given version object's properties are not greater than or equal to each other, else true.
     */
    bool version_t::compare(const version_t &version) const noexcept
    {
        return *this >= version;
    }

    /**
     * @brief Determine if the version is public.
     * @returns True if the version's major value is greater than or equal to one, else false.
     */
    bool version_t::is_public(void) const noexcept
    {
        return compare_major(1);
    }

    /**
     * @brief Obtain a string representation of the version object.
     * @returns A string representing a semantic version.
     */
    std::string version_t::_string(void) const noexcept
    {
        std::stringstream result = std::stringstream();
        _version_print(result, *this, false);
        return result.str();
    }

    /**
     * @brief Obtain a string representation of the version object.
     * @returns A string representing a semantic version.
     */
    const char *version_t::to_string(void) const noexcept
    {
        std::string string = _string();
        return string.c_str();
    }

    /**
     * @brief Operator `<<` overload.
     * @param stream Stream to push.
     * @param version Version to append to the stream.
     * @returns The given stream with the given version appended.
     */
    std::ostream &operator<<(std::ostream &stream, const version_t &version)
    {
        stream << version.to_string();
        return stream;
    }

    /**
     * @brief Operator `>>` overload.
     * @param stream Stream from which to extract a version.
     * @param version Version object to populate.
     * @returns The given stream after extracting the version.
     */
    std::istream &operator>>(std::istream &stream, version_t &version)
    {
        unsigned int major;
        unsigned int minor;
        unsigned int patch;
        char separator;

        if (!(stream >> major)) return stream;

        else if (!(stream >> separator) || separator != '.')
        {
            stream.setstate(std::ios::failbit);
            return stream;
        }

        else if (!(stream >> minor)) return stream;

        else if (!(stream >> separator) || separator != '.')
        {
            stream.setstate(std::ios::failbit);
            return stream;
        }
        else if (!(stream >> patch)) return stream;

        else if (major > UINT8_MAX || minor > UINT8_MAX || patch > UINT8_MAX)
        {
            stream.setstate(std::ios::failbit);
            return stream;
        }

        version.set_major(static_cast<std::uint8_t>(major));
        version.set_minor(static_cast<std::uint8_t>(minor));
        version.set_patch(static_cast<std::uint8_t>(patch));

        return stream;
    }

    /**
     * @brief Determine if two versions are equal to each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is equal to the right.
     */
    bool operator==(const version_t &left, const version_t &right) noexcept
    {
        return left.major() == right.major() && left.minor() == right.minor() && left.patch() == right.patch();
    }

    /**
     * @brief Determine if two versions are not equal to each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is not equal to the right.
     */
    bool operator!=(const version_t &left, const version_t &right) noexcept
    {
        return !(left == right);
    }

    /**
     * @brief Determine if two versions are less than each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is less than the right.
     */
    bool operator<(const version_t &left, const version_t &right) noexcept
    {
        if (left.major() != right.major()) return left.major() < right.major();

        else if (left.minor() != right.minor()) return left.minor() < right.minor();

        return left.patch() < right.patch();
    }

    /**
     * @brief Determine if two versions are less than or equal to each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is less than or equal to the right.
     */
    bool operator<=(const version_t &left, const version_t &right) noexcept
    {
        if (left.major() != right.major()) return left.major() <= right.major();

        else if (left.minor() != right.minor()) return left.minor() <= right.minor();

        return left.patch() <= right.patch();
    }

    /**
     * @brief Determine if two versions are greater than each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is greater than the right.
     */
    bool operator>(const version_t &left, const version_t &right) noexcept
    {
        if (left.major() != right.major()) return left.major() > right.major();

        else if (left.minor() != right.minor()) return left.minor() > right.minor();

        return left.patch() > right.patch();
    }

    /**
     * @brief Determine if two versions are greater than or equal each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is greater than or equal to the right.
     */
    bool operator>=(const version_t &left, const version_t &right) noexcept
    {
        if (left.major() != right.major()) return left.major() >= right.major();

        else if (left.minor() != right.minor()) return left.minor() >= right.minor();

        return left.patch() >= right.patch();
    }

    /**
     * @brief Parse a given string representation of a version.
     * @param value String representation of a version.
     * @exception If any of the stream operations fail, a `ValueError` is thrown.
     */
    version_t parse(const std::string &value)
    {
        std::stringstream stream(value);

        std::uint16_t major;
        std::uint16_t minor;
        std::uint16_t patch;

        char separator;

        if (!(stream >> major)) throw ValueError("Invalid version.");
        else if (!(stream >> separator) || separator != '.') throw ValueError("Invalid version.");
        else if (!(stream >> minor)) throw ValueError("Invalid version.");
        else if (!(stream >> separator) || separator != '.') throw ValueError("Invalid version.");
        else if (!(stream >> patch)) throw ValueError("Invalid version.");
        else if (major > UINT8_MAX || minor > UINT8_MAX || patch > UINT8_MAX) throw ValueError("Version component exceeds UINT8_MAX.");
        else if (stream.peek() != std::char_traits<char>::eof()) throw ValueError("Invalid version.");

        return version_t(static_cast<std::uint8_t>(major), static_cast<std::uint8_t>(minor), static_cast<std::uint8_t>(patch));
    }
}

#endif // VERSION_IMPLEMENTATION