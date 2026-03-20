#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#define EXCEPTION_IMPLEMENTATION
#include "exception.hpp"

namespace polutils
{
    /**
     * @brief Exception for an exception when allocating memory.
     */
    struct AllocationError : public Exception
    {
        /**
         * @brief Construct an `AllocationError` with a message.
         * @param message Message to accompany the exception.
         */
        explicit AllocationError(const std::string &message);

        /**
         * @brief Construct an `AllocationError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
         * @param arguemtns Variadic fomatting arguments.
         */
        template <typename... Arguments>
        explicit AllocationError(const std::string &format, Arguments...arguments);
    };

    /**
     * @brief Exception for any indexed access outside of the alotted range.
     */
    struct OutOfRangeError : public Exception
    {
        /**
         * @brief Construct an `OutOfRangeError` with a message.
         * @param message Message to accompany the exception.
         */
        explicit OutOfRangeError(const std::string &message);

        /**
         * @brief Construct an `OutOfRangeError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
         * @param arguemtns Variadic fomatting arguments.
         */
        template <typename... Arguments>
        explicit OutOfRangeError(const std::string &format, Arguments...arguments);
    };
    
    /**
     * @brief Exception for any indexed access.
     */
    struct IndexError : public Exception
    {
        /**
         * @brief Construct an `IndexError` with a message.
         * @param message Message to accompany the exception.
         */
        explicit IndexError(const std::string &message);

        /**
         * @brief Construct an `IndexError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
         * @param arguemtns Variadic fomatting arguments.
         */
        template <typename... Arguments>
        explicit IndexError(const std::string &format, Arguments...arguments);
    };

    /**
     * @brief Exception for any miscellaneous value exceptions.
     */
    struct ValueError : public Exception
    {
        /**
         * @brief Construct a `ValueError` with a message.
         * @param message Message to accompany the exception.
         */
        explicit ValueError(const std::string &message);

        /**
         * @brief Construct an `ValueError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
         * @param arguemtns Variadic fomatting arguments.
         */
        template <typename... Arguments>
        explicit ValueError(const std::string &format, Arguments...arguments);
    };

    /**
     * @brief Exception for any unreachable control flow traps.
     */
    struct UnreachableError : public Exception
    {
        /**
         * @brief Construct an `UnreachableError` with a message.
         * @param message Message to accompany the exception.
         */
        explicit UnreachableError(const std::string &message);

        /**
         * @brief Construct an `UnreachableError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
         * @param arguemtns Variadic fomatting arguments.
         */
        template <typename... Arguments>
        explicit UnreachableError(const std::string &format, Arguments...arguments);
    };

    /**
     * @brief Exception for when a file can not be found on the filesystem.
     */
    struct FileNotFoundError : public Exception
    {
        /**
         * @brief Construct a `FileNotFoundError` with a message.
         * @param message Message to accompany the exception.
         */
        explicit FileNotFoundError(const std::string &message);

        /**
         * @brief Construct an `FileNotFoundError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
         * @param arguemtns Variadic fomatting arguments.
         */
        template <typename... Arguments>
        explicit FileNotFoundError(const std::string &format, Arguments...arguments);
    };

    /**
     * @brief Exception for when a file already exists on the filesystem.
     */
    struct FileExistsError : public Exception
    {
        /**
         * @brief Construct a `FileExistsError` with a message.
         * @param message Message to accompany the exception.
         */
        explicit FileExistsError(const std::string &message);

        /**
         * @brief Construct an `FileExistsError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
         * @param arguemtns Variadic fomatting arguments.
         */
        template <typename... Arguments>
        explicit FileExistsError(const std::string &format, Arguments...arguments);
    };

    /**
     * @brief Exception for any miscellaneous I/O exceptions.
     */
    struct IOError : public Exception
    {
        /**
         * @brief Construct an `IOError` with a message.
         * @param message Message to accompany the exception.
         */
        explicit IOError(const std::string &message);

        /**
         * @brief Construct an `IOError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
         * @param arguemtns Variadic fomatting arguments.
         */
        template <typename... Arguments>
        explicit IOError(const std::string &format, Arguments...arguments);
    };
}

#endif // EXCEPTIONS_HPP

// #ifdef EXCEPTIONS_IMPLEMENTATION

namespace polutils
{
    /**
     * @brief Construct an `AllocationError` with a message.
     * @param message Message to accompany the exception.
     */
    AllocationError::AllocationError(const std::string &message) : Exception(message.data())
    {
        _assign_name("AllocationError");
    }

    /**
     * @brief Construct an `AllocationError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
     * @param arguemtns Variadic fomatting arguments.
     */
    template <typename... Arguments>
    AllocationError::AllocationError(const std::string &format, Arguments...arguments) : Exception(format, arguments...)
    {
        _assign_name("AllocationError");
    }

    /**
     * @brief Construct an `OutOfRangeError` with a message.
     * @param message Message to accompany the exception.
     */
    OutOfRangeError::OutOfRangeError(const std::string &message) : Exception(message.data())
    {
        _assign_name("OutOfRangeError");
    }

    /**
     * @brief Construct an `OutOfRangeError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
     * @param arguemtns Variadic fomatting arguments.
     */
    template <typename... Arguments>
    OutOfRangeError::OutOfRangeError(const std::string &format, Arguments...arguments) : Exception(format, arguments...)
    {
        _assign_name("OutOfRangeError");
    }

    /**
     * @brief Construct an `IndexError` with a message.
     * @param message Message to accompany the exception.
     */
    IndexError::IndexError(const std::string &message) : Exception(message.data())
    {
        _assign_name("IndexError");
    }

    /**
     * @brief Construct an `IndexError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
     * @param arguemtns Variadic fomatting arguments.
     */
    template <typename... Arguments>
    IndexError::IndexError(const std::string &format, Arguments...arguments) : Exception(format, arguments...)
    {
        _assign_name("IndexError");
    }

    /**
     * @brief Construct a `ValueError` with a message.
     * @param message Message to accompany the exception.
     */
    ValueError::ValueError(const std::string &message) : Exception(message.data())
    {
        _assign_name("ValueError");
    }

    /**
     * @brief Construct an `ValueError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
     * @param arguemtns Variadic fomatting arguments.
     */
    template <typename... Arguments>
    ValueError::ValueError(const std::string &format, Arguments...arguments) : Exception(format, arguments...)
    {
        _assign_name("ValueError");
    }

    /**
     * @brief Construct an `UnreachableError` with a message.
     * @param message Message to accompany the exception.
     */
    UnreachableError::UnreachableError(const std::string &message) : Exception(message.data())
    {
        _assign_name("UnreachableError");
    }

    /**
     * @brief Construct an `UnreachableError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
     * @param arguemtns Variadic fomatting arguments.
     */
    template <typename... Arguments>
    UnreachableError::UnreachableError(const std::string &format, Arguments...arguments) : Exception(format, arguments...)
    {
        _assign_name("UnreachableError");
    }

    /**
     * @brief Construct a `FileNotFoundError` with a message.
     * @param message Message to accompany the exception.
     */
    FileNotFoundError::FileNotFoundError(const std::string &message) : Exception(message.data())
    {
        _assign_name("FileNotFoundError");
    }

    /**
     * @brief Construct an `FileNotFoundError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
     * @param arguemtns Variadic fomatting arguments.
     */
    template <typename... Arguments>
    FileNotFoundError::FileNotFoundError(const std::string &format, Arguments...arguments) : Exception(format, arguments...)
    {
        _assign_name("FileNotFoundError");
    }

    /**
     * @brief Construct a `FileExistsError` with a message.
     * @param message Message to accompany the exception.
     */
    FileExistsError::FileExistsError(const std::string &message) : Exception(message.data())
    {
        _assign_name("FileExistsError");
    }

    /**
     * @brief Construct an `FileExistsError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
     * @param arguemtns Variadic fomatting arguments.
     */
    template <typename... Arguments>
    FileExistsError::FileExistsError(const std::string &format, Arguments...arguments) : Exception(format, arguments...)
    {
        _assign_name("FileExistsError");
    }

    /**
     * @brief Construct an `IOError` with a message.
     * @param message Message to accompany the exception.
     */
    IOError::IOError(const std::string &message) : Exception(message.data())
    {
        _assign_name("IOError");
    }

    /**
     * @brief Construct an `IOError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
     * @param arguemtns Variadic fomatting arguments.
     */
    template <typename... Arguments>
    IOError::IOError(const std::string &format, Arguments...arguments) : Exception(format, arguments...)
    {
        _assign_name("IOError");
    }
}

// #endif // EXCEPTIONS_IMPLMENTATION