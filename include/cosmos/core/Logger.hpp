#pragma once
// ==
// Standard Library
// == 
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

// ==
// Third Party
// ==


// ==
// Cosmos
// ==


namespace cosmos::core  {

// ANSI color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"

enum class LogLevel {
    ERROR = 0,
    INFO  = 1,
    DEBUG = 2
};

class Logger {
public:
    static void set_level(LogLevel level) {
        current_level_ = level;
    }

    static void log(const std::string& message, LogLevel level) {
        if (level > current_level_) return;

        const std::string timestamp = current_time_();

        switch (level) {
            case LogLevel::ERROR:
                std::cout << COLOR_RED << "[ERROR] " << timestamp << " - " << message << COLOR_RESET << std::endl;
                break;
            case LogLevel::INFO:
                std::cout << COLOR_GREEN << "[INFO] " << timestamp << " - " << message << COLOR_RESET << std::endl;
                break;
            case LogLevel::DEBUG:
                std::cout << COLOR_CYAN << "[DEBUG] " << timestamp << " - " << message << COLOR_RESET << std::endl;
                break;
        }
    }

private:
    static inline LogLevel current_level_ = LogLevel::DEBUG;

    static std::string current_time_() {
        auto now = std::chrono::system_clock::now();
        auto t_c = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
#ifdef _WIN32
        localtime_s(&tm, &t_c);
#else
        localtime_r(&t_c, &tm);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};

#define LOG_ERROR(msg) cosmos::core::Logger::log(msg, cosmos::core::LogLevel::ERROR)
#define LOG_INFO(msg)  cosmos::core::Logger::log(msg, cosmos::core::LogLevel::INFO)
#define LOG_DEBUG(msg) cosmos::core::Logger::log(msg, cosmos::core::LogLevel::DEBUG)

}
