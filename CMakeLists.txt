cmake_minimum_required(VERSION 3.24)
project(nihilox)
set(CMAKE_CXX_STANDARD 17)
enable_testing()

set (PROJECT_HEADERS
        src/lexer.h
        src/ast.h
        src/parse/parser.h
        src/parse/pretty_print.h
        src/token_iterator.h
        )

set (PROJECT_SOURCES
        src/lexer.cpp
        src/parse/parser.cpp
        src/parse/pretty_print.cpp
        src/main.cpp
        )

set (TEST_SOURCES
        test/expressionTest.cpp
        )

add_executable(nihilox ${PROJECT_HEADERS} ${PROJECT_SOURCES})
add_subdirectory(test)