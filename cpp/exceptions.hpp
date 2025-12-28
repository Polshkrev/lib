#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

// #define EXCEPTION_IMPLEMENTATION
#include "exception.hpp"

namespace polutils
{
    /*
    * @brief Exception for an exception when allocating memory.
    */
    struct AllocationError : public Exception
    {
        /*
        * @brief Construct an `AllocationError` with a message.
        * @param message Message to accompany the exception.
        */
        explicit AllocationError(const std::string &message);
    };

    /*
    * @brief Exception for any miscellaneous value exceptions.
    */
    struct ValueError : public Exception
    {
        /*
        * @brief Construct a `ValueError` with a message.
        * @param message Message to accompany the exception.
        */
        explicit ValueError(const std::string &message);
    };

    /*
    * @brief Exception for any miscellaneous value exceptions.
    */
    struct UnreachableError : public Exception
    {
        /*
        * @brief Construct a `ValueError` with a message.
        * @param message Message to accompany the exception.
        */
        explicit UnreachableError(const std::string &message);
    };

    /*
    * @brief Exception for any miscellaneous value exceptions.
    */
    struct FileNotFoundError : public Exception
    {
        /*
        * @brief Construct a `ValueError` with a message.
        * @param message Message to accompany the exception.
        */
        explicit FileNotFoundError(const std::string &message);
    };
}

#endif // EXCEPTIONS_HPP

#ifdef EXCEPTIONS_IMPLEMENTATION

#define EXCEPTION_IMPLEMENTATION
#include "exception.hpp"

namespace polutils
{
    /*
    * @brief Construct an `AllocationError` with a message.
    * @param message Message to accompany the exception.
    */
    AllocationError::AllocationError(const std::string &message) : Exception(message.data())
    {
        _assign_name("AllocationError");
    }

    /*
    * @brief Construct a `ValueError` with a message.
    * @param message Message to accompany the exception.
    */
    ValueError::ValueError(const std::string &message) : Exception(message.data())
    {
        _assign_name("ValueError");
    }

    /*
    * @brief Construct a `ValueError` with a message.
    * @param message Message to accompany the exception.
    */
    UnreachableError::UnreachableError(const std::string &message) : Exception(message.data())
    {
        _assign_name("UnreachableError");
    }

    /*
    * @brief Construct a `ValueError` with a message.
    * @param message Message to accompany the exception.
    */
    FileNotFoundError::FileNotFoundError(const std::string &message) : Exception(message.data())
    {
        _assign_name("FileNotFoundError");
    }

}

#endif // EXCEPTIONS_IMPLMENTATION