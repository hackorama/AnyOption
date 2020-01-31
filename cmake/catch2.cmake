cmake_minimum_required(VERSION 3.13)

include(ExternalProject)

set(repo_catch2 "https://github.com/catchorg/Catch2.git")
message(STATUS "Repository for catch2: ${repo_catch2}")

ExternalProject_add(extern-catch2
    PREFIX ${CMAKE_BINARY_DIR}/catch2
    GIT_REPOSITORY ${repo_catch2}
    TIMEOUT 10
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/catch2/lib/${CMAKE_STATIC_LIBRARY_PREFIX}catch2${CMAKE_STATIC_LIBRARY_SUFFIX}
)

ExternalProject_Get_Property(extern-catch2 source_dir)
add_library(catch INTERFACE)
add_library(catch2::catch2 ALIAS catch)

target_include_directories(catch INTERFACE ${source_dir}/single_include)

add_dependencies(catch extern-catch2)
