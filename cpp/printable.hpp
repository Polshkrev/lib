#ifndef PRINTABLE_HPP_
#define PRINTABLE_HPP_

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
    };

    /**
     * @brief Print a given printable.
     * @param printable Printable to print.
     */
    void print(const printable_t &printable) noexcept;
}

#endif // PRINTABLE_HPP_

#ifdef PRINTABLE_IMPLEMENTATION

namespace polutils
{
    /**
     * @brief Print a given printable.
     * @param printable Printable to print.
     */
    void print(const printable_t &printable) noexcept
    {
        std::printf("%s\n", printable.to_string());
    }
}

#endif // PRINTABLE_IMPLEMENTATION