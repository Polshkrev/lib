#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <string>
#include <ostream>

/*Base Exception class to wrap the cpp `std::exception`.*/
class Exception : public std::exception
{
    private:
        /* @brief Name (de facto python-like type) of the exception.*/
        std::string __name;
        /* @brief Full string representation of the exception. Composed of the name, a colon, and the message.*/
        std::string __repr;
        /* @brief Message to occompany the exception.*/
        std::string  __message;
    protected:
        /*
        * @brief Assign the name – the main type of exception – to be displayed. This function is marked with the noexcept keyword.
        * @param name name to assign.
        */
        void _assign_name(std::string name) noexcept;
        /*
        * @brief Assign the representation – a combination of the name and message seperated by a colon – to be displayed. This function is marked with the noexcept keyword.
        */
        void _assign_repr() noexcept;
    public:
        /*
        * @brief Standard constructor for an exception.
        * @param message A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
        */
        Exception(const std::string message);
        /*
        * @brief Operator << overlaod for an exception to aid in printing.
        */
        const char *what();
};

/*
* @brief Operator << overlaod for an exception to aid in printing.
*/
void operator<<(std::ostream &stream, Exception &exception);

#endif // EXCEPTION_HPP_

#ifdef EXCEPTION_IMPLEMENTATION

/*
* @brief Standard constructor for an exception.
* @param message A message string (without newline) to be displayed when the exception is thrown. The parametre is marked with const.
*/
Exception::Exception(const std::string message)
{
    Exception::_assign_name("Exception");
    Exception::__message = message;
    Exception::_assign_repr();
}

/*
* @brief Assign the name – the main type of exception – to be displayed. This function is marked with the noexcept keyword.
* @param name name to assign.
*/
void Exception::_assign_name(std::string name) noexcept
{
    Exception::__name = name;
}

/*
* @brief Assign the representation – a combination of the name and message seperated by a colon – to be displayed. This function is marked with the noexcept keyword.
*/
void Exception::_assign_repr() noexcept
{
    Exception::__repr = Exception::__name + ": " + Exception::__message;
}

/*
* @brief Overload to interface with the parent exception class.
* @return a const c string.
*/
const char *Exception::what()
{
    return (Exception::__repr).c_str();
}

/*
* @brief Operator << overlaod for an exception to aid in printing.
*/
void operator<<(std::ostream &stream, Exception &exception)
{
    stream << exception.what() << "\n";
}

#endif // EXCEPTION_IMPLEMENTATION