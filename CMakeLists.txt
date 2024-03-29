cmake_minimum_required (VERSION 3.5)

project(catch_builder CXX)
include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    GIT_TAG v1.10.0
    TIMEOUT 10
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "./catch")

# Qt
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Find the QtWidgets library
find_package(Qt5Core REQUIRED)

project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

####################################################
# TSP
####################################################
set(TSP_SOURCE_DIR src/impl)
set(TSP_TEST_DIR src/unit_tests) 

add_library(TSP SHARED
    ${TSP_SOURCE_DIR}/_Library.h
    ${TSP_SOURCE_DIR}/TSP.h
    ${TSP_SOURCE_DIR}/TSP.cpp
    ${TSP_SOURCE_DIR}/GeneticAlgo.h
    ${TSP_SOURCE_DIR}/GeneticAlgo.cpp
    ${TSP_SOURCE_DIR}/PredefinedConfigurations.h
    ${TSP_SOURCE_DIR}/PredefinedConfigurations.cpp
    ${TSP_SOURCE_DIR}/ITSPSolver.h
    ${TSP_SOURCE_DIR}/TSP2OptSolver.h
    ${TSP_SOURCE_DIR}/TSP2OptSolver.cpp
)
target_compile_definitions(TSP PRIVATE TSP_LIBRARY)

add_executable(TSP_Tests 
    ${TSP_TEST_DIR}/tsp_tests.cpp
    ${TSP_TEST_DIR}/tsp_genetic_tests.cpp
)
target_link_libraries(TSP_Tests PUBLIC TSP)
add_dependencies(TSP_Tests TSP)

set(TSP_GUI_SOURCE_DIR src/gui)
add_executable(TSP_GUI 
    ${TSP_GUI_SOURCE_DIR}/main.cpp
    ${TSP_GUI_SOURCE_DIR}/MainWindow.h
    ${TSP_GUI_SOURCE_DIR}/MainWindow.cpp
    ${TSP_GUI_SOURCE_DIR}/TSPDrawingWidget.h
    ${TSP_GUI_SOURCE_DIR}/TSPDrawingWidget.cpp
    ${TSP_GUI_SOURCE_DIR}/TSPGeneticSolver.h
    ${TSP_GUI_SOURCE_DIR}/TSPGeneticSolver.cpp
    ${TSP_GUI_SOURCE_DIR}/TSPSolverStatusReporter.h
    ${TSP_GUI_SOURCE_DIR}/TSPSolverStatusReporter.cpp
    )


#target_link_libraries(TSP_GUI Qt5::WinMain)
qt5_use_modules(TSP_GUI Widgets)
target_link_libraries(TSP_GUI TSP)
####################################################

include_directories(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})
enable_testing(true)  # Enables unit-testing.
