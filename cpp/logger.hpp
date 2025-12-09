#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string> // std::string
#include <array> // std::array

namespace
{
    #define AVAILABLE_OUTPUTS 2
}

namespace polutils
{
    namespace logging
    {
        /*
        * @brief Severity of a logging message.
        */
        enum level_t
        {
            DEBUG,
            INFO,
            WARNING,
            ERROR,
            CRITICAL
        };

        /*
        * @brief Represent a logging level as a string.
        * @param level LoggingLevel to represent as a string.
        * @returns A string representation of the given level is returned.
        * @exception If the given logging level is not available, a `UnreachableError` is thrown; else a string representation of the given level is returned.
        */
        const char *lltostr(level_t level);

        /*
        * @brief Close any file opened from the logger. If no file has been added to the logger, there is no need to call this function; although this is internally checked.
        */
        void close(const std::array<FILE *, AVAILABLE_OUTPUTS> &outputs);

        /*
        * @brief A logger.
        */
        class logger_t
        {
            protected:
                /*
                * @brief Name of the logger.
                */
                std::string name;
                /*
                * @brief Minimum allowed logging level.
                */
                level_t level;
            public:
                /*
                * @brief Construct a new logger given a name and a logging level.
                * @param name Name to give the logger.
                * @param level Minimum logging level that will be logged.
                */
                explicit logger_t(const std::string &name = "main", level_t level = DEBUG) noexcept;

                /*
                * @brief Add `stdout` to the logger.
                * @param logger The logger to which to update.
                * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
                */
                void add_console(void);

                /*
                * @brief Add a file to the logger. The file is opened in append mode and is not closed.
                * @param filename Name of the file to add to the logger.
                * @exception If the the file does not exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit.
                * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
                */
                void add_file(const std::string &filename = "./log.log");

                /*
                * @brief Add both `stdout` and a file to the logger.
                * @param filename Name of the file to add to the logger.
                * @exception If the the file does not exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit.
                * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
                */
                void full_setup(const std::string &filename = "./log.log");

                /*
                * @brief Log a message.
                * @param message Message to log.
                * @param level The level of the message. If the given level is less than the minimum the logger has allowed, the message will not be logged.
                */
                void log(const std::string &message, level_t level) const noexcept;

                /*
                * @brief Close a logger.
                */
                virtual ~logger_t();
            private:
                /*
                * @brief List of available outputs.
                */
                std::array<FILE *, AVAILABLE_OUTPUTS> outputs;
        };
    }
}

#endif // LOGGER_HPP

#ifdef LOGGER_IMPLEMENTATION

#define EXCEPTION_IMPLEMENTATION
#include "exception.hpp"

#include <cstddef> // std::size_t
#include <cstdio> // stdout
#include <ctime> // time_t, struct tm, strftime
#include <clocale> // setlocale, LC_TIME

namespace
{
    #define FORMAT_BUFFER_SIZE 200

    char timestamp[FORMAT_BUFFER_SIZE] = {0};
    
    #ifndef TIMESTAMP_FORMAT
    #define TIMESTAMP_FORMAT "%Y-%m-%d %X"
    #endif // TIMESTAMP_FORMAT

    #ifndef LOCALE
    #define LOCALE "en_US.UTF-8"
    #endif // LOCALE

    std::size_t output_count = 0;

    /*
    * @brief Set the locale of the timezone information.
    * @param locale Country code of the target locale.
    */
    void _set_locale(const char *locale)
    {
        setlocale(LC_TIME, locale);
    }

    /*
    * @brief Set a timestamp to be used in the logging format.
    */
    void set_timestamp(void)
    {
        time_t t = time(NULL);
        struct tm date = *localtime(&t);
        _set_locale(LOCALE);
        strftime(timestamp, FORMAT_BUFFER_SIZE, TIMESTAMP_FORMAT, &date);
    }

    /*
    * @brief Determine wheather a specific output stream is a file.
    * @param stream Output file stream to check against i/o output.
    * @returns A boolean of whether a given file is composed of an i/o stream (e.g.) stdout, stdin, and stderr.
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
}

namespace polutils
{
    namespace logging
    {
        /*
        * @brief Represent a logging level as a string.
        * @param level LoggingLevel to represent as a string.
        * @returns A string representation of the given level is returned.
        * @exception If the given logging level is not available, a `UnreachableError` is thrown; else a
        */
        const char *lltostr(level_t level)
        {
            switch (level)
            {
                case DEBUG:
                {
                    return "DEBUG";
                } break;
                case INFO:
                {
                    return "INFO";
                } break;
                case WARNING:
                {
                    return "WARNING";
                } break;
                case ERROR:
                {
                    return "ERROR";
                } break;
                case CRITICAL:
                {
                    return "CRITICAL";
                } break;
            }
            throw UnreachableError("Cock and balls.");
        }

        /*
        * @brief Construct a new logger given a name and a logging level.
        * @param name Name to give the logger.
        * @param level Minimum logging level that will be logged.
        */
        logger_t::logger_t(const std::string &name, level_t level) noexcept : name(name), level(level) {};

        /*
        * @brief Add `stdout` to the logger.
        * @param logger The logger to which to update.
        * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
        */
        void logger_t::add_console(void)
        {
            if (output_count >= AVAILABLE_OUTPUTS)
            {
                throw ValueError("The number of allocated outputs has exceded the maximum allowed.");
            }
            outputs[output_count++] = stdout;
        }

        /*
        * @brief Add a file to the logger. The file is opened in append mode and is not closed.
        * @param filename Name of the file to add to the logger.
        * @exception If the the file does not exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit.
        * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
        */
        void logger_t::add_file(const std::string &filename)
        {
            FILE *file = fopen(filename.c_str(), "a");
            if (nullptr == file)
            {
                throw FileNotFoundError("Unable to open file.");
            }
            else if (output_count >= AVAILABLE_OUTPUTS)
            {
                close(outputs);
                throw ValueError("The number of allocated outputs has exceded the maximum allowed.");
            }
            outputs[output_count++] = file;
        }
        /*
        * @brief Add both `stdout` and a file to the logger.
        * @param filename Name of the file to add to the logger.
        * @exception If the the file does not exist, a `FileNotFoundError` will be printed to `stderr` and the programme will exit.
        * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
        */
        void logger_t::full_setup(const std::string &filename)
        {
            add_console();
            add_file(filename);
        }

        /*
        * @brief Log a message.
        * @param message Message to log.
        * @param level The level of the message. If the given level is less than the minimum the logger has allowed, the message will not be logged.
        */
        void logger_t::log(const std::string &message, level_t level) const noexcept
        {
            if (level < this->level)
            {
                return;
            }
            set_timestamp();
            for (std::size_t output_num = 0; output_num < output_count; ++output_num)
            {
                fprintf(outputs[output_num], "%s:%s[%s] - %s\n", timestamp, name.c_str(), lltostr(level), message);
            }
        }

        /*
        * @brief Close a logger.
        */
        logger_t::~logger_t()
        {
            close(outputs);
        }

        /*
        * @brief Close any file opened from the logger. If no file has been added to the logger, there is no need to call this function; although this is internally checked.
        */
        void close(const std::array<FILE *, AVAILABLE_OUTPUTS> &outputs)
        {
            for (size_t output_num = 0; output_num > output_count; ++output_num)
            {
                FILE *current_output = outputs[output_num];
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
    }
}

#endif // LOGGER_IMPLEMENTATION