#ifndef PRINTABLE_HPP_
#define PRINTABLE_HPP_

#include "doc.hpp" // ! NEEDED FOR NAMESPACE DOCUMENTATION

#include <cstdio> // std::printf, std::fprintf, std::FILE

#include <ostream> // std::ostream

namespace polutils
{
    /**
     * @brief Stringify interface.
     */
    struct printable_t
    {
        /**
         * @brief Convert a custom type into a string to be printed.
         * @returns A c-string to be printed.
         */
        virtual const char *to_string(void) const noexcept = 0;

        /**
         * @brief Default destructor.
         */
        virtual ~printable_t(void) noexcept = default;
    };

    /**
     * @brief Insert a printable into a C++ stream.
     * @param stream Stream to which to insert.
     * @param printable Printable to insert.
     * @returns The given stream.
     */
    std::ostream &operator<<(std::ostream &stream, const printable_t &printable);

    /**
     * @brief Print a given printable.
     * @param printable Printable to print.
     */
    void print(const printable_t &printable) noexcept;

    /**
     * @brief Print a given printable.
     * @param stream Stream to which to print.
     * @param printable Printable to print.
     */
    void print(std::FILE *stream, const printable_t &printable) noexcept;

    /**
     * @brief Print a given printable.
     * @param stream Stream to which to print.
     * @param printable Printable to print.
     */
    void print(std::ostream &stream, const printable_t &printable) noexcept;

    /**
     * @brief Print a given printable.
     * @param printable Printable to print.
     */
    void println(const printable_t &printable) noexcept;

    /**
     * @brief Print a given printable.
     * @param stream Stream to which to print.
     * @param printable Printable to print.
     */
    void println(std::FILE *stream, const printable_t &printable) noexcept;

    /**
     * @brief Print a given printable.
     * @param stream Stream to which to print.
     * @param printable Printable to print.
     */
    void println(std::ostream &stream, const printable_t &printable) noexcept;
}

#endif // PRINTABLE_HPP_

#ifdef PRINTABLE_IMPLEMENTATION

namespace polutils
{

    /**
     * @brief Insert a printable into a C++ stream.
     * @param stream Stream to which to insert.
     * @param printable Printable to insert.
     * @returns The given stream.
     */
    std::ostream &operator<<(std::ostream &stream, const printable_t &printable)
    {
        return stream << printable.to_string();
    }

    /**
     * @brief Print a given printable.
     * @param printable Printable to print.
     */
    void print(const printable_t &printable) noexcept
    {
        std::printf("%s", printable.to_string());
    }

    /**
     * @brief Print a given printable.
     * @param stream Stream to which to print.
     * @param printable Printable to print.
     */
    void print(std::FILE *stream, const printable_t &printable) noexcept
    {
        if (!stream) return;
        std::fprintf(stream, "%s", printable.to_string());
    }

    /**
     * @brief Print a given printable.
     * @param stream Stream to which to print.
     * @param printable Printable to print.
     */
    void print(std::ostream &stream, const printable_t &printable) noexcept
    {
        stream << printable;
    }

    /**
     * @brief Print a given printable.
     * @param printable Printable to print.
     */
    void println(const printable_t &printable) noexcept
    {
        std::printf("%s\n", printable.to_string());
    }

    /**
     * @brief Print a given printable.
     * @param stream Stream to which to print.
     * @param printable Printable to print.
     */
    void println(std::FILE *stream, const printable_t &printable) noexcept
    {
        if (!stream) return;
        std::fprintf(stream, "%s\n", printable.to_string());
    }

    /**
     * @brief Print a given printable.
     * @param stream Stream to which to print.
     * @param printable Printable to print.
     */
    void println(std::ostream &stream, const printable_t &printable) noexcept
    {
        stream << printable << '\n';
    }
}

#endif // PRINTABLE_IMPLEMENTATION