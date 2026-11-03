#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include "doc.hpp" // ! NEEDED FOR NAMESPACE DOCUMENTATION

#include "exception.hpp" // Exception

namespace polutils
{
    /**
     * @brief Exception for allocating memory.
     */
    struct AllocationError : public Exception
    {
        /**
         * @brief Construct an `AllocationError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
         * @param arguments Variadic formatting arguments.
         */
        template <typename... Arguments>
        explicit AllocationError(const std::string &format, const Arguments &...arguments);
    };

    /**
     * @brief Exception for any miscellaneous value exceptions.
     */
    struct ValueError : public Exception
    {
        /**
         * @brief Construct an `ValueError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
         * @param arguments Variadic formatting arguments.
         */
        template <typename... Arguments>
        explicit ValueError(const std::string &format, const Arguments &...arguments);
    };
    
    /**
     * @brief Exception for any indexed access.
     */
    struct IndexError : public Exception
    {
        /**
         * @brief Construct an `IndexError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
         * @param arguments Variadic formatting arguments.
         */
        template <typename... Arguments>
        explicit IndexError(const std::string &format, const Arguments &...arguments);
    };
    
    /**
     * @brief Exception for any indexed access outside of the allotted range.
     */
    struct OutOfRangeError : public IndexError
    {
        /**
         * @brief Construct an `OutOfRangeError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
         * @param arguments Variadic formatting arguments.
         */
        template <typename... Arguments>
        explicit OutOfRangeError(const std::string &format, const Arguments &...arguments);
    };
    
    /**
     * @brief Exception for any numeric underflow.
     */
    struct UnderflowError : public ValueError
    {
        /**
         * @brief Construct an `UnderflowError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
         * @param arguments Variadic formatting arguments.
         */
        template <typename... Arguments>
        explicit UnderflowError(const std::string &format, const Arguments &...arguments);
    };
    
    /**
     * @brief Exception for any numeric overflow.
     */
    struct OverflowError : public ValueError
    {
        /**
         * @brief Construct an `OverflowError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
         * @param arguments Variadic formatting arguments.
         */
        template <typename... Arguments>
        explicit OverflowError(const std::string &format, const Arguments &...arguments);
    };
    
    /**
     * @brief Exception for any numeric overflow.
     */
    struct KeyError : public ValueError
    {
        /**
         * @brief Construct an `KeyError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
         * @param arguments Variadic formatting arguments.
         */
        template <typename... Arguments>
        explicit KeyError(const std::string &format, const Arguments &...arguments);
    };

    /**
     * @brief Exception for any unreachable control flow traps.
     */
    struct UnreachableError : public Exception
    {
        /**
         * @brief Construct an `UnreachableError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
         * @param arguments Variadic formatting arguments.
         */
        template <typename... Arguments>
        explicit UnreachableError(const std::string &format, const Arguments &...arguments);
    };
    
    /**
     * @brief Exception for when a file can not be found on the filesystem.
     */
    struct FileNotFoundError : public Exception
    {
        /**
         * @brief Construct an `FileNotFoundError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
         * @param arguments Variadic formatting arguments.
         */
        template <typename... Arguments>
        explicit FileNotFoundError(const std::string &format, const Arguments &...arguments);
    };
    
    /**
     * @brief Exception for when a file already exists on the filesystem.
     */
    struct FileExistsError : public Exception
    {
        /**
         * @brief Construct an `FileExistsError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
         * @param arguments Variadic formatting arguments.
         */
        template <typename... Arguments>
        explicit FileExistsError(const std::string &format, const Arguments &...arguments);
    };
    
    /**
     * @brief Exception for any miscellaneous I/O exceptions.
     */
    struct IOError : public Exception
    {
        /**
         * @brief Construct an `IOError` with a formatted message.
         * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
         * @param arguments Variadic formatting arguments.
         */
        template <typename... Arguments>
        explicit IOError(const std::string &format, const Arguments &...arguments);
    };
}

#endif // EXCEPTIONS_HPP

#ifdef EXCEPTIONS_IMPLEMENTATION

#define EXCEPTION_IMPLEMENTATION
#include "exception.hpp" // Exception

namespace polutils
{
    /**
     * @brief Construct an `AllocationError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     * @param arguments Variadic formatting arguments.
     */
    template <typename... Arguments>
    AllocationError::AllocationError(const std::string &format, const Arguments &...arguments) : Exception(format, arguments...)
    {
        _assign_name("AllocationError");
    }

    /**
     * @brief Construct an `OutOfRangeError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     * @param arguments Variadic formatting arguments.
     */
    template <typename... Arguments>
    OutOfRangeError::OutOfRangeError(const std::string &format, const Arguments &...arguments) : IndexError(format, arguments...)
    {
        _assign_name("OutOfRangeError");
    }

    /**
     * @brief Construct an `IndexError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     * @param arguments Variadic formatting arguments.
     */
    template <typename... Arguments>
    IndexError::IndexError(const std::string &format, const Arguments &...arguments) : Exception(format, arguments...)
    {
        _assign_name("IndexError");
    }

    /**
     * @brief Construct an `OverflowError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     * @param arguments Variadic formatting arguments.
     */
    template <typename... Arguments>
    OverflowError::OverflowError(const std::string &format, const Arguments &...arguments) : Exception(format, arguments...)
    {
        _assign_name("OverflowError");
    }

    /**
     * @brief Construct an `UnderflowError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     * @param arguments Variadic formatting arguments.
     */
    template <typename... Arguments>
    UnderflowError::UnderflowError(const std::string &format, const Arguments &...arguments) : Exception(format, arguments...)
    {
        _assign_name("UnderflowError");
    }

    /**
     * @brief Construct an `ValueError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     * @param arguments Variadic formatting arguments.
     */
    template <typename... Arguments>
    ValueError::ValueError(const std::string &format, const Arguments &...arguments) : Exception(format, arguments...)
    {
        _assign_name("ValueError");
    }

    /**
     * @brief Construct an `KeyError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     * @param arguments Variadic formatting arguments.
     */
    template <typename... Arguments>
    KeyError::KeyError(const std::string &format, const Arguments &...arguments) : ValueError(format, arguments...)
    {
        _assign_name("KeyError");
    }

    /**
     * @brief Construct an `UnreachableError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     * @param arguments Variadic formatting arguments.
     */
    template <typename... Arguments>
    UnreachableError::UnreachableError(const std::string &format, const Arguments &...arguments) : Exception(format, arguments...)
    {
        _assign_name("UnreachableError");
    }

    /**
     * @brief Construct an `FileNotFoundError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     * @param arguments Variadic formatting arguments.
     */
    template <typename... Arguments>
    FileNotFoundError::FileNotFoundError(const std::string &format, const Arguments &...arguments) : Exception(format, arguments...)
    {
        _assign_name("FileNotFoundError");
    }

    /**
     * @brief Construct an `FileExistsError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     * @param arguments Variadic formatting arguments.
     */
    template <typename... Arguments>
    FileExistsError::FileExistsError(const std::string &format, const Arguments &...arguments) : Exception(format, arguments...)
    {
        _assign_name("FileExistsError");
    }

    /**
     * @brief Construct an `IOError` with a formatted message.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     * @param arguments Variadic formatting arguments.
     */
    template <typename... Arguments>
    IOError::IOError(const std::string &format, const Arguments &...arguments) : Exception(format, arguments...)
    {
        _assign_name("IOError");
    }
}

#endif // EXCEPTIONS_IMPLMENTATION