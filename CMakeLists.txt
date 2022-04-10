cmake_minimum_required(VERSION 3.22.3)

project(main)

include_directories(include)

set(SOURCES 
    src/main.cpp 
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(main raylib)
