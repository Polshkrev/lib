#ifndef DOC_HPP_
#define DOC_HPP_
namespace polutils
{
    /**
     * @brief `collections` provides generic data structures and utilities for storing and manipulating collections of values.
     * The collection types are designed to provide an abstraction and standardization of how collections are used.
     *
     * Types:
     *
     * The primary types provided by the namespace include:
     *
     * - `collection_t` — Interface defining a collection.
     * 
     * - `array_t` — Contiguous collection with a compile-time non-resizable capacity.
     * 
     * - `dynamic_array_t` — Dynamically resizable array.
     * 
     * - `sized_t` - Representation of a `collection_t` with a size.
     *
     * Collection:
     *
     * `collection_t` defines the common interface implemented by the collection implementations.
     * It provides operations and properties common to collections while leaving specific behaviour to derived types.
     *
     * Array:
     *
     * `array_t` provides a contiguous collection with a compile-time non-resizable capacity.
     * The type is intended to provide the default array functionality while allowing sub classes to implement their own override behaviour.
     *
     * Dynamic Array:
     *
     * `dynamic_array_t` provides a dynamically resizable array.
     * 
     * Rationale:
     * 
     * The collection utilities are designed around reusable collection abstractions rather than individual container implementations.
     * Common collection behavior is centralized where possible, while storage management and resizing remain the responsibility of the appropriate collection type.
     */
    namespace collections {}
}
#endif // COLLECTIONS_DOC_HPP_