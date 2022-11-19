cmake_minimum_required(VERSION 3.20)
project(EagleInterpreter)

set(CMAKE_CXX_STANDARD 17)

include_directories(./src)

include_directories(./lib)

set(LIB_FILES
        lib/BigFloat.h
        lib/BigFloat.cc
        src/object.h
        src/object.cpp)

set(SOURCE_FILES
        src/eagle.h
        src/eagle.cpp
        src/interpreter/interpreter.h
        src/interpreter/interpreter.cpp
        src/parser/parser.h
        src/parser/parser.cpp
        src/parser/lexer.h
        src/parser/token.h
        src/parser/lexer.cpp
        src/parser/token.cpp
        src/object.h
        src/object.cpp
        src/ast/stmt.h
        src/ast/stmt.cpp
        src/ast/expr.h
        src/ast/expr.cpp
        src/interpreter/resolver.h
        src/interpreter/resolver.cpp
        src/interpreter/modules/environment.h
        src/interpreter/modules/eagle_class.h
        src/util/error_reporter.h
        src/interpreter/modules/eagle_exceptions.h
        src/util/error_reporter.cpp
        src/interpreter/modules/environment.cpp
        src/interpreter/modules/eagle_exceptions.cpp)

add_library(eagle_lib STATIC ${SOURCE_FILES})
add_library(big_float_lib STATIC ${LIB_FILES})

add_executable(EagleInterpreter src/main.cpp)
target_link_libraries(EagleInterpreter eagle_lib big_float_lib)

add_executable(test_simple test/main.cpp)
target_link_libraries(EagleInterpreter eagle_lib big_float_lib)
