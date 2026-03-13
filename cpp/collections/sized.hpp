#ifndef SIZED_HPP
#define SIZED_HPP

#include <cstddef> // std::size_t
#include <cstdbool> // bool

namespace polutils
{
    namespace collections
    {
        /**
         * @brief Representation of a sized collection.
         */
        struct sized_t
        {
            /**
             * @brief Obtain the length of the collection.
             * @returns The length of the collection.
             */
            virtual std::size_t size() const noexcept = 0;

            /**
             * @brief Determine if the collection is empty.
             * @returns True if the collection is determined to be empty, else false.
             */
            virtual bool is_empty() const noexcept = 0;

            /**
             * @brief Determine if the array is full.
             * @returns True if the size of the array is greater than or equal to its capacity.
             */
            virtual bool is_full() const noexcept = 0;
        };
    }
}


#endif // SIZED_HPP