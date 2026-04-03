cmake_minimum_required(VERSION 3.16)
project(GameOfLifeQt6)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(GameOfLife
    WIN32
    main.cpp
    mainwindow.cpp
    mainwindow.h
    GameWidget.cpp
    GameWidget.h


)

target_link_libraries(GameOfLife PRIVATE Qt6::Widgets)
