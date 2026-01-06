#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "collection.hpp"

namespace polutils
{
    namespace collections
    {
        /*
        * @brief Standardization of a linear array.
        */
        template <typename Type>
        struct array_t : public collection_t<Type>
        {
            public:
                /*
                * @brief Construct a new array with a fixed capacity.
                * @exception If the array can not be allocated, an `AllocationError` is thrown.
                */
                array_t(void);

                /*
                * @brief Construct a new array with a given capacity.
                * @exception If the array can not be allocated, an `AllocationError` is thrown.
                */
                array_t(std::size_t capacity);

                /*
                * @brief Append an item to the array. This method is marked as virtual and can be overridden.
                * @param item Item to be appended.
                * @exception If the array if full, a `ValueError` is thrown.
                */
                virtual void append(Type item) override;

                /*
                * @brief Obtain a mutable pointer to an element at a given index within the array. This method is marked as virtual and can be overridden.
                * @param index Index at which the element within the array is located.
                * @exception If the given index is greater than the size of the array, an `IndexError` is thrown.
                */
                virtual Type *at(std::size_t index) const override;

                /*
                * @brief Remove an element from the array at a given index. This method is marked as virtual and can be overridden.
                * @param index Index at which the element within the array is located.
                * @exception If the given index is greater than the size of the array, an `IndexError` is thrown.
                */
                virtual void remove(std::size_t index) override;

                /*
                * @brief Determine the size of the collection.
                * @returns The size of the collection.
                */
                std::size_t length() const noexcept;


                /*
                * @brief Determine if the collection is empty.
                * @returns True if the collection is determined to be empty, else false.
                */
                bool is_empty() const noexcept;

                /*
                * @brief Virtual destructor override to cleanup resources.
                */
                virtual ~array_t();

            protected:
                /*
                * @brief Resize the array by a factor of 2.
                * @exception If the array can not be reallocated, an `AllocationError` is thrown.
                */
                void _resize(void);

                /*
                * @brief Resize the array by a given scaler.
                * @param scaler Scaler value by which to resize the array.
                * @exception If the array can not be reallocated, an `AllocationError` is thrown.
                */
                void _resize(std::size_t scaler);

                /*
                * @brief Deallocate the array.
                */
                virtual void _delete(void);

            protected:
                Type *items;
                std::size_t size;
                std::size_t capacity;
        };
    }
}

#endif // ARRAY_HPP

#ifdef ARRAY_IMPLEMENTATION

#include <cstdlib> // std::calloc, std::free, nullptr

#define EXCEPTIONS_IMPLEMENTATION
#include "../exceptions.hpp"

#ifndef ARRAY_CAPACITY
#define ARRAY_CAPACITY 256
#endif // ARRAY_CAPACITY

namespace
{
    /*
    * @brief Determine whether a given size is between a given upper and lower boundry.
    * @param index Size to fit into each boundry.
    * @param lower_limit The lowest most value the index can be checked against.
    * @param upper_limit The highest most value the index can be checked against.
    * @returns Whether or not the given index is within the two given upper and lower boundry.
    */
    constexpr bool __check_bounds(std::size_t index, std::size_t lower_limit, std::size_t upper_limit) noexcept
    {
        return (index >= lower_limit && index <= upper_limit);
    }
}

namespace polutils
{
    namespace collections
    {
        /*
        * @brief Construct a new array with a fixed capacity.
        * @exception If the array can not be allocated, an `AllocationError` is thrown.
        */
        template <typename Type>
        array_t<Type>::array_t() : size(0), capacity(ARRAY_CAPACITY)
        {
            items = static_cast<Type *>(std::calloc(capacity, sizeof(Type)));
            if (nullptr == items)
            {
                throw AllocationError("Can not allocate enough memory for the array.");
            }
        }

        /*
        * @brief Construct a new array with a given capacity.
        * @exception If the array can not be allocated, an `AllocationError` is thrown.
        */
        template <typename Type>
        array_t<Type>::array_t(std::size_t capacity) : size(0), capacity(capacity)
        {
            items = static_cast<Type *>(std::calloc(capacity, sizeof(Type)));
            if (nullptr == items)
            {
                throw AllocationError("Can not allocate enough memory for the array.");
            }
        }

        /*
        * @brief Append an item to the array. This method is marked as virtual and can be overridden.
        * @param item Item to be appended.
        * @exception If the array if full, a `ValueError` is thrown.
        */
        template <typename Type>
        void array_t<Type>::append(Type item)
        {
            if (size >= capacity)
            {
                throw ValueError("Can not add an element to a full array.");
            }
            items[size++] = item;
        }

        /*
        * @brief Obtain a mutable pointer to an element at a given index within the array. This method is marked as virtual and can be overridden.
        * @param index Index at which the element within the array is located.
        * @exception If the given index is greater than the size of the array, an `IndexError` is thrown.
        */
        template <typename Type>
        Type *array_t<Type>::at(std::size_t index) const
        {
            if (!__check_bounds(index, 0, size))
            {
                throw IndexError("Can not access element outside of array capacity.");
            }
            return &items[index];
        }

        /*
        * @brief Remove an element from the array at a given index. This method is marked as virtual and can be overridden.
        * @param index Index at which the element within the array is located.
        * @exception If the given index is greater than the size of the array, an `OutOfRangeError` is thrown.
        * @exception If the array's memory can not be reallocated, an `AllocationError` is thrown.
        */
        template <typename Type>
        void array_t<Type>::remove(std::size_t index)
        {
            if (index >= size) {
                throw OutOfRangeError("Can not access element outside of array bounds.");
            }
            for (size_t i = index; i < size - 1; i++) {
                items[i] = items[i + 1];
            }
            size--;
            Type *temp = static_cast<Type*>(std::realloc(items, size * sizeof(Type)));
            if (temp == nullptr) {
                throw AllocationError("Can not resize array.");
            }
            items = temp;
        }

        /*
        * @brief Determine the size of the collection.
        * @returns The size of the collection.
        */
        template <typename Type>
        std::size_t array_t<Type>::length() const noexcept
        {
            return size;
        }

        /*
        * @brief Determine if the collection is empty.
        * @returns True if the collection is determined to be empty, else false.
        */
        template <typename Type>
        bool array_t<Type>::is_empty() const noexcept
        {
            return size == 0 || nullptr == items;
        }

        /*
        * @brief Resize the array by a factor of 2.
        * @exception If the array can not be reallocated, an `AllocationError` is thrown.
        */
        template <typename Type>
        void array_t<Type>::_resize()
        {
            _resize(2);
        }

        /*
        * @brief Resize the array by a given scaler.
        * @param scaler Scaler value by which to resize the array.
        * @exception If the array can not be reallocated, an `AllocationError` is thrown.
        */
        template <typename Type>
        void array_t<Type>::_resize(std::size_t scaler)
        {
            capacity *= scaler;
            items = static_cast<Type *>(std::realloc(items, sizeof(Type) * capacity));
            if (nullptr == items)
            {
                throw AllocationError("Can not resize array.");
            }
        }

        /*
        * @brief Deallocate the array.
        */
        template <typename Type>
        void array_t<Type>::_delete()
        {
            capacity = 0;
            size = 0;
            if (nullptr == items)
            {
                return;
            }
            std::free(items);
            items = nullptr;
        }

        /*
        * @brief Virtual destructor override to cleanup resources.
        */
        template <typename Type>
        array_t<Type>::~array_t()
        {
            _delete();
        }
    }
}

#endif // ARRAY_IMPLEMENTATION