cmake_minimum_required(VERSION 3.16)
project(EnterpriseAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(enterprise_ai
    src/main.cpp
    src/api/http_server.cpp
    src/ai/model.cpp
    src/services/ai_service.cpp
    src/infrastructure/logger.cpp
    src/infrastructure/config.cpp
)

target_include_directories(enterprise_ai PRIVATE
    src
    third_party
)

find_package(Threads REQUIRED)
target_link_libraries(enterprise_ai PRIVATE Threads::Threads)
