cmake_minimum_required(VERSION 3.24)

project(nihilox_tests)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set (PROJECT_HEADERS
        ../src/lexer.h
        ../src/ast.h
        ../src/parse/parser.h
        ../src/parse/pretty_print.h
        )

set (PROJECT_SOURCES
        ../src/lexer.cpp
        ../src/parse/parser.cpp
        ../src/parse/pretty_print.cpp
        )

set(TEST_SOURCES
        expressionTest.cpp
        lexerTest.cpp
        )

add_executable(nihilox_tests ${PROJECT_HEADERS} ${PROJECT_SOURCES} ${TEST_SOURCES})
target_link_libraries(nihilox_tests gtest_main)
include(GoogleTest)
gtest_discover_tests(nihilox_tests)