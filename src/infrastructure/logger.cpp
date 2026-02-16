#include "logger.hpp"

static std::mutex logMutex;

void Logger::init() {
    info("Logger initialized");
}

void Logger::info(const std::string& msg) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << "[INFO] " << msg << std::endl;
}
