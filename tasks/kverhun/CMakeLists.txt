cmake_minimum_required(VERSION 2.8.9)

project(catch_builder CXX)
include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "./catch")

project(Algorithms)

####################################################
# TSP
####################################################
set(TSP_SOURCE_DIR 08-TSP/impl)
set(TSP_TEST_DIR 08-TSP/unit_tests) 

add_library(TSP SHARED
    ${TSP_SOURCE_DIR}/_Library.h
    ${TSP_SOURCE_DIR}/TSP.h
    ${TSP_SOURCE_DIR}/TSP.cpp
)
target_compile_definitions(TSP PRIVATE TSP_LIBRARY)

add_executable(TSP_Tests ${TSP_TEST_DIR}/tsp_tests.cpp)
target_link_libraries(TSP_Tests PUBLIC TSP)
add_dependencies(TSP_Tests TSP)

####################################################

include_directories(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})
enable_testing(true)  # Enables unit-testing.
