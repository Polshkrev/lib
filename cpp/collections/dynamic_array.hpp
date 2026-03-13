#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#define ARRAY_IMPLEMENTATION
#include "array.hpp" // array_t, array_t::_resize

namespace polutils
{
    namespace collections
    {
        /**
         * @brief Construct a new dynamic array.
         * @exception If the array can not be allocated, an `AllocationError` is thrown.
         */
        template <typename Type>
        struct dynamic_array_t : public array_t<Type>
        {
            /**
             * @brief Construct a new dynamic array.
             * @exception If the array can not be allocated, an `AllocationError` is thrown.
             */
            explicit dynamic_array_t();

            /**
             * @brief Construct a new dynamic array with a given initial capacity.
             * @param capacity Initial capacity with which to initialize the new dynamic array.
             * @exception If the array can not be allocated, an `AllocationError` is thrown.
             */
            explicit dynamic_array_t(std::size_t capacity);

            /**
             * @brief Append an item to the dynamic array.
             * @param item Item to be appended.
             * @exception If the array can not be reallocated, an `AllocationError` is thrown.
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
        /**
         * @brief Construct a new dynamic array.
         * @exception If the array can not be allocated, an `AllocationError` is thrown.
         */
        template <typename Type>
        dynamic_array_t<Type>::dynamic_array_t() : array_t<Type>() {}

        /**
         * @brief Construct a new dynamic array with a given initial capacity.
         * @param capacity Initial capacity with which to initialize the new dynamic array.
         * @exception If the array can not be allocated, an `AllocationError` is thrown.
         */
        template <typename Type>
        dynamic_array_t<Type>::dynamic_array_t(std::size_t capacity) : array_t<Type>(capacity) {}

        /**
         * @brief Append an item to the dynamic array.
         * @param item Item to be appended.
         * @exception If the array can not be reallocated, an `AllocationError` is thrown.
         */
        template <typename Type>
        void dynamic_array_t<Type>::append(Type item)
        {
            if (this->is_full())
            {
                this->_resize();
            }
            this->__items[this->__size++] = item;
        }
    }
}

#endif // DYNAMIC_ARRAY_IMPLEMENTATION