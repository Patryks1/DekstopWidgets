project(DesktopWidget C CXX)
cmake_minimum_required(VERSION 3.3.2)

include(cmake/App.cmake)

set(SOURCES "src/DesktopWidget.h"
            "src/DesktopWidget.cpp"
            "src/main.cpp")

add_app("${SOURCES}")