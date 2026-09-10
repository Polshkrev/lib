#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdio>   // std::FILE, std::fclose, std::fprintf, std::fopen, stdout
#include <cstddef> // std::size_t

#include <string> // std::string
#include <array> // std::array

namespace
{
    /**
     * @brief Total number of possible outputs that can be allocated at once.
     */
    #define AVAILABLE_OUTPUTS 2
}

namespace polutils
{
    namespace logging
    {
        /**
         * @brief Severity of a logging message.
         */
        enum class level_t
        {
            DEBUG,
            INFO,
            WARNING,
            ERROR,
            CRITICAL
        };

        /**
         * @brief Represent a logging level as a string.
         * @param level LoggingLevel to represent as a string.
         * @returns A string representation of the given level is returned.
         */
        const char *lltostr(level_t level) noexcept;

        /**
         * @brief A logger.
         */
        class logger_t
        {
            protected:
                /**
                 * @brief Name of the logger.
                 */
                std::string _name;

                /**
                 * @brief Minimum allowed logging level.
                 */
                level_t _level;
            public:
                /**
                 * @brief Construct a new logger given a name and a logging level.
                 * @param name Name to give the logger.
                 * @param level Minimum logging level that will be logged.
                 */
                explicit logger_t(const std::string &name = "main", level_t level = level_t::DEBUG) noexcept;

                /**
                 * @brief Obtain the name of the logger.
                 * @returns A non-owning view into the name of the logger.
                 */
                const std::string &name(void) const noexcept;

                /**
                 * @brief Obtain the logging level of the logger.
                 * @returns The level of the logger.
                 */
                level_t level(void) const noexcept;

                /**
                 * @brief Set the name of the logger.
                 * @param name New name to set to the logger.
                 */
                void set_name(const std::string &name) noexcept;

                /**
                 * @brief Set the minimum level of the logger.
                 * @param level New minimum level of the logger.
                 */
                void set_level(level_t level) noexcept;

                /**
                 * @brief Add `stdout` to the logger.
                 * @param logger The logger to which to update.
                 * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be printed to `stderr` and the programme exits.
                 */
                void add_console(void);

                /**
                 * @brief Add a file to the logger. The file is opened in append mode and is not closed.
                 * @param filename Name of the file to add to the logger.
                 * @exception If the the file does not exist, a `FileNotFoundError` will be thrown.
                 * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be thrown.
                 */
                void add_file(const std::string &filename = "./log.log");

                /**
                 * @brief Add both `stdout` and a file to the logger.
                 * @param filename Name of the file to add to the logger.
                 * @exception If the the file does not exist, a `FileNotFoundError` will be thrown.
                 * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be trown.
                 */
                void full_setup(const std::string &filename = "./log.log");

                /**
                 * @brief Log a message.
                 * @param message Message to log.
                 * @param level The level of the message. If the given level is less than the minimum the logger has allowed, the message will not be logged.
                 */
                void log(const std::string &message, level_t level = level_t::DEBUG) const noexcept;

                /**
                 * @brief Close any file opened from the logger.
                 * @brief If no file has been added to the logger, there is no need to call this method; although this is internally checked.
                 */
                void close(void) noexcept;
            private:
                /**
                 * @brief List of available outputs.
                 */
                std::array<std::FILE *, AVAILABLE_OUTPUTS> __outputs;

                /**
                 * @brief Count of currently stored outputs.
                 */
                size_t __output_count;
        };
    }
}

#endif // LOGGER_HPP

#ifdef LOGGER_IMPLEMENTATION

#define EXCEPTIONS_IMPLEMENTATION
#include "exceptions.hpp" // FileNotFoundError, ValueError

#include <ctime>    // std::time_t, std::tm, std::time, std::localtime_r, std::strftime
#include <clocale>  // std::setlocale, LC_TIME

namespace
{
    /**
     * @brief Buffer size to format a __timestamp.
     */
    #define FORMAT_BUFFER_SIZE 200

    /**
     * @brief Timestamp to be displayed in a log message.
     */
    char __timestamp[FORMAT_BUFFER_SIZE] = {0};
    
    #ifndef TIMESTAMP_FORMAT
    /**
     * @brief Default timestamp format. This can be overridden.
     */
    #define TIMESTAMP_FORMAT "%Y-%m-%d %X"
    #endif // TIMESTAMP_FORMAT

    #ifndef LOCALE
    /**
     * @brief Locale of the timestamp format. This can be overridden.
     */
    #define LOCALE "en_US.UTF-8"
    #endif // LOCALE

    /**
     * @brief Set the locale of the timezone information.
     * @param locale Country code of the target locale.
     */
    void _set_locale(const char *locale)
    {
        setlocale(LC_TIME, locale);
    }

    /**
     * @brief Set a timestamp to be used in the logging format.
     * @returns True if the timestamp can be set, else false.
     */
    bool _set_timestamp(void)
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
     * @brief Determine wheather a specific output stream is a file.
     * @param stream Output file stream to check against i/o output.
     * @returns A boolean of whether a given file is composed of an i/o stream (e.g.) stdout, stdin, and stderr.
     */
    bool _is_file(const std::FILE *stream)
    {
        bool found = false;
        std::FILE *streams[3] = {stdout, stdin, stderr};
        for (int output = 0; output < 3; ++output)
        {
            if (streams[output] != stream) continue;
            found = true;
        }
        return !found;
    }
}

namespace polutils
{
    namespace logging
    {
        /**
         * @brief Represent a logging level as a string.
         * @param level LoggingLevel to represent as a string.
         * @returns A string representation of the given level is returned.
         */
        const char *lltostr(level_t level) noexcept
        {
            switch (level)
            {
                case level_t::DEBUG:
                {
                    return "DEBUG";
                } break;
                case level_t::INFO:
                {
                    return "INFO";
                } break;
                case level_t::WARNING:
                {
                    return "WARNING";
                } break;
                case level_t::ERROR:
                {
                    return "ERROR";
                } break;
                case level_t::CRITICAL:
                {
                    return "CRITICAL";
                } break;
                default:
                {
                    return "NONE";
                } break;
            }
        }

        /**
         * @brief Construct a new logger given a name and a logging level.
         * @param name Name to give the logger.
         * @param level Minimum logging level that will be logged.
         */
        logger_t::logger_t(const std::string &name, level_t level) noexcept : _name(name), _level(level), __outputs({0}), __output_count(0) { _set_locale(LOCALE); }

        /**
         * @brief Obtain the name of the logger.
         * @returns A non-owning view into the name of the logger.
         */
        const std::string &logger_t::name(void) const noexcept
        {
            return _name;
        }

        /**
         * @brief Obtain the logging level of the logger.
         * @returns The level of the logger.
         */
        level_t logger_t::level(void) const noexcept
        {
            return _level;
        }

        /**
         * @brief Set the name of the logger.
         * @param name New name to set to the logger.
         */
        void logger_t::set_name(const std::string &name) noexcept
        {
            _name = name;
        }

        /**
         * @brief Set the minimum level of the logger.
         * @param level New minimum level of the logger.
         */
        void logger_t::set_level(level_t level) noexcept
        {
            _level = level;
        }

        /**
         * @brief Add `stdout` to the logger.
         * @param logger The logger to which to update.
         * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be thrown.
         */
        void logger_t::add_console(void)
        {
            if (__output_count >= AVAILABLE_OUTPUTS)
            {
                throw ValueError("The number of allocated outputs has exceded the maximum allowed.");
            }
            __outputs[__output_count++] = stdout;
        }

        /**
         * @brief Add a file to the logger. The file is opened in append mode and is not closed.
         * @param filename Name of the file to add to the logger.
         * @exception If the the file does not exist, a `FileNotFoundError` will be thrown.
         * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be thrown.
         */
        void logger_t::add_file(const std::string &filename)
        {
            if (__output_count >= AVAILABLE_OUTPUTS)
            {
                throw ValueError("The number of allocated outputs has exceded the maximum allowed.");
            }
            std::FILE *file = std::fopen(filename.c_str(), "a");
            if (nullptr == file)
            {
                throw FileNotFoundError("Unable to open file.");
            }
            __outputs[__output_count++] = file;
        }

        /**
         * @brief Add both `stdout` and a file to the logger.
         * @param filename Name of the file to add to the logger.
         * @exception If the the file does not exist, a `FileNotFoundError` will be thrown.
         * @exception If the number of outputs that have been added to the logger has exceded the maximum allowed, a `ValueError` will be trown.
         */
        void logger_t::full_setup(const std::string &filename)
        {
            add_console();
            add_file(filename);
        }

        /**
         * @brief Log a message.
         * @param message Message to log.
         * @param level The level of the message. If the given level is less than the minimum the logger has allowed, the message will not be logged.
         */
        void logger_t::log(const std::string &message, level_t level) const noexcept
        {
            if (level < _level) return;
            bool timestamp_result = _set_timestamp();
            for (std::size_t output_num = 0; output_num < __output_count; ++output_num)
            {
                if (!timestamp_result)
                {
                    std::fprintf(__outputs[output_num], "%s[%s] - %s\n", _name.c_str(), lltostr(level), message.c_str());
                    continue;
                }
                std::fprintf(__outputs[output_num], "%s:%s[%s] - %s\n", __timestamp, _name.c_str(), lltostr(level), message.c_str());
            }
        }

        /**
         * @brief Close any file opened from the logger. If no file has been added to the logger, there is no need to call this function; although this is internally checked.
         * @param outputs Array of file pointers to close.
         */
        void logger_t::close(void) noexcept
        {
            for (size_t output_num = 0; output_num < __outputs.size(); ++output_num)
            {
                std::FILE *current_output = __outputs[output_num];
                if (!current_output) continue;
                else if (!_is_file(current_output)) continue;
                fclose(current_output);
                __outputs[output_num] = nullptr;
                __output_count--;
            }
        }
    }
}

#endif // LOGGER_IMPLEMENTATION