#include "config.hpp"
#include <fstream>
#include <iostream>

void Config::load(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Config file not found!" << std::endl;
        return;
    }
    std::cout << "Config loaded from " << path << std::endl;
}
