#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <ctime>
#include <clocale>
#include <cstdio>

#include <string>
#include <array>
#include <iostream>

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

namespace logging
{
    /*
    * @brief Representation of a logging level.
    */
    typedef enum
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    } level_t;

    /*
    * @brief Represent a logging level as a std::string.
    * @param level Level to interpret.
    * @returns An upper-case representation of the given logging level.
    */
    const char *lltostr(level_t level);

    /*
    * @brief Helper function to aid in deconstructing the object.
    * @param outputs An array of outputs to close.
    */
    void close_logger(const std::array<FILE *, AVAILABLE_OUTPUTS> &outputs);

    /*
    * @brief A Logger.
    */
    class Logger
    {
        protected:
            /*
            * @brief Name of the logger.
            */
            std::string name;
            /*
            * @brief Minimum severity of a logging message that will be logged.
            */
            const level_t level;
        public:

            /*
            * @brief Construct a new logger with a given name and default level.
            * @param name Name of the logger.
            * @param level Default logging level.
            */
            explicit Logger(const std::string &name, level_t level = DEBUG) noexcept;

            // explicit Logger(const std::string name) noexcept;

            /*
            * @brief Add `stdout` to the array of outputs.
            */
            void add_console();

            /*
            * @brief Add a file to the array of outputs.
            * @param filename Name of the file to add.
            */
            void add_file(const std::string &filename = "./log.log");

            /*
            * @brief Setup the logger with both `stdout` and a file. Under the hood, this function calls both `add_console();` and `add_file(const std::string filename);`.
            * @param filename Name of the file to pass to `add_file(const std::string filename);`.
            */
            void full_setup(const std::string &filename = "./log.log");

            /*
            * @brief Setup the logger with only `stdout`.
            */
            void console_only();

            /*
            * @brief Setup the logger with only file.
            * @param filename Name of the file to setup.
            */
            void file_only(const std::string &filename);

            /*
            * @brief Log a message.
            * @param message Message to log.
            * @param level level_t to set for the message. If the level is less than the level property of the logger, then the message is not logged. By default, this parametre is set to DEBUG.
            */
            void log(const std::string &message, const level_t level) const;
            virtual ~Logger();
        private:
            std::array<FILE *, AVAILABLE_OUTPUTS> outputs;
    };
}

#endif //LOGGER_HPP_

#ifdef LOGGER_IMPLEMENTATION

namespace
{
    char timestamp[FORMAT_BUFFER_SIZE];
    // char __file_buffer[FILE_BUFFER_SIZE];
    std::size_t output_count = 0;
}

namespace logging
{
    /*
    * @brief Set the locale of the logging timestamp.
    * @param locale Locale code string to set the timestamp.
    */
    void _set_locale(const std::string &locale)
    {
        setlocale(LC_TIME, locale.c_str());
    }

    /*
    * @brief Set the current timestamp to aid in logging.
    */
    void _set_timestamp()
    {
        time_t t = time(nullptr);
        struct tm date = *localtime(&t);
        _set_locale(LOCALE);
        strftime(timestamp, FORMAT_BUFFER_SIZE, TIMESTAMP_FORMAT, &date);
    }

    /*
    * @brief Publish a message to each of the outputs stored in the logger.
    * @param outputs View to the outputs stored in the logger.
    * @param name Name of the logger.
    * @param message Message to be logged.
    * @param level Level of the message to be logged.
    */
    void _publish_message(const std::array<FILE *, AVAILABLE_OUTPUTS> &outputs, const std::string &name, const std::string &message, level_t level)
    {
        for (std::size_t output_num = 0; output_num < output_count; ++output_num)
        {
            fprintf(outputs[output_num], "%s:%s[%s] - %s\n", timestamp, name.c_str(), lltostr(level), message.c_str());
        }
    }

    /*
    * @brief Determine if the given stream is a file.
    * @param stream View of a file stream to evaluated as a system file.
    * @returns True if the given stream is neither standard output, error, or input; else false.
    */
    bool _is_file(const FILE *stream)
    {
        bool found = false;
        const std::array<FILE *, 3> streams = {stdout, stdin, stderr};
        for (size_t output = 0; output < streams.size(); ++output)
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
    * @brief Close the logger.
    * @param outputs View of the outputs stored in the logger.
    */
    void close_logger(const std::array<FILE *, AVAILABLE_OUTPUTS> &outputs)
    {
        for (std::size_t output_num = 0; output_num < output_count; ++output_num)
        {
            FILE *current_output = outputs[output_num];
            if (!_is_file(current_output))
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
    * @brief Represent a logging level as a std::string.
    * @param level Level to interpret.
    * @returns An upper-case representation of the given logging level.
    */
    const char *lltostr(level_t level)
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
        std::cerr << "Unknown Logging Level\n";
        exit(1);
    }

    /*
    * @brief Construct a new logger with a given name and default level.
    * @param name Name of the logger.
    * @param level Default logging level.
    */
    Logger::Logger(const std::string &name, level_t level) noexcept : name(name), level(level) {}

    /*
    * @brief Add `stdout` to the array of outputs.
    */
    void Logger::add_console()
    {
        if (output_count >= AVAILABLE_OUTPUTS)
        {
            std::cerr << "Too many outputs assigned.\n";
            exit(1);
        }
        outputs[output_count] = stdout;
        output_count++;
    }

    /*
    * @brief Add a file to the array of outputs.
    * @param filename Name of the file to add.
    */
    void Logger::add_file(const std::string &filename)
    {
        FILE *file = fopen(filename.c_str(), "a");
        if (NULL == file)
        {
            std::cerr << "Unable to open the file.\n";
            exit(1);
        }
        if (output_count >= AVAILABLE_OUTPUTS)
        {
            std::cerr << "Too many outputs assigned.\n";
            fclose(file);
            exit(1);
        }
        outputs[output_count] = file;
        output_count++;
        // fclose(file);
    }

    /*
    * @brief Setup the logger with both `stdout` and a file. Under the hood, this function calls both `add_console();` and `add_file(const std::string filename);`.
    * @param filename Name of the file to pass to `add_file(const std::string filename);`.
    */
    void Logger::full_setup(const std::string &filename)
    {
        add_console();
        add_file(filename);
    }

    /*
    * @brief Setup the logger with only `stdout`.
    */
    void Logger::console_only()
    {
        add_console();
        output_count = 2;
    }

    /*
    * @brief Setup the logger with only file.
    * @param filename Name of the file to setup.
    */
    void Logger::file_only(const std::string &filename = "./log.log")
    {
        add_file(filename);
        output_count = 2;
    }

    /*
    * @brief Log a message.
    * @param message Message to log.
    * @param level level_t to set for the message. If the level is less than the level property of the logger, then the message is not logged. By default, this parametre is set to DEBUG.
    */
    void Logger::log(const std::string &message, level_t level = DEBUG) const
    {
        if (level < this->level)
        {
            return;
        }
        _set_timestamp();
        _publish_message(outputs, name, message, level);
    }

    Logger::~Logger()
    {
        close_logger(outputs);
    }
}

#endif // LOGGER_IMPLEMENTATION