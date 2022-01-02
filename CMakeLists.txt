cmake_minimum_required(VERSION 3.0.0)

project(canvas VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5Widgets REQUIRED)

option(JSON_SERIALISATION "use third part library to serialize best bird brain" ON)

if(JSON_SERIALISATION)
  include(FetchContent)

  FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git
    GIT_TAG v3.7.3)

  FetchContent_GetProperties(json)
  if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
endif()


add_library(lib${PROJECT_NAME}
    src/p5/grid.h
    src/p5/grid.cpp
    src/p5/application.h
    src/p5/application.cpp
    src/p5/qtcanvas.h
    src/p5/qtcanvas.cpp
    )

target_link_libraries(lib${PROJECT_NAME} PUBLIC Qt5::Widgets)


add_library(libNeuralNetwork INTERFACE)
if(JSON_SERIALISATION)
  target_link_libraries(libNeuralNetwork INTERFACE nlohmann_json::nlohmann_json)
  target_compile_definitions(libNeuralNetwork INTERFACE JSON_SERIALIZATION=1 )
endif()

add_executable(minesweeper src/minesweeper.cpp)
target_link_libraries(minesweeper lib${PROJECT_NAME} )


add_executable(flappy_bird src/flappy_bird.cpp)
target_link_libraries(flappy_bird lib${PROJECT_NAME} libNeuralNetwork)

option (BUILD_TESTING "build test" ON)

if(BUILD_TESTING)
    enable_testing()
    find_package(Qt5Test REQUIRED)

    add_executable(test_grid  test/test_grid.cpp)
    target_link_libraries(test_grid Qt5::Test lib${PROJECT_NAME})
    target_include_directories(test_grid PRIVATE src)
    add_test(test_grid test_grid)

    add_executable(test_matrix test/test_matrix.cpp) 
    target_link_libraries(test_matrix Qt5::Test lib${PROJECT_NAME} libNeuralNetwork)
    target_include_directories(test_matrix PRIVATE src)
    add_test(test_matrix test_matrix)

    add_executable(test_nn test/test_nn.cpp) 
    target_link_libraries(test_nn Qt5::Test lib${PROJECT_NAME} libNeuralNetwork)
    target_include_directories(test_nn PRIVATE src)
    add_test(test_nn test_nn )
endif()
