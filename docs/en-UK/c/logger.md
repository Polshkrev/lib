# Logger
A logger written in c.

## Table Of Contents
1. [Usage](#usage)
    1. [Construction](#construction)
    2. [Logging](#logging)
    3. [Destruction](#destruction)

## Usage
### Construction
To construct a new logger object, use the `logger_new` function. This function will return a heap allocated logger object. The parametres of the function is the name and default logging level enum value. Just as in my [wrapper](https://github.com/Polshkrev/Utilities/tree/main/docs/en-UK/globals/log) around the python logging package, the level parametre passed to this function sets the minimum level that the logger will output.
To setup the logger correctly, you will need to either add `stdout` or a file. To do so, you will either need to call the `logger_add_console` function or `logger_add_file` function respectively. To use both `stdout` and a file, you need to call the `logger_full_setup` function.
### Logging
To log a message, you need to call the `logger_log` function. This function takes a view into the logger object, a null-terminated log message without a newline, and a level with which the message will be logged. If you want to change the default logging level, call the `logger_set_level` function.
### Destruction
To deallocate the logger object, call the `close_logger` function.