#ifndef LOGGER_H_
#define LOGGER_H_

#include <time.h> // time_t, struct tm, strftime
#include <locale.h> // setlocale, LC_TIME
#include <stdio.h> // FILE, size_t, fprintf, stderr, fopen, fclose
#include <stdlib.h> // malloc, exit, free
#include <string.h> // memset
#include <stdbool.h> // bool

#ifndef LOCALE
#define LOCALE "en_US.UTF-8"
#endif // LOCALE

#define FORMAT_BUFFER_SIZE 200

#define AVAILABLE_OUTPUTS 2

// #ifndef FILE_BUFFER_SIZE
// #define FILE_BUFFER_SIZE 200
// #endif // FILE_BUFFER

#ifndef TIMESTAMP_FORMAT
#define TIMESTAMP_FORMAT "%Y-%m-%d %X"
#endif // TIMESTAMP_FORMAT

/*
* @brief Severity of a logging message.
*/
typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
} LoggingLevel;

/*
* @brief A logger.
*/
typedef struct
{
    const char *name;
    LoggingLevel level;
    FILE *outputs[AVAILABLE_OUTPUTS];
} Logger;

/*
* @brief Represent a LoggingLevel enum as a string.
* To handle the impossible case of providing an unsupported enum, a message is printed to stderr.
* @param level LoggingLevel enum to return as a string.
* @returns String representation of LoggingLevel enum.
*/
const char *lltostr(LoggingLevel level);

/*
* @brief Set the minimum allowable LoggingLevel of the logger. This is inforced in the `logger_log` function.
* @param logger A pointer to the logger whose level will be set.
* @param level LoggingLevel to set for the provided logger.
*/
void logger_set_level(Logger *logger, LoggingLevel level);

/*
* @brief Create a new logger given a name and a LoggingLevel. This is the API entry point.
* @param name Name of the logger.
* @param level LoggingLevel that indicates the minimal logging level that will be logged.
* @returns A new logger.
*/
Logger *logger_new(const char *name, LoggingLevel level);

/*
* @brief Add `stdout` to the list of ouputs of the logger. If there's too many outputs that have been "allocated" to the struct, an error will be printed to `stderr` and the programme will exit with and exit code of 1.
* @param logger A pointer to the Logger whose list of outputs will be updated.
* @exception If the number of outputs that have been "allocated" to the logger has exceeded the number of constant available outputs, an `AllocationError` will be printed to stderr.
*/
void logger_add_console(Logger *logger);

/*
* @brief Add a file to the list of outputs of the logger. The file is opened in append mode and is not closed.
* @param logger A pointer to the Logger whose list of outputs will be updated.
* @param filename A string – marked with const – that will be passed to `fopen`.
* @exception If the file doesn't exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit with an exit code of 1.
* @exception If the number of outputs that have been "allocated" to the logger has exceeded the number of constant available outputs, an `AllocationError` will be printed to `stderr` and the file will be closed.
*/
void logger_add_file(Logger *logger, const char *filename);

/*
* @brief Setup the logger with both `stdout` and a file with a given filename.
* @param logger A pointer to a Logger.
* @param filename A string – marked with const – that will be passed to `logger_add_file` that was previously defined.
* @exception As defined in the `logger_add...` functions, if the file doesn't exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit with an exit code of 1.
* @exception As defined in the `logger_add...` functions, if the number of outputs that have been "allocated" to the logger has exceeded the number of constant available outputs, an `AllocationError` will be printed to `stderr` and the file will be closed.
*/
void logger_full_setup(Logger *logger, const char *filename);

/*
* @brief A prerequisite setup function that setups the logger with only `stdout`.
* @param logger A pointer to a Logger.
*/
void logger_console_only(Logger *logger);

/*
* @brief A prerequisite setup function that setups the logger with only a file.
* @param logger A pointer to the Logger as a side effect of an OOP style of thinking and programming.
* @param filename A string – marked with const – that will be passed to `logger_add_file` that was previously defined.
*/
void logger_file_only(Logger *logger, const char *filename);

/*
* @brief Log a message.
* @param logger A logger object to use.
* @param message A string – marked with const – to publish to each of the elements in the array of outputs added to the logger.
* @param level The level of the message.
*/
void logger_log(const Logger *logger, const char *message, LoggingLevel level);

/*
* @brief Close any file outputs linked to the logger.
* @param logger A logger object to close.
*/
void close_logger(Logger *logger);

#endif // LOGGER_H_

#ifdef LOGGER_IMPLEMENTATION

char timestamp[FORMAT_BUFFER_SIZE];
// char file_buffer[FILE_BUFFER_SIZE] = {0};
size_t output_count = 0;

/*
* @brief Determine wheather a specific output stream is a file.
* @param stream Output file stream to check against i/o output.
* @returns A boolean of wheather a given file stream is composed of an i/o stream (e.g) stdout, stdin, etc. 
*/
bool is_file(const FILE *stream)
{
    bool found = false;
    FILE *streams[3] = {stdout, stdin, stderr};
    for (int output = 0; output < 3; ++output)
    {
        if (streams[output] != stream)
        {
            continue;
        }
        found = true;
    }
    return !found;
}

/*
* @brief Set the locale of the timezone information.
* @param locale Country code of target locale.
*/
void _set_locale(const char *locale)
{
    setlocale(LC_TIME, locale);
}

/*
* @brief Set a timestamp to be used in the logging format.
*/
void set_timestamp()
{
    time_t t = time(NULL);
    struct tm date = *localtime(&t);
    _set_locale(LOCALE);
    strftime(timestamp, FORMAT_BUFFER_SIZE, TIMESTAMP_FORMAT, &date);
}

/*
* @brief Represent a LoggingLevel enum as a string.
* To handle the impossible case of providing an unsupported enum, a message is printed to stderr.
* @param level LoggingLevel enum to return as a string.
* @returns String representation of LoggingLevel enum.
*/
const char *lltostr(LoggingLevel level)
{
    switch (level)
    {
        case LOG_DEBUG:
            return "DEBUG";
            break;
        case LOG_INFO:
            return "INFO";
            break;
        case LOG_WARNING:
            return "WARNING";
            break;
        case LOG_ERROR:
            return "ERROR";
            break;
        case LOG_CRITICAL:
            return "CRITICAL";
            break;
    }
    fprintf(stderr, "%s\n", "Unknown Logging Level");
    exit(1);
}

/*
* @brief Set the minimum allowable LoggingLevel of the logger. This is inforced in the `logger_log` function.
* @param logger A pointer to the logger whose level will be set.
* @param level LoggingLevel to set for the provided logger. If the provided level is less than or equal to the level already set in the logger, the function returns without modifying the logger.
*/
void logger_set_level(Logger *logger, LoggingLevel level)
{
    if (logger->level <= level)
    {
        return;
    }
    logger->level = level;
}

/*
* @brief Create a new logger given a name and a LoggingLevel. This is the API entry point.
* @param name Name of the logger.
* @param level LoggingLevel that indicates the minimal logging level that will be logged.
* @returns A new logger.
*/
Logger *logger_new(const char *name, LoggingLevel level)
{
    Logger *logger = (Logger *)malloc(sizeof(Logger));
    if (NULL == logger)
    {
        fprintf(stderr, "AllocationError: Can not allocate enough memory to initialize logger.\n");
        exit(1);
    }
    memset(logger, 0, sizeof(Logger));
    logger->name = name;
    logger_set_level(logger, level);
    return logger;
}

/*
* @brief Add `stdout` to the list of ouputs of the logger. If there's too many outputs that have been "allocated" to the struct, an error will be printed to `stderr` and the programme will exit with and exit code of 1.
* @param logger A pointer to the Logger whose list of outputs will be updated.
* @exception If the number of outputs that have been "allocated" to the logger has exceeded the number of constant available outputs, an `AllocationError` will be printed to stderr.
*/
void logger_add_console(Logger *logger)
{
    if (output_count >= AVAILABLE_OUTPUTS)
    {
        fprintf(stderr, "AllocationError: %s\n", "Too many outputs assigned.");
        exit(1);
    }
    logger->outputs[output_count] = stdout;
    output_count++;
}

/*
* @brief Add a file to the list of outputs of the logger. The file is opened in append mode and is not closed.
* @param logger A pointer to the Logger whose list of outputs will be updated.
* @param filename A string – marked with const – that will be passed to `fopen`.
* @exception If the file doesn't exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit with an exit code of 1.
* @exception If the number of outputs that have been "allocated" to the logger has exceeded the number of constant available outputs, an `AllocationError` will be printed to `stderr` and the file will be closed.
*/
void logger_add_file(Logger *logger, const char *filename)
{
    FILE *file = fopen(filename, "a");
    if (NULL == file)
    {
        fprintf(stderr, "FileNotFoundError: %s\n", "Unable to open the file.");
        exit(1);
    }
    if (output_count >= AVAILABLE_OUTPUTS)
    {
        fprintf(stderr, "AllocationError: %s\n", "Too many outputs assigned.");
        fclose(file);
        exit(1);
    }
    logger->outputs[output_count] = file;
    output_count++;
    // fclose(file);
}

/*
* @brief Setup the logger with both `stdout` and a file with a given filename.
* @param logger A pointer to a Logger.
* @param filename A string – marked with const – that will be passed to `logger_add_file` that was previously defined.
* @exception As defined in the `logger_add...` functions, if the file doesn't exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit with an exit code of 1.
* @exception As defined in the `logger_add...` functions, if the number of outputs that have been "allocated" to the logger has exceeded the number of constant available outputs, an `AllocationError` will be printed to `stderr` and the file will be closed.
*/
void logger_full_setup(Logger *logger, const char *filename)
{
    logger_add_console(logger);
    logger_add_file(logger, filename);
}

/*
* @brief A prerequisite setup function that setups the logger with only `stdout`.
* @param logger A pointer to a Logger.
*/
void logger_console_only(Logger *logger)
{
    logger_add_console(logger);
    output_count = 2;
}

/*
* @brief A prerequisite setup function that setups the logger with only a file.
* @param logger A pointer to the Logger as a side effect of an OOP style of thinking and programming.
* @param filename A string – marked with const – that will be passed to `logger_add_file` that was previously defined.
*/
void logger_file_only(Logger *logger, const char *filename)
{
    logger_add_file(logger, filename);
    output_count = 2;
}

/*
* @brief Helper function to publish a message to each of the outputs defined in the logger output array.
* @param logger A logger object from which to read.
* @param message A string – marked with const – to publish to each of the elements in the array of outputs added to the logger.
* @param level The level of the message.
*/
static void _publish_message(const Logger *logger, const char *message, LoggingLevel level)
{
    for (size_t output_num = 0; output_num < output_count; ++output_num)
    {
        fprintf(logger->outputs[output_num], "%s:%s[%s] - %s\n", timestamp, logger->name, lltostr(level), message);
    }
}

/*
* @brief Deallocate a given logger object.
* @param logger Object to deallocate.
*/
static void _logger_delete(Logger *logger)
{
    if (!logger)
    {
        return;
    }
    free(logger);
    logger = NULL;
}

/*
* @brief Close any file outputs linked to the logger.
* @param logger A logger object to close.
*/
void close_logger(Logger *logger)
{
    for (size_t output_num = 0; output_num < output_count; ++output_num)
    {
        FILE *current_output = logger->outputs[output_num];
        if (!is_file(current_output))
        {
            continue;
        }
        else if (NULL == current_output)
        {
            continue;
        }
        else if (!current_output)
        {
            continue;
        }
        fclose(current_output);
    }
    _logger_delete(logger);
}

/*
* @brief Log a message.
* @param logger A logger object to use.
* @param message A string – marked with const – to publish to each of the elements in the array of outputs added to the logger.
* @param level The level of the message.
*/
void logger_log(const Logger *logger, const char *message, LoggingLevel level)
{
    if (level < logger->level)
    {
        return;
    }
    set_timestamp();
    _publish_message(logger, message, level);
}

#endif // LOGGER_IMPLEMENTATION