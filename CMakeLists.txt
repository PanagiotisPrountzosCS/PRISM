cmake_minimum_required(VERSION 3.25)
project(PRISM VERSION 0.1.0 LANGUAGES CXX)

option(BUILD_GUI "Build the GUI application" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



include_directories(${PROJECT_SOURCE_DIR}/include)

# Core library sources (excluding main executables)
add_library(prism_core INTERFACE
    # TO DO: REMOVE INTERFACE^^
    # Core components will be added here
)

# CLI Executable
add_executable(prism_cli src/cli/main.cpp
)
target_link_libraries(prism_cli PRIVATE prism_core)

# GUI Executable (Optional)

if(BUILD_GUI)
    find_package(Qt6 COMPONENTS Widgets Charts QUIET)
    if(Qt6_FOUND)
        add_executable(prism_gui src/gui/main.cpp
        )
        target_link_libraries(prism_gui PRIVATE 
            prism_core
            Qt6::Widgets
            Qt6::Charts
        )
        target_include_directories(prism_gui PRIVATE ${Qt6Widgets_INCLUDE_DIRS})
    else()
        message(STATUS "Qt6 not found - GUI application will NOT be built")
    endif()
else()
    message(STATUS "GUI building is disabled - GUI application will NOT be built")
endif()

# Tests Executable (Optional)
find_package(GTest QUIET)
if(GTest_FOUND)
    enable_testing()
    add_executable(prism_tests test/main.cpp
    )
    target_link_libraries(prism_tests PRIVATE 
        prism_core
        GTest::GTest
        GTest::Main
    )
    add_test(NAME prism_tests COMMAND prism_tests)
else()
    message(STATUS "GTest not found - Tests will not be built")
endif()