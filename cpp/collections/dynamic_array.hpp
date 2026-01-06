#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#define ARRAY_IMPLEMENTATION
#include "./array.hpp" // polutils::collections::array_t

namespace polutils
{
    namespace collections
    {
        /*
        * @brief Implementation of a classical dynamic array.
        */
        template <typename Type>
        struct dynamic_array_t : public array_t<Type>
        {
            /*
            * @brief Construct a new dynamic array.
            */
            explicit dynamic_array_t();

            /*
            * @brief Construct a new dynamic array with a given initial capacity.
            * @param capacity Initial capacity with which to initialize the new dynamic array.
            */
            explicit dynamic_array_t(std::size_t capacity);

            /*
            * @brief Append an item to the dynamic array.
            * @param item Item to be appended.
            */
            void append(Type item) override;
        };
    }
}

#endif // DYNAMIC_ARRAY_HPP

#ifdef DYNAMIC_ARRAY_IMPLEMENTATION

namespace polutils
{
    namespace collections
    {
        /*
        * @brief Construct a new dynamic array.
        */
        template <typename Type>
        dynamic_array_t<Type>::dynamic_array_t() : array_t<Type>() {}

        /*
        * @brief Construct a new dynamic array with a given initial capacity.
        * @param capacity Initial capacity with which to initialize the new dynamic array.
        */
        template <typename Type>
        dynamic_array_t<Type>::dynamic_array_t(std::size_t capacity) : array_t<Type>(capacity) {}

        /*
        * @brief Append an item to the dynamic array.
        * @param item Item to be appended.
        */
        template <typename Type>
        void dynamic_array_t<Type>::append(Type item)
        {
            if (this->size >= this->capacity)
            {
                this->_resize();
            }
            this->items[this->size++] = item;
        }
    }
}

#endif // DYNAMIC_ARRAY_IMPLEMENTATION