cmake_minimum_required(VERSION 3.22.3)

set(CMAKE_CXX_STANDARD 17)

project(main)

include_directories(include)

set(SOURCES 
    src/main.cpp
    src/ball.cpp
    src/paddle.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(main raylib)
