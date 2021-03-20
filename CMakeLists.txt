project(DesktopWidget C CXX)
cmake_minimum_required(VERSION 3.3.2)

include(cmake/App.cmake)

set(SOURCES 
            "src/App.h"
            "src/DesktopBackground.h"
            "src/DesktopBackground.cpp"
            "src/EditorWindow.h"
            "src/HTMLWindow.h"
            "src/pch.cpp"
            "src/pch.h"
            "src/pch.cpp"
            "src/StringHelper.h"
            "src/main.cpp")


add_app("${SOURCES}")

