cmake_minimum_required(VERSION 3.7.2)

project(pathfinder)

find_package(Qt5Widgets  REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(pathfinder_app main.cpp)
target_link_libraries(pathfinder_app
        PRIVATE
        Qt5::Widgets
        )
