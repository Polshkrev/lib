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
char timestamp[FORMAT_BUFFER_SIZE];

#define AVAILABLE_OUTPUTS 2

#ifndef FILE_BUFFER_SIZE
#define FILE_BUFFER_SIZE 200
#endif // FILE_BUFFER

static char __file_buffer[FILE_BUFFER_SIZE];
static std::size_t output_count = 0;

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
    const std::string lltostr(level_t level);

    /*
    * @brief Helper function to aid in deconstructing the object.
    * @param outputs An array of outputs to close.
    */
    void close_logger(std::array<FILE *, AVAILABLE_OUTPUTS> outputs);

    /*
    * @brief A Logger.
    */
    class Logger
    {
        public:
            std::string name;
            level_t level;
            explicit Logger(const std::string name, const level_t level) noexcept;

            // explicit Logger(const std::string name) noexcept;

            /*
            * @brief Add `stdout` to the array of outputs.
            */
            void add_console();

            /*
            * @brief Add a file to the array of outputs.
            * @param filename Name of the file to add.
            */
            void add_file(const std::string filename);

            /*
            * @brief Setup the logger with both `stdout` and a file. Under the hood, this function calls both `add_console();` and `add_file(const std::string filename);`.
            * @param filename Name of the file to pass to `add_file(const std::string filename);`.
            */
            void full_setup(const std::string filename);

            /*
            * @brief Setup the logger with only `stdout`.
            */
            void console_only();

            /*
            * @brief Setup the logger with only file.
            * @param filename Name of the file to setup.
            */
            void file_only(const std::string filename);

            /*
            * @brief Log a message.
            * @param message Message to log.
            * @param level level_t to set for the message. If the level is less than the level property of the logger, then the message is not logged. By default, this parametre is set to DEBUG.
            */
            void log(const std::string message, const level_t level) const;
            ~Logger();
        private:
            std::array<FILE *, AVAILABLE_OUTPUTS> outputs;
    };
}

#endif //LOGGER_HPP_

#ifdef LOGGER_IMPLEMENTATION

namespace logging
{

    static void _set_locale(const std::string locale)
    {
        setlocale(LC_TIME, locale.data());
    }

    static void _set_timestamp()
    {
        time_t t = time(nullptr);
        struct tm date = *localtime(&t);
        _set_locale(LOCALE);
        strftime(timestamp, FORMAT_BUFFER_SIZE, TIMESTAMP_FORMAT, &date);
    }

    static void _publish_message(const std::array<FILE *, AVAILABLE_OUTPUTS> outputs, const std::string name, const std::string message, level_t level)
    {
        for (std::size_t output_num = 0; output_num < output_count; ++output_num)
        {
            fprintf(outputs[output_num], "%s:%s[%s] - %s\n", timestamp, name.data(), lltostr(level).c_str(), message.data());
        }
    }

    static bool _is_file(FILE *stream)
    {
        bool found = false;
        std::array<FILE *, 3> streams = {stdout, stdin, stderr};
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

    void close_logger(std::array<FILE *, AVAILABLE_OUTPUTS> outputs)
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
    const std::string lltostr(level_t level)
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

    Logger::Logger(const std::string name, const level_t level = DEBUG) : name(name), level(level) noexcept {}

    /*
    * @brief Add `stdout` to the array of outputs.
    */
    void Logger::add_console()
    {
        if (output_count > AVAILABLE_OUTPUTS)
        {
            std::cerr << "Too many outputs assigned.\n";
            exit(1);
        }
        outputs[output_count++] = stdout;
    }

    /*
    * @brief Add a file to the array of outputs.
    * @param filename Name of the file to add.
    */
    void Logger::add_file(const std::string filename = "./log.log")
    {
        FILE *file = fopen(filename.data(), "a");
        if (NULL == file)
        {
            std::cerr << "Unable to open the file.\n";
            exit(1);
        }
        if (output_count > AVAILABLE_OUTPUTS)
        {
            std::cerr << "Too many outputs assigned.\n";
            fclose(file);
            exit(1);
        }
        outputs[output_count++] = file;
        // fclose(file);
    }

    /*
    * @brief Setup the logger with both `stdout` and a file. Under the hood, this function calls both `add_console();` and `add_file(const std::string filename);`.
    * @param filename Name of the file to pass to `add_file(const std::string filename);`.
    */
    void Logger::full_setup(const std::string filename = "./log.log")
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
    }

    /*
    * @brief Setup the logger with only file.
    * @param filename Name of the file to setup.
    */
    void Logger::file_only(const std::string filename = "./log.log")
    {
        add_file(filename);
    }

    /*
    * @brief Log a message.
    * @param message Message to log.
    * @param level level_t to set for the message. If the level is less than the level property of the logger, then the message is not logged. By default, this parametre is set to DEBUG.
    */
    void Logger::log(const std::string message, const level_t level = DEBUG) const
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