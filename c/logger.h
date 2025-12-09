#ifndef LOGGER_H
#define LOGGER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h> // FILE

#define AVAILABLE_OUTPUTS 2

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
} logger_t;

/*
* @brief Represent a logging level as a string.
* @param level LoggingLevel to represent as a string.
* @returns If the given logging level is not available, the function returns NULL; else a string representation of the given level is returned.
*/
const char *lltostr(LoggingLevel level);

/*
* @brief Construct a new logger given a name and a logging level.
* @param name Name to give the logger.
* @param level Minimum logging level that will be logged.
* @returns A new logger.
* @exception If the logger can not be allocated, an `AllocationError` will be printed to `stderr` and the programme will exit.
*/
logger_t *logger_new(const char *name, LoggingLevel level);

/*
* @brief Set the minimum allowable logging level.
* @param logger The logger whose level to update.
* @param level The minimum allowable logging level to set.
*/
void logger_set_level(logger_t *logger, LoggingLevel level);

/*
* @brief Add `stdout` to the logger.
* @param logger The logger to which to update.
* @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
*/
void logger_add_console(logger_t *logger);

/*
* @brief Add a file to the logger. The file is opened in append mode and is not closed.
* @param logger Logger to which to add the file.
* @param filename Name of the file to add to the logger.
* @exception If the the file does not exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit.
* @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
*/
void logger_add_file(logger_t *logger, const char *filename);

/*
* @brief Add both `stdout` and a file to the logger.
* @param logger The logger to which to update.
* @param filename Name of the file to add to the logger.
* @exception If the the file does not exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit.
* @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
*/
void logger_full_setup(logger_t *logger, const char *filename);

/*
* @brief Log a message.
* @param logger Logger to dispatch the message.
* @param message Message to log.
* @param level The level of the message. If the given level is less than the minimum the logger has allowed, the message will not be logged.
*/
void logger_log(const logger_t *logger, const char *message, LoggingLevel level);

/*
* @brief Close any file opened from the logger. If no file has been added to the logger, there is no need to call this function; although this is internally checked.
* @param logger Logger from which to close files.
*/
void logger_close(logger_t *logger);

/*
* @brief Deallocate the logger.
* @param logger Logger to deallocate.
*/
void logger_delete(logger_t *logger);

#if defined(__cplusplus)
}
#endif

#endif // LOGGER_H

#ifdef LOGGER_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif


// #include <stdio.h> // FILE, size_t, fprintf, stderr, fopen, fclose
#include <stdlib.h> // malloc, exit, free, NULL
#include <time.h> // time_t, struct tm, strftime
#include <locale.h> // setlocale, LC_TIME
#include <stdbool.h> // bool

#ifndef LOCALE
#define LOCALE "en_US.UTF-8"
#endif // LOCALE

#define FORMAT_BUFFER_SIZE 200

#ifndef TIMESTAMP_FORMAT
#define TIMESTAMP_FORMAT "%Y-%m-%d %X"
#endif // TIMESTAMP_FORMAT

static char timestamp[FORMAT_BUFFER_SIZE] = {0};

static size_t output_count = 0;

/*
* @brief Represent a logging level as a string.
* @param level LoggingLevel to represent as a string.
* @returns If the given logging level is not available, the function returns NULL; else a string representation of the given level is returned.
*/
const char *lltostr(LoggingLevel level)
{
    switch (level)
    {
        case LOG_DEBUG:
        {
            return "DEBUG";
        } break;
        case LOG_INFO:
        {
            return "INFO";
        } break;
        case LOG_WARNING:
        {
            return "WARNING";
        } break;
        case LOG_ERROR:
        {
            return "ERROR";
        } break;
        case LOG_CRITICAL:
        {
            return "CRITICAL";
        } break;
    }
    return NULL;
}

/*
* @brief Construct a new logger given a name and a logging level.
* @param name Name to give the logger.
* @param level Minimum logging level that will be logged.
* @returns A new logger.
* @exception If the logger can not be allocated, an `AllocationError` will be printed to `stderr` and the programme will exit.
*/
logger_t *logger_new(const char *name, LoggingLevel level)
{
    logger_t *logger = malloc(sizeof(logger));
    if (NULL == logger)
    {
        fprintf(stderr, "AllocationError: Can not allocate enough memory for a new logger.\n");
        exit(1);
    }
    logger->name = name;
    logger_set_level(logger, level);
    return logger;
}

/*
* @brief Set the minimum allowable logging level.
* @param logger The logger whose level to update.
* @param level The minimum allowable logging level to set.
*/
void logger_set_level(logger_t *logger, LoggingLevel level)
{
    logger->level = level;
}

/*
* @brief Add `stdout` to the logger.
* @param logger The logger to which to update.
* @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
*/
void logger_add_console(logger_t *logger)
{
    if (output_count >= AVAILABLE_OUTPUTS)
    {
        fprintf(stderr, "ValueError: The number of allocated outputs has exceded the maximum allowed.\n");
        exit(1);
    }
    logger->outputs[output_count++] = stdout;
}

/*
* @brief Add a file to the logger. The file is opened in append mode and is not closed.
* @param logger Logger to which to add the file.
* @param filename Name of the file to add to the logger.
* @exception If the the file does not exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit.
* @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
*/
void logger_add_file(logger_t *logger, const char *filename)
{
    FILE *file = fopen(filename, "a");
    if (NULL == file)
    {
        fprintf(stderr, "FileNotFoundError: Unable to open file.");
        logger_delete(logger);
        exit(1);
    }
    else if (output_count >= AVAILABLE_OUTPUTS)
    {
        fprintf(stderr, "ValueError: The number of allocated outputs has exceded the maximum allowed.\n");
        fclose(file);
        logger_delete(logger);
        exit(1);
    }
    logger->outputs[output_count++] = file;
}

/*
* @brief Add both `stdout` and a file to the logger.
* @param logger The logger to which to update.
* @param filename Name of the file to add to the logger.
* @exception If the the file does not exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit.
* @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
*/
void logger_full_setup(logger_t *logger, const char *filename)
{
    logger_add_console(logger);
    logger_add_file(logger, filename);
}

/*
* @brief Set the locale of the timezone information.
* @param locale Country code of the target locale.
*/
static void _set_locale(const char *locale)
{
    setlocale(LC_TIME, locale);
}

/*
* @brief Set a timestamp to be used in the logging format.
*/
static void set_timestamp(void)
{
    time_t t = time(NULL);
    struct tm date = *localtime(&t);
    _set_locale(LOCALE);
    strftime(timestamp, FORMAT_BUFFER_SIZE, TIMESTAMP_FORMAT, &date);
}

/*
* @brief Publish a message to each of the outputs defined in a logger output array.
* @param logger Logger from which to read.
* @param message Message to log.
* @param level The level of the message. If the given level is less than the minimum the logger has allowed, the message will not be logged.
*/
static void publish_message(const logger_t *logger, const char *message, LoggingLevel level)
{
    for (size_t output_num = 0; output_num < output_count; ++output_num)
    {
        fprintf(logger->outputs[output_num], "%s:%s[%s] - %s\n", timestamp, logger->name, lltostr(level), message);
    }
}

/*
* @brief Log a message.
* @param logger Logger to dispatch the message.
* @param message Message to log.
* @param level The level of the message. If the given level is less than the minimum the logger has allowed, the message will not be logged.
*/
void logger_log(const logger_t *logger, const char *message, LoggingLevel level)
{
    if (level < logger->level)
    {
        return;
    }
    set_timestamp();
    publish_message(logger, message, level);
}

/*
* @brief Determine wheather a specific output stream is a file.
* @param stream Output file stream to check against i/o output.
* @returns A boolean of whether a given file is composed of an i/o stream (e.g.) stdout, stdin, and stderr.
*/
static bool is_file(const FILE *stream)
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
* @brief Close any file opened from the logger. If no file has been added to the logger, there is no need to call this function; although this is internally checked.
* @param logger Logger from which to close files.
*/
void logger_close(logger_t *logger)
{
    for (size_t output_num = 0; output_num > output_count; ++output_num)
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
}

/*
* @brief Deallocate the logger.
* @param logger Logger to deallocate.
*/
void logger_delete(logger_t *logger)
{
    if (!logger)
    {
        return;
    }
    logger_close(logger);
    free(logger);
    logger = NULL;
}

#endif // LOGGER_IMPLEMENTATION