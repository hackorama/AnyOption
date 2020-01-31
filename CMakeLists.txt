cmake_minimum_required(VERSION 3.13)

project(anyoption VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(catch2)

option(WARNING_AS_ERRORS "Treat any warning as compile time error" OFF)
option(STATIC_ANALYZERS "Run static analyzers (clang-tidy, cppcheck) during compilation" ON)
option(CODE_COVERAGE "Enable code coverage reports; requires compilation with clamg/llvm" ON)

if(WARNING_AS_ERRORS)
  set(COMPILE_WARNING_AS_ERROR "-Werror")
endif(WARNING_AS_ERRORS)

if(STATIC_ANALYZERS)
  find_program(CPPCHECK_COMMAND NAMES cppcheck)
  set(CMAKE_CXX_CPPCHECK "${CPPCHECK_COMMAND};--quiet;--template=gcc;--suppress=*:*/*_autogen/*")
endif(STATIC_ANALYZERS)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    src/
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    ${COMPILE_WARNING_AS_ERROR}
#    -Wall
#    -Wextra
#    -Wshadow
#    -Wnon-virtual-dtor
#    -pedantic
#    -Wold-style-cast
#    -Wcast-align
#    -Wunused
#    -Woverloaded-virtual
#    -Wpedantic
#    -Wconversion
#    -Wsign-conversion
#    -Wnull-dereference
#    -Wdouble-promotion
)

target_sources(${PROJECT_NAME}
    PRIVATE
    src/anyoption.cpp
    src/anyoption.h
)

#-------------------------------------------------------------------------------------------
add_executable(${PROJECT_NAME}-test)

target_sources(${PROJECT_NAME}-test
    PRIVATE
    tests/test.cpp
)

target_link_libraries(${PROJECT_NAME}-test
    PUBLIC
    anyoption::anyoption
    catch2::catch2
)

if( STATIC_ANALYZERS )
    set_target_properties(${PROJECT_NAME}-test PROPERTIES CXX_CPPCHECK "")
endif(STATIC_ANALYZERS)

add_test(${PROJECT_NAME}-test ${PROJECT_NAME}-test)

#-------------------------------------------------------------------------------------------
add_executable(${PROJECT_NAME}-demo)

target_sources(${PROJECT_NAME}-demo
    PRIVATE
    demo/demo.cpp
)

target_link_libraries(${PROJECT_NAME}-demo
    PUBLIC
    anyoption::anyoption
)
