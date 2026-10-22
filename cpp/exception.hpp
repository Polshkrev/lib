#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "doc.hpp" // ! NEEDED FOR NAMESPACE DOCUMENTATION

#include <exception>
#include <string> // string
#include <ostream> // ostream

#include "printable.hpp" // printable_t

namespace polutils
{
    /**
     * @brief Base Exception class to wrap the cpp `std::exception`.
     */
    class Exception : public std::exception, public printable_t
    {
        private:

            /**
             * @brief Name (de facto python-like type) of the exception.
             */
            std::string __name;

            /**
             * @brief Message to accompany the exception.
             */
            std::string  __message;

            /**
             * @brief Full string representation of the exception. Composed of the name, a colon, and the message.
             */
            std::string __repr;

        protected:

            /**
             * @brief Assign the name – the main type of exception – to be displayed. This function is marked with the noexcept keyword.
             * @param name Name to assign.
             */
            void _assign_name(const std::string &name) noexcept;
            
            /**
             * @brief Assign the representation – a combination of the name and message separated by a colon – to be displayed. This function is marked with the noexcept keyword.
             */
            void _assign_repr(void) noexcept;

        public:

            /**
             * @brief Default constructor for an exception.
             */
            explicit Exception(void) noexcept;

            /**
             * @brief Standard constructor for an exception.
             * @param message A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
             */
            explicit Exception(const std::string &message) noexcept;

            /**
             * @brief Standard constructor for an exception.
             * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
             * @param arguments Variadic formatting arguments.
             */
            template <typename ...Arguments>
            explicit Exception(const std::string &format, const Arguments&...arguments);

            /**
             * @brief Obtain a string representation of the exception.
             * @returns A string representation of the exception.
             */
            const char *to_string(void) const noexcept override;

            /**
             * @brief Overload to interface with the parent exception class.
             * @return A const c string.
             */
            const char *what(void) const noexcept override;

            /**
             * @brief Default destructor of the exception.
             */
            virtual ~Exception(void) noexcept = default;
    };

    /**
     * @brief Operator << overload for an exception to aid in printing.
     * @param stream Output stream to which to write.
     * @param exception `Exception` from which to write.
     */
    std::ostream &operator<<(std::ostream &stream, const Exception &exception);
}

#endif // EXCEPTION_HPP_

#ifdef EXCEPTION_IMPLEMENTATION

#define BUFFER_IMPLEMENTATION
#include "../c/collections/buffer.h" // bufer_save, buffer_sprintf, buffer_rewind

namespace polutils
{
    /**
     * @brief Default constructor for an exception.
     */
    Exception::Exception(void) noexcept : Exception("An error has occurred.") {}

    /**
     * @brief Standard constructor for an exception.
     * @param message A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     */
    Exception::Exception(const std::string &message) noexcept : __name("Exception"), __message(message), __repr(__name.append(": ").append(__message)) {}

    /**
     * @brief Standard constructor for an exception.
     * @param format A message string (without newline) to be displayed when the exception is thrown. The parameter is marked with const.
     * @param arguments Variadic formatting arguments.
     */
    template <typename ...Arguments>
    Exception::Exception(const std::string &format, const Arguments &...arguments) : __name("Exception"), __message(""), __repr("")
    {
        std::size_t checkpoint = buffer_save();
        __message.assign(buffer_sprintf(format.c_str(), arguments...));
        _assign_repr();
        buffer_rewind(checkpoint);
    }

    /**
     * @brief Assign the name – the main type of exception – to be displayed. This function is marked with the noexcept keyword.
     * @param name Name to assign.
     */
    void Exception::_assign_name(const std::string &name) noexcept
    {
        __name = name;
        _assign_repr();
    }

    /**
     * @brief Assign the representation – a combination of the name and message separated by a colon – to be displayed. This function is marked with the noexcept keyword.
     */
    void Exception::_assign_repr(void) noexcept
    {
        __repr = __name + ": " + __message;
    }

    /**
     * @brief Obtain a string representation of the exception.
     * @returns A string representation of the exception.
     */
    const char *Exception::to_string(void) const noexcept
    {
        return what();
    }

    /**
     * @brief Overload to interface with the parent exception class.
     * @return A const c string.
     */
    const char *Exception::what(void) const noexcept
    {
        return (__repr).c_str();
    }

    /**
     * @brief Operator << overload for an exception to aid in printing.
     * @param stream Output stream to which to write.
     * @param exception `Exception` from which to write.
     */
    std::ostream &operator<<(std::ostream &stream, const Exception &exception)
    {
        return stream << exception.what();
    }
}

#endif // EXCEPTION_IMPLEMENTATION