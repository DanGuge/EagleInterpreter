cmake_minimum_required(VERSION 3.16)
project(EagleInterpreter)

set(CMAKE_CXX_STANDARD 17)

include_directories(./src)

include_directories(./lib)

set(LIB_FILES
        lib/Number.h
        lib/Number.cc
        src/object.h
        src/object.cpp)

set(BUILT_IN_FUNCTIONS
        src/built_in_functions/eagle_built_in_functions.h
        src/built_in_functions/eagle_built_in_functions.cpp)

set(PRETTY_PRINT_FILES
        src/pretty_print/pretty_print.h
        src/pretty_print/pretty_print.cpp)

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
        src/interpreter/environment.h
        src/modules/eagle_class.h
        src/util/error_reporter.h
        src/modules/eagle_exceptions.h
        src/util/error_reporter.cpp
        src/interpreter/environment.cpp
        src/modules/eagle_exceptions.cpp
        src/modules/eagle_callable.h
        src/modules/eagle_built_in_class.h
        src/modules/eagle_string.h
        src/modules/eagle_string.cpp
        src/modules/eagle_class.cpp
        src/modules/eagle_function.h
        src/modules/eagle_function.cpp
        src/modules/eagle_container.h
        src/modules/eagle_list.cpp
        src/modules/eagle_tuple.cpp
        src/modules/eagle_dict.cpp
        src/shell/eagle_shell.cpp
        src/shell/eagle_shell.h
        src/modules/eagle_stream.h
        src/modules/eagle_stream.cpp
        src/modules/eagle_stack.h
        src/modules/eagle_stack.cpp)

add_library(eagle_lib STATIC ${SOURCE_FILES})
add_library(big_float_lib STATIC ${LIB_FILES})
add_library(pretty_print_lib STATIC ${PRETTY_PRINT_FILES})
add_library(built_in_functions STATIC ${BUILT_IN_FUNCTIONS})

add_executable(EagleInterpreter src/main.cpp)
target_link_libraries(EagleInterpreter eagle_lib big_float_lib pretty_print_lib built_in_functions)

add_executable(test_simple test/main.cpp)
target_link_libraries(EagleInterpreter eagle_lib big_float_lib pretty_print_lib built_in_functions)
