#ifndef DIRECTORY_HPP_
#define DIRECTORY_HPP_

#include "doc.hpp" // ! NEEDED FOR NAMESPACE DOCUMENTATION

#include "../collections/collection.hpp" // collections::collection_t

#define ENTRY_IMPLEMENTATION
#include "entry.hpp" // entry_t

namespace polutils
{
    namespace fayl
    {
        /**
         * @brief Representation of a directory on the file system.
         */
        struct directory_t : public collections::collection_t<entry_t>
        {
            /**
             * @brief Construct a directory type of the current working directory.
             * @exception If the internal `entry_t` array can not be allocated, an `AllocationError` is thrown.
             */
            explicit directory_t(void);

            /**
             * @brief Construct a directory of a given root.
             * @param root Root of the directory.
             * @exception If the internal `entry_t` array can not be allocated, an `AllocationError` is thrown.
             */
            explicit directory_t(const path_t &root);

            /**
             * @brief Construct a directory of a given initial capacity.
             * @param capacity Initial allocation capacity of the internal `entry_t` array.
             * @exception If the internal `entry_t` array can not be allocated, an `AllocationError` is thrown.
             */
            explicit directory_t(std::size_t capacity);

            /**
             * @brief Append a given `entry_t` item to the directory.
             * @param item Item to append to the directory.
             * @exception If the internal array can not be reallocated, an `AllocationError` is thrown.
             */
            void append(entry_t item) override;

            /**
             * @brief Obtain a mutable pointer to the `entry_t` stored at the given index.
             * @param index Index at which the entry is stored.
             * @exception If the given index is greater than or equal to the size of the array, an `IndexError` is thrown.
             */
            entry_t *at(std::size_t index) const override;

            /**
             * @brief Remove the entry stored at the given index.
             * @param index Index at which the entry is stored.
             * @exception If the directory is empty, an `IndexError` is thrown.
             * @exception If the given index is greater than or equal to the size of the directory, an `IndexError` is thrown.
             */
            void remove(std::size_t index) override;

            /**
             * @brief Obtain the size of the directory.
             * @returns The size of the directory.
             */
            std::size_t size(void) const noexcept override;

            /**
             * @brief Determine if the directory is empty.
             * @returns True if the directory's size is zero, else false.
             */
            bool is_empty(void) const noexcept override;

            /**
             * @brief Determine if the directory is full.
             * @returns True if the directory's size is greater than or equal to its capacity.
             */
            bool is_full(void) const noexcept override;

            /**
             * @brief Deallocate the directory.
             */
            virtual ~directory_t(void) noexcept;
            protected:
                /**
                 * @brief Resize the directory's internal `entry_t` array by a factor of two.
                 * @exception If the internal array can not be reallocated, an `AllocationError` is thrown.
                 */
                void _resize(void);

                /**
                 * @brief Resize the directory's internal `entry_t` array by a given scaler.
                 * @param scaler The scaler value by which to exponentially increase the capacity of the directory's internal array.
                 * @exception If the capacity is greater than the maximum allowed by type, an `OverflowError` is thrown.
                 * @exception If the internal array can not be reallocated, an `AllocationError` is thrown.
                 */
                void _resize(std::size_t scaler);

                /**
                 * @brief Deallocate the directory's internal `entry_t` array.
                 */
                void _delete(void) noexcept;
            private:

                /**
                 * @brief Construct a directory of a given root and initial capacity.
                 * @param root Root of the directory.
                 * @param capacity Initial allocation capacity of the internal `entry_t` array.
                 * @exception If the internal `entry_t` array can not be allocated, an `AllocationError` is thrown.
                 */
                directory_t(const path_t &root, std::size_t capacity);

                /**
                 * @brief Root of the directory.
                 */
                path_t __root;

                /**
                 * @brief Size of the directory.
                 */
                std::size_t __size;

                /**
                 * @brief Capacity of the directory.
                 */
                std::size_t __capacity;

                /**
                 * @brief Internal owned array of entries.
                 */
                entry_t *__entries;
        };
    }
}

#endif // DIRECTORY_HPP_

#ifdef DIRECTORY_IMPLEMENTATION

#include <cstddef> // std::size_t
#include <cstdint> // SIZE_MAX
#include <cstdlib> // std::malloc, std::realloc, std::free
#include <utility> // std::move

#include "../numeric.hpp" // CLAMP

namespace
{
    /**
     * @brief Initial directory capacity.
     */
    #define DIRECTORY_CAPACITY 256
}

namespace polutils
{
    namespace fayl
    {
        /**
         * @brief Construct a directory type of the current working directory.
         * @exception If the internal `entry_t` array can not be allocated, an `AllocationError` is thrown.
         */
        directory_t::directory_t(void) : directory_t(path_t(), DIRECTORY_CAPACITY) {}

        /**
         * @brief Construct a directory of a given root.
         * @param root Root of the directory.
         * @exception If the internal `entry_t` array can not be allocated, an `AllocationError` is thrown.
         */
        directory_t::directory_t(const path_t &root) : directory_t(root, DIRECTORY_CAPACITY) {}

        /**
         * @brief Construct a directory of a given initial capacity.
         * @param capacity Initial allocation capacity of the internal `entry_t` array.
         * @exception If the internal `entry_t` array can not be allocated, an `AllocationError` is thrown.
         */
        directory_t::directory_t(std::size_t capacity) : directory_t(path_t(), capacity) {}

        /**
         * @brief Construct a directory of a given root and initial capacity.
         * @param root Root of the directory.
         * @param capacity Initial allocation capacity of the internal `entry_t` array.
         * @exception If the internal `entry_t` array can not be allocated, an `AllocationError` is thrown.
         */
        directory_t::directory_t(const path_t &root, std::size_t capacity) : __root(root), __size(0), __capacity(CLAMP(capacity, 1, SIZE_MAX)), __entries(nullptr)
        {
            try
            {
                __entries = new entry_t[__capacity];
            }
            catch (const std::bad_alloc &)
            {
                throw AllocationError("Can not allocate enough memory for the array.");
            }
        }

        /**
         * @brief Append a given `entry_t` item to the directory.
         * @param item Item to append to the directory.
         * @exception If the internal array can not be reallocated, an `AllocationError` is thrown.
         */
        void directory_t::append(entry_t item)
        {
            if (is_full())
            {
                _resize();
            }
            __entries[__size++] = std::move(item);
        }

        /**
         * @brief Obtain a mutable pointer to the `entry_t` stored at the given index.
         * @param index Index at which the entry is stored.
         * @exception If the given index is greater than or equal to the size of the array, an `IndexError` is thrown.
         */
        entry_t *directory_t::at(std::size_t index) const
        {
            if (index >= size())
            {
                throw IndexError("Can not access directory of size %zu at index %zu.", size(), index);
            }
            return &__entries[index];
        }

        /**
         * @brief Obtain the size of the directory.
         * @returns The size of the directory.
         */
        std::size_t directory_t::size(void) const noexcept
        {
            return __size;
        }

        /**
         * @brief Determine if the directory is empty.
         * @returns True if the directory's size is zero, else false.
         */
        bool directory_t::is_empty(void) const noexcept
        {
            return __size == 0;
        }

        /**
         * @brief Determine if the directory is full.
         * @returns True if the directory's size is greater than or equal to its capacity.
         */
        bool directory_t::is_full(void) const noexcept
        {
            return __size >= __capacity;
        }

        /**
         * @brief Remove the entry stored at the given index.
         * @param index Index at which the entry is stored.
         * @exception If the directory is empty, an `IndexError` is thrown.
         * @exception If the given index is greater than or equal to the size of the directory, an `IndexError` is thrown.
         */
        void directory_t::remove(std::size_t index)
        {
            if (is_empty())
            {
                throw IndexError("Can not remove from an empty directory.");
            }
            else if (index >= size())
            {
                throw IndexError("Can not remove from directory of size %zu at index %zu.", size(), index);
            }
            for (std::size_t current = index; current < __size - 1; ++current)
            {
                __entries[current] = std::move(__entries[current + 1]);
            }
            __size--;
        }

        /**
         * @brief Resize the directory's internal `entry_t` array by a factor of two.
         * @exception If the internal array can not be reallocated, an `AllocationError` is thrown.
         */
        void directory_t::_resize(void)
        {
            _resize(2);
        }

        /**
         * @brief Resize the directory's internal `entry_t` array by a given scaler.
         * @param scaler The scaler value by which to exponentially increase the capacity of the directory's internal array.
         * @exception If the capacity is greater than the maximum allowed by type, an `OverflowError` is thrown.
         * @exception If the internal array can not be reallocated, an `AllocationError` is thrown.
         */
        void directory_t::_resize(std::size_t scaler)
        {
            if (scaler < 2)
            {
                return;
            }
            else if (__capacity > (SIZE_MAX / scaler))
            {
                throw Exception("OverflowError: The capacity has overflown its type.");
            }

            std::size_t new_capacity = __capacity * scaler;

            if (new_capacity > (SIZE_MAX / sizeof(entry_t)))
            {
                throw Exception("OverflowError: The directory allocation size has overflown its type.");
            }

            entry_t *new_entries = new entry_t[new_capacity];

            for (std::size_t index = 0; index < __size; ++index)
            {
                new_entries[index] = std::move(__entries[index]);
            }

            delete[] __entries;

            __entries = new_entries;
            __capacity = new_capacity;
        }

        /**
         * @brief Deallocate the directory's internal `entry_t` array.
         */
        void directory_t::_delete(void) noexcept
        {
            delete[] __entries;

            __entries = nullptr;
            __size = 0;
            __capacity = 0;
        }

        /**
         * @brief Deallocate the directory.
         */
        directory_t::~directory_t(void) noexcept { _delete(); }
    }
}

#endif // DIRECTORY_IMPLEMENTATION