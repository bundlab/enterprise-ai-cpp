#pragma once
#include <iostream>
#include <mutex>

class Logger {
public:
    static void init();
    static void info(const std::string& msg);
};
