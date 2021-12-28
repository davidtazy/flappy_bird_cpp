cmake_minimum_required(VERSION 3.0.0)

project(canvas VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5Widgets REQUIRED)

add_library(lib${PROJECT_NAME}
    src/p5/grid.h
    src/p5/grid.cpp
    src/p5/application.h
    src/p5/application.cpp
    src/p5/qtcanvas.h
    src/p5/qtcanvas.cpp
    )

target_link_libraries(lib${PROJECT_NAME} PUBLIC Qt5::Widgets)

add_executable(minesweeper src/minesweeper.cpp)
target_link_libraries(minesweeper lib${PROJECT_NAME} )

option (BUILD_TESTING "build test" ON)

if(BUILD_TESTING)
    enable_testing()
    find_package(Qt5Test REQUIRED)
    add_executable(test_grid
        test/test_grid.cpp
       )
    target_link_libraries(test_grid Qt5::Test lib${PROJECT_NAME})
    target_include_directories(test_grid PRIVATE src)
    add_test(test_grid test_generator)
endif()
