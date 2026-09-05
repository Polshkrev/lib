#ifndef LOGGER_H
#define LOGGER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h> // bool
#include <stdio.h> // FILE, size_t, stderr, fprintf, fopen, fclose

#define AVAILABLE_OUTPUTS 2

/**
 * @brief Severity of a logging message.
 */
typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL,
    __total_levels
} LoggingLevel;

/**
 * @brief A logger.
 */
typedef struct
{
    const char *name;
    LoggingLevel level;
    FILE *outputs[AVAILABLE_OUTPUTS];
    size_t output_count;
} logger_t;

/**
 * @brief Represent a logging level as a string.
 * @param level LoggingLevel to represent as a string.
 * @returns If the given logging level is not valid, the function returns `NULL`; else a string representation of the given level is returned.
 */
const char *lltostr(LoggingLevel level);

/**
 * @brief Construct a new logger given a name and a logging level.
 * @param name Name to give the logger.
 * @param level Minimum logging level that will be logged.
 * @returns A new logger.
 * @returns If the logger can not be allocated, `NULL` is returned.
 */
logger_t *logger_init(const char *name, LoggingLevel level);

/**
 * @brief Set the minimum allowable logging level.
 * @param logger The logger whose level to update.
 * @param level The minimum allowable logging level to set.
 */
void logger_set_level(logger_t *logger, LoggingLevel level);

/**
 * @brief Add `stdout` to the logger.
 * @param logger The logger to which to update.
 * @returns True if `stdout` can be added to the logger, else false.
 */
bool logger_add_console(logger_t *logger);

/**
 * @brief Add a file to the logger. The file is opened in append mode and is not closed.
 * @param logger Logger to which to add the file.
 * @param filename Name of the file to add to the logger.
 * @returns True if a file can be added to the logger, else false.
 * @exception If the the file does not exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit.
 */
bool logger_add_file(logger_t *logger, const char *filename);

/**
 * @brief Add both `stdout` and a file to the logger.
 * @param logger The logger to which to update.
 * @param filename Name of the file to add to the logger.
 * @exception If the the file does not exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit.
 */
bool logger_full_setup(logger_t *logger, const char *filename);

/**
 * @brief Log a message.
 * @param logger Logger to dispatch the message.
 * @param message Message to log.
 * @param level The level of the message. If the given level is less than the minimum the logger has allowed, the message will not be logged.
 */
void logger_log(const logger_t *logger, const char *message, LoggingLevel level);

/**
 * @brief Close any file opened from the logger. If no file has been added to the logger, there is no need to call this function; although this is internally checked.
 * @param logger Logger from which to close files.
 */
void logger_close(logger_t *logger);

/**
 * @brief Deallocate the logger.
 * @param logger Logger to deallocate.
 */
void logger_delete(logger_t **logger);

#if defined(__cplusplus)
}
#endif

#endif // LOGGER_H

#ifdef LOGGER_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdlib.h> // malloc, exit, free, NULL
#include <time.h> // time_t, struct tm, strftime
#include <locale.h> // setlocale, LC_TIME

#ifndef LOCALE
#define LOCALE "en_US.UTF-8"
#endif // LOCALE

/**
 * @brief Default capacity of the format buffer.
 */
#define FORMAT_BUFFER_SIZE 200

/**
 * @brief Format of the __timestamp.
 */
#ifndef TIMESTAMP_FORMAT
#define TIMESTAMP_FORMAT "%Y-%m-%d %X"
#endif // TIMESTAMP_FORMAT

/**
 * @brief Global output array.
 */
static char __timestamp[FORMAT_BUFFER_SIZE] = {0};

/**
 * @brief Static mapping of logger levels to names.
 */
static const char *const __logging_level_names[__total_levels] =
{
    [LOG_DEBUG] = "DEBUG",
    [LOG_INFO] = "INFO",
    [LOG_WARNING] = "WARNING",
    [LOG_ERROR] = "ERROR",
    [LOG_CRITICAL] = "CRITICAL"
};

/**
 * @brief Determine if the given logging level is valid.
 * @param level Given level to evaluate.
 * @returns True if the given level is evaluated to be valid, else false.
 */
static bool __is_valid_level(LoggingLevel level)
{
    return level >= LOG_DEBUG && level <= LOG_CRITICAL;
}

/**
 * @brief Represent a logging level as a string.
 * @param level LoggingLevel to represent as a string.
 * @returns If the given logging level is not valid, the function returns `NULL`; else a string representation of the given level is returned.
 */
const char *lltostr(LoggingLevel level)
{
    if (!__is_valid_level(level)) return "NONE";
    return __logging_level_names[level];
}

/**
 * @brief Construct a new logger given a name and a logging level.
 * @param name Name to give the logger.
 * @param level Minimum logging level that will be logged.
 * @returns A new logger.
 * @returns If the logger can not be allocated, `NULL` is returned.
 */
logger_t *logger_init(const char *name, LoggingLevel level)
{
    logger_t *logger = (logger_t *)malloc(sizeof(logger_t));
    if (NULL == logger) return NULL;
    logger->name = name;
    logger_set_level(logger, level);
    logger->output_count = 0;
    _set_locale(LOCALE);
    return logger;
}

/**
 * @brief Set the minimum allowable logging level.
 * @param logger The logger whose level to update.
 * @param level The minimum allowable logging level to set.
 */
void logger_set_level(logger_t *logger, LoggingLevel level)
{
    logger->level = level;
}

/**
 * @brief Add `stdout` to the logger.
 * @param logger The logger to which to update.
 * @returns True if `stdout` can be added to the logger, else false.
 * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
 */
bool logger_add_console(logger_t *logger)
{
    if (logger->output_count >= AVAILABLE_OUTPUTS) return false;
    logger->outputs[logger->output_count++] = stdout;
    return true;
}

/**
 * @brief Add a file to the logger. The file is opened in append mode and is not closed.
 * @param logger Logger to which to add the file.
 * @param filename Name of the file to add to the logger.
 * @returns True if a file can be added to the logger, else false.
 * @exception If the the file does not exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit.
 */
bool logger_add_file(logger_t *logger, const char *filename)
{
    if (logger->output_count >= AVAILABLE_OUTPUTS) return false;
    FILE *file = fopen(filename, "a");
    if (NULL == file)
    {
        fprintf(stderr, "FileNotFoundError: Unable to open file.");
        logger_delete(&logger);
        exit(1);
    }
    logger->outputs[logger->output_count++] = file;
    return true;
}

/**
 * @brief Add both `stdout` and a file to the logger.
 * @param logger The logger to which to update.
 * @param filename Name of the file to add to the logger.
 * @exception If the the file does not exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit.
 * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
 */
bool logger_full_setup(logger_t *logger, const char *filename)
{
    if (!logger_add_console(logger)) return false;
    return logger_add_file(logger, filename);
}

/**
 * @brief Set the locale of the timezone information.
 * @param locale Country code of the target locale.
 */
static void _set_locale(const char *locale)
{
    setlocale(LC_TIME, locale);
}

/**
 * @brief Set a timestamp to be used in the logging format.
 * @returns True if the timestamp can be set, else false.
 */
static bool __set_timestamp(void)
{
    struct tm date;
    time_t current_time = time(NULL);
    if (current_time == (time_t)-1) return false;
#ifdef _WIN32
    else if (localtime_s(&date, &current_time) != 0) return false;
#else
    else if (localtime_r(&current_time, &date) == NULL) return false;
#endif
    return strftime(__timestamp, FORMAT_BUFFER_SIZE, TIMESTAMP_FORMAT, &date) != 0;
}

/**
 * @brief Publish a message to each of the outputs defined in a logger output array.
 * @param logger Logger from which to read.
 * @param message Message to log.
 * @param level The level of the message. If the given level is less than the minimum the logger has allowed, the message will not be logged.
 */
static void __publish_message(const logger_t *logger, const char *message, LoggingLevel level)
{
    const char *level_string = lltostr(level);
    for (size_t output_num = 0; output_num < logger->output_count; ++output_num)
    {
        if (!__set_timestamp())
        {
            fprintf(logger->outputs[output_num], "%s[%s] - %s\n", logger->name, level_string, message);
            continue;
        }
        fprintf(logger->outputs[output_num], "%s:%s[%s] - %s\n", __timestamp, logger->name, level_string, message);
    }
}

/**
 * @brief Log a message.
 * @param logger Logger to dispatch the message.
 * @param message Message to log.
 * @param level The level of the message. If the given level is less than the minimum the logger has allowed, the message will not be logged.
 */
void logger_log(const logger_t *logger, const char *message, LoggingLevel level)
{
    if (level < logger->level) return;
    __publish_message(logger, message, level);
}

/**
 * @brief Determine wheather a specific output stream is a file.
 * @param stream Output file stream to check against i/o output.
 * @returns A boolean of whether a given file is composed of an i/o stream (e.g.) `stdout`, `stdin`, and `stderr`.
 */
static bool is_file(const FILE *stream)
{
    bool found = false;
    FILE *streams[3] = {stdout, stdin, stderr};
    for (int output = 0; output < 3; ++output)
    {
        if (streams[output] != stream) continue;
        found = true;
    }
    return !found;
}

/**
 * @brief Close any file opened from the logger. If no file has been added to the logger, there is no need to call this function; although this is internally checked.
 * @param logger Logger from which to close files.
 */
void logger_close(logger_t *logger)
{
    for (size_t output_num = 0; output_num < logger->output_count; ++output_num)
    {
        FILE *current_output = logger->outputs[output_num];
        if (!current_output) continue;
        else if (!is_file(current_output)) continue;
        fclose(current_output);
    }
}

/**
 * @brief Deallocate the logger.
 * @param logger Logger to deallocate.
 */
void logger_delete(logger_t **logger)
{
    if (!(*logger)) return;
    logger_close(*logger);
    free(*logger);
    *logger = NULL;
}

#endif // LOGGER_IMPLEMENTATION