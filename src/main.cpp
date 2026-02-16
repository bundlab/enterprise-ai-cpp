#include "api/http_server.h"
#include "infrastructure/logger.hpp"
#include "infrastructure/config.hpp"

int main() {
    Logger::init();
    Config::load("config/config.yaml");

    Logger::info("Starting Enterprise AI System...");

    HttpServer server;
    server.run(8080);

    return 0;
}
