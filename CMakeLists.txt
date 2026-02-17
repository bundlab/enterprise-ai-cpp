cmake_minimum_required(VERSION 3.16)
project(enterprise_ai VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/main.cpp
    src/api/http_server.cpp
    src/services/ai_service.cpp
    src/ai/model.cpp
    src/infrastructure/logger.cpp
    src/infrastructure/config.cpp
)

# 1️⃣ Create the executable first
add_executable(enterprise_ai ${SOURCES})

# 2️⃣ Then add include directories
target_include_directories(enterprise_ai PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party
    ${Boost_INCLUDE_DIRS}   # if using Boost
)

# 3️⃣ Then link libraries
find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(enterprise_ai PRIVATE Boost::system)
