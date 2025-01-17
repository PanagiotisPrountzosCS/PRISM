# =================================================================
# ======================PROJECT SETUP==============================
# =================================================================

cmake_minimum_required(VERSION 3.25)
project(PRISM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =================================================================
# ======================HEADERS / SOURCES==========================
# =================================================================


set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)

set(HEADERS
    ${INCLUDE_DIR}/cli/mainapplication.h
    ${INCLUDE_DIR}/globaldefinitions.h
    ${INCLUDE_DIR}/core/objectid.h
    ${INCLUDE_DIR}/core/sensor.h
    ${INCLUDE_DIR}/core/measurement.h
    ${INCLUDE_DIR}/datagenerators/randomnumberfactory.h
)

set(SOURCES
    ${SOURCE_DIR}/core/objectid.cpp
    ${SOURCE_DIR}/core/sensor.cpp
    ${SOURCE_DIR}/datagenerators/randomnumberfactory.cpp
)

set(TEST_SOURCES
    ${TEST_DIR}/main.cpp
    ${TEST_DIR}/test_objectid.cpp
    ${TEST_DIR}/test_sensor.cpp
    ${TEST_DIR}/test_measurement.cpp
)

set(CLI_SOURCES
    ${SOURCE_DIR}/cli/mainapplication.cpp
    ${SOURCE_DIR}/cli/main.cpp
)

#=================================================================
#=========================BUILD CONFIGURATION=====================
#=================================================================

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DNDEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ftree-vectorize -march=native")
endif()

# Core Library
add_library(prism_core STATIC ${SOURCES})
target_include_directories(prism_core PRIVATE ${INCLUDE_DIR})

#=================================================================
#=========================APPLICATIONS============================
#=================================================================

# CLI Application

add_executable(prism_cli src/cli/main.cpp ${CLI_SOURCES})
target_include_directories(prism_cli PRIVATE ${INCLUDE_DIR})
target_link_libraries(prism_cli PRIVATE prism_core)

# Tests Application (Optional)
# =================================================================

option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    find_package(GTest QUIET)
    if(GTest_FOUND)
        message("GTest found, building tests...")
        add_executable(prism_tests test/main.cpp ${TEST_SOURCES})
        target_include_directories(prism_tests PRIVATE ${INCLUDE_DIR})
        target_link_libraries(prism_tests 
            prism_core
            GTest::GTest 
            GTest::Main
        )
        add_test(NAME prism_tests COMMAND prism_tests)
    else()
        message("GTest not found, skipping tests...")
    endif()
endif()

# GUI Application (Optional)
# =================================================================

option(BUILD_GUI "Build the GUI application" OFF)
if(BUILD_GUI)
    find_package(Qt6 COMPONENTS Widgets Charts QUIET)
    if(Qt6_FOUND)
        add_executable(prism_gui src/gui/main.cpp)
        target_link_libraries(prism_gui PRIVATE 
            prism_core
            Qt6::Widgets
            Qt6::Charts
        )
    else()
        message("Qt6 not found, skipping GUI application...")
    endif()
endif()