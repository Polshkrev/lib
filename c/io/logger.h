#ifndef LOGGER_H_
#define LOGGER_H_

#include <time.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef LOCALE
#define LOCALE "en_US.UTF-8"
#endif // LOCALE

#define FORMAT_BUFFER_SIZE 200
char timestamp[FORMAT_BUFFER_SIZE];

#define AVAILABLE_OUTPUTS 2

#ifndef FILE_BUFFER_SIZE
#define FILE_BUFFER_SIZE 200
#endif // FILE_BUFFER

char file_buffer[FILE_BUFFER_SIZE];
int output_count = 0;

#ifndef TIMESTAMP_FORMAT
#define TIMESTAMP_FORMAT "%Y-%m-%d %X"
#endif // TIMESTAMP_FORMAT

typedef enum
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
} LoggingLevel;

typedef struct
{
    char *name;
    FILE *outputs[AVAILABLE_OUTPUTS];
    LoggingLevel level;
} Logger;

char *lltostr(LoggingLevel level);
void logger_set_level(Logger *logger, LoggingLevel level);
Logger logger_new(char *name, LoggingLevel level);
void logger_add_console(Logger *logger);
void logger_add_file(Logger *logger, const char *filename);
void logger_full_setup(Logger *logger, const char *filename);
void logger_console_only(Logger *logger);
void logger_file_only(Logger *logger, const char *filename);
void logger_log(Logger *logger, const char *message, LoggingLevel level);
void close_logger(Logger *logger);
#endif // LOGGER_H_

#ifdef LOGGER_IMPLEMENTATION

/*Determine wheather a specific output stream is a file.*/
static bool is_file(FILE *stream)
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

/*Set the locale of the timezone information.*/
static void _set_locale(char *locale)
{
    setlocale(LC_TIME, locale);
}

/*Set a timestamp to be used in the logging format.*/
static void set_timestamp()
{
    time_t t = time(NULL);
    struct tm date = *localtime(&t);
    _set_locale(LOCALE);
    strftime(timestamp, FORMAT_BUFFER_SIZE, TIMESTAMP_FORMAT, &date);
}

/*Represent a LoggingLevel enum as a string.*/
char *lltostr(LoggingLevel level)
{
    switch (level)
    {
        case DEBUG:
            return "DEBUG";
            break;
        case INFO:
            return "INFO";
            break;
        case WARNING:
            return "WARNING";
            break;
        case ERROR:
            return "ERROR";
            break;
        case CRITICAL:
            return "CRITICAL";
            break;
    }
    fprintf(stderr, "%s\n", "Unknown Logging Level");
    exit(1);
}

/*Set the minimum allowable LoggingLevel of the logger. This is inforced in the `logger_log` function.*/
void logger_set_level(Logger *logger, LoggingLevel level)
{
    logger->level = level;
}

/*Create a new logger given a name and a LoggingLevel. This is the API entry point.*/
Logger logger_new(char *name, LoggingLevel level)
{
    Logger logger = {0};
    logger.name = name;
    logger_set_level(&logger, level);
    return logger;
}

/*Add `stdout` to the list of ouputs of the logger. If there's too many outputs that have been "allocated" to the struct, an error will be printed to `stderr` and the programme will exit with and exit code of 1.*/
void logger_add_console(Logger *logger)
{
    if (output_count > AVAILABLE_OUTPUTS)
    {
        fprintf(stderr, "%s\n", "Too many outputs assigned.");
        exit(1);
    }
    logger->outputs[output_count++] = stdout;
}

/*Add a file to the list of outputs of the logger. The file is opened in append mode and is not closed. Any errors are handled with an exit with an exit code of 1 preceded by a `stderr` message.*/
void logger_add_file(Logger *logger, const char *filename)
{
    FILE *file = fopen(filename, "a");
    if (NULL == file)
    {
        fprintf(stderr, "%s\n", "Unable to open the file.");
        exit(1);
    }
    if (output_count > AVAILABLE_OUTPUTS)
    {
        fprintf(stderr, "%s\n", "Too many outputs assigned.");
        fclose(file);
    }
    logger->outputs[output_count++] = file;
    // fclose(file);
}

/*Setup the logger with both `stdout` and a file with a given filename.*/
void logger_full_setup(Logger *logger, const char *filename)
{
    logger_add_console(logger);
    logger_add_file(logger, filename);
}

/*Setup the logger with the console only. */
void logger_console_only(Logger *logger)
{
    logger_add_console(logger);
}

/*Setup the logger with only a file.*/
void logger_file_only(Logger *logger, const char *filename)
{
    logger_add_file(logger, filename);
}

/*Helper function to publish a message to each of the outputs defined in the logger output array.*/
static void _publish_message(Logger *logger, const char *message, LoggingLevel level)
{
    for (int output_num = 0; output_num < output_count; ++output_num)
    {
        fprintf(logger->outputs[output_num], "%s:%s[%s] - %s\n", timestamp, logger->name, lltostr(level), message);
    }
}

/*Close any file outputs linked to the logger.*/
void close_logger(Logger *logger)
{
    for (int output_num = 0; output_num < output_count; ++output_num)
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

/*Log a message.*/
void logger_log(Logger *logger, const char *message, LoggingLevel level)
{
    if (level < logger->level)
    {
        return;
    }
    set_timestamp();
    _publish_message(logger, message, level);
}

#endif // LOGGER_IMPLEMENTATION