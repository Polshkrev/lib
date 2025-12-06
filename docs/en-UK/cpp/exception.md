# Exception
A wrapper around `std::exception` that is based on [Python's exception](https://docs.python.org/3/tutorial/errors.html).
This is to be used as a base class to more easily define custom exceptions.

## Table Of Contents
1. [Usage](#usage)

## Usage
To use the base class without deriving a subclass, you can either use the default contructor or the constructor that uses a string view to a message. An `operator<<` overload is available. To derive a subclass, you will need to call the protected `_assign_name` method in the constructor of the subclass. I have chosen to pass the message of the subclass to the base class in the "class initializer list". Here is banking project as an example:
```cpp
#include <string>
#include <exception.hpp>

InsufficientFundsError::InsufficientFundsError(const std::string &message) : Exception(message.data())
{
    _assign_name("InsufficientFundsError");
}
```