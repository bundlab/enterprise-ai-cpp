cmake_minimum_required(VERSION 3.16)

project(enterprise_ai LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/api/http_server.cpp
    src/services/ai_service.cpp
    src/ai/model.cpp
    src/infrastructure/logger.cpp
    src/infrastructure/config.cpp
)

# Create executable
add_executable(enterprise_ai ${SOURCES})

# Include directories
target_include_directories(enterprise_ai PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party
)

# Enable pthread (needed by Crow)
find_package(Threads REQUIRED)
target_link_libraries(enterprise_ai PRIVATE Threads::Threads)
