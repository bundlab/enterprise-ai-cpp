#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger {
private:
    static std::string levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG:    return "DEBUG";
            case LogLevel::INFO:     return "INFO";
            case LogLevel::WARNING:  return "WARNING";
            case LogLevel::ERROR:    return "ERROR";
            case LogLevel::CRITICAL: return "CRITICAL";
        }
        return "UNKNOWN";
    }

    static std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    static void log(LogLevel level, const std::string& message) {
        std::cout << "[" << getCurrentTimestamp() << "] "
                  << "[" << levelToString(level) << "] "
                  << message << std::endl;
    }

public:
    template<typename... Args>
    static void debug(const std::string& message) { log(LogLevel::DEBUG, message); }

    template<typename... Args>
    static void info(const std::string& message) { log(LogLevel::INFO, message); }

    template<typename... Args>
    static void warning(const std::string& message) { log(LogLevel::WARNING, message); }

    template<typename... Args>
    static void error(const std::string& message) { log(LogLevel::ERROR, message); }

    template<typename... Args>
    static void critical(const std::string& message) { log(LogLevel::CRITICAL, message); }
};