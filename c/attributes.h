#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_

#include <stdio.h> // fprintf, stderr
#include <stdlib.h> // abort

#if defined(__GNUC__) || defined(__clang__)
    /**
     * @brief Define a given attribute.
     * @param attribute Attribute to define.
     */
    #define ATTRIBUTE(attribute) __attribute__((attribute))
#else
    /**
     * @brief Define a given attribute.
     * @param attribute Attribute to define.
     */
    #define ATTRIBUTE(attribute)
#endif

#ifndef DEPRECATED
    #if defined(__GNUC__) || defined(__clang__)
        /**
         * @brief Mark a symbol as deprecated.
         * @param message Message to display.
         */
        #define DEPRECATED(message) ATTRIBUTE(deprecated((message)))
    #elif defined(_MSC_VER)
        /**
         * @brief Mark a symbol as deprecated.
         * @param message Message to display.
         */
        #define DEPRECATED(message) __declspec(deprecated(message))
    #else
        /**
         * @brief Mark a symbol as deprecated.
         * @param message Message to display.
         */
        #define DEPRECATED(...)
    #endif
#endif // DEPRECATED

/**
 * @brief Mark a symbol as unused.
 * @param value Value to mark as unused.
 */
#define UNUSED(value) (void)(value)

/**
 * @brief Mark a symbol as todo.
 * @param message Message to display.
 */
#define TODO(message)\
do\
{\
    fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, message);\
    abort();\
} while(0)

/**
 * @brief Mark a function/method as todo.
 */
#define TODO_FUNCTION TODO(__func__)

/**
 * @brief Mark a control flow branch as unreachable.
 * @param message Message to display.
 */
#define UNREACHABLE(message)\
do\
{\
    fprintf(stderr, "%s:%d: UNREACHABLE: %s\n", __FILE__, __LINE__, message);\
    abort();\
} while(0)

#if defined(__cplusplus)
    #if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard)
        /**
         * @brief Mark a return value as one not to be discarded.
         */
        #define NODISCARD [[nodiscard]]
    #endif
#elif defined(__GNUC__) || defined(__clang__)
    /**
     * @brief Mark a return value as one not to be discarded.
     */
    #define NODISCARD ATTRIBUTE(warn_unused_result)
#elif defined(_MSC_VER)
    #include <sal.h> // _Check_return_
    /**
     * @brief Mark a return value as one not to be discarded.
     */
    #define NODISCARD _Check_return_
#else
    /**
     * @brief Mark a return value as one not to be discarded.
     */
    #define NODISCARD
#endif // __cplusplus


#if defined(__GNUC__) || defined(__clang__)
    /**
     * @brief Mark a type as being packed.
     */
    #define PACKED ATTRIBUTE(packed)
    /**
     * @brief Begin the packed pragma for `MSVC`.
     * @note This only works on `MSVC`.
     */
    #define PACKED_BEGIN
    /**
     * @brief End the packed pragma for `MSVC`.
     * @note This only works on `MSVC`.
     */
    #define PACKED_END
#elif defined(_MSC_VER)
    #define PACKED
    /**
     * @brief Begin the packed pragma for `MSVC`.
     * @note This only works on `MSVC`.
     */
    #define PACKED_BEGIN __pragma(pack(push, 1))
    /**
     * @brief End the packed pragma for `MSVC`.
     * @note This only works on `MSVC`.
     */
    #define PACKED_END __pragma(pack(pop))
#else
    #define PACKED
    /**
     * @brief Begin the packed pragma for `MSVC`.
     * @note This only works on `MSVC`.
     */
    #define PACKED_BEGIN
    /**
     * @brief End the packed pragma for `MSVC`.
     * @note This only works on `MSVC`.
     */
    #define PACKED_END
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    #include <stdnoreturn.h> // noreturn
    /**
     * @brief Mark a symbol as not returning.
     */
    #define NORETURN [[noreturn]]
#elif defined(__GNUC__) || defined(__clang__)
    /**
     * @brief Mark a symbol as not returning.
     */
    #define NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
    /**
     * @brief Mark a symbol as not returning.
     */
    #define NORETURN __declspec(noreturn)
#else
    #define NORETURN
#endif

#endif // ATTRIBUTES_H_
