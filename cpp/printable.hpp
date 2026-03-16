#ifndef PRINTABLE_HPP
#define PRINTABLE_HPP

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
}

#endif // PRINTABLE_HPP