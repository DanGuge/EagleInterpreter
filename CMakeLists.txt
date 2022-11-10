cmake_minimum_required(VERSION 3.20)
project(EagleInterpreter)

set(CMAKE_CXX_STANDARD 17)

include_directories(./src)

set(SOURCE_FILES src/interpreter/interpreter.h src/interpreter/interpreter.cpp src/parser/parser.h src/parser/parser.cpp src/object.h)

add_library(eagle_lib STATIC ${SOURCE_FILES})

add_executable(EagleInterpreter src/main.cpp)
target_link_libraries(EagleInterpreter eagle_lib)

add_executable(test_simple test/main.cpp)
target_link_libraries(EagleInterpreter eagle_lib)
