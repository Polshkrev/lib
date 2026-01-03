#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include <cstddef> // std::size_t

namespace polutils
{
    namespace collections
    {
        /*
        * @brief Interface to standardize a linear collection.
        */
        template <typename Type>
        struct collection_t
        {
            /*
            * @brief Append a given item to a collection.
            * @param item Item to append to the collection.
            */
            virtual void append(Type item) = 0;
            /*
            * @brief Obtain a mutable pointer to an item within the collection at a given index.
            * @param index Index at which an item is located.
            * @returns a mutable pointer to an item located within the collection at a given index.
            * @exception If the given index is greater than the size of the collection, an `IndexError` is thrown.
            */
            virtual Type *at(std::size_t index) const = 0;
            /*
            * @brief Remove an item within the collection at a given index.
            * @param index Index in the collection at which the removeable value is located.
            * @exception If the given index is greater than the size of the collection, an `IndexError` is thrown.
            */
            virtual void remove(std::size_t index) = 0;
        };
    }
}

#endif // COLLECTION_HPP