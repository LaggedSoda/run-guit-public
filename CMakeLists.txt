cmake_minimum_required(VERSION 3.14)
project(run_guit)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/usr/lib/cmake/Qt5")

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(SOURCES
    MainWindow.cpp
    ServiceManager.cpp
    main.cpp
)

set(HEADERS
    MainWindow.h
    ServiceManager.h
)

set(UI_FILES
    MainWindow.ui
)

add_executable(run-guit
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

target_link_libraries(run-guit Qt5::Widgets)
