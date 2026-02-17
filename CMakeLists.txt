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

# Key change: include the project root + src + third_party
target_include_directories(enterprise_ai PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}           # project root → allows #include "src/api/http_server.h" if you want
    ${CMAKE_CURRENT_SOURCE_DIR}/src       # ← this makes #include "api/http_server.h" work from everywhere
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

find_package(Threads REQUIRED)
target_link_libraries(enterprise_ai PRIVATE Threads::Threads)

# Optional but recommended: enable more warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(enterprise_ai PRIVATE -Wall -Wextra -Wpedantic)
endif()