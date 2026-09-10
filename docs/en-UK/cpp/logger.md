# Logger
A logger written in c++.
## Table Of Contents
1. [Usage](#usage)
    1. [Construction](#construction)
    2. [Logging](#logging)
    3. [Destruction](#destruction)
## Usage
Everything is defined behind the `logging` namespace.
### Construction
A logger constuctor is provided. The parametres of the constructor is the name and default logging level enum value. Just as in my [wrapper](https://github.com/Polshkrev/Utilities/tree/main/docs/en-UK/globals/log) around the python logging package, the level parametre passed to this function sets the minimum level that the logger will output.
To setup the logger correctly, you will need to either add `stdout` or a file. To do so, you will either need to call the `add_console` function or `add_file` method respectively. To use both `stdout` and a file, you need to call the `full_setup` method.
### Logging
To log a message, you need to call the `log` method. This method takes a view into a null-terminated log message without a newline, and a level with which the message will be logged.
### Destruction
A default destructor is available.