cmake_minimum_required(VERSION 3.16)

project(enterprise_ai LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_definitions(_WIN32_WINNT=0x0601)
endif()

# Reusable core library (health check and future core components)
add_library(enterprise_core STATIC
    src/core/health_check.cpp
)
target_include_directories(enterprise_core PUBLIC ${CMAKE_SOURCE_DIR}/src)

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
target_link_libraries(enterprise_ai PRIVATE enterprise_core)

# Enable pthread (needed by Crow / asio)
find_package(Threads REQUIRED)
target_link_libraries(enterprise_ai PRIVATE Threads::Threads)

# Unit tests
enable_testing()
add_executable(health_check_tests tests/test_health_check.cpp)
target_link_libraries(health_check_tests PRIVATE enterprise_core)
add_test(NAME health_check COMMAND health_check_tests)
