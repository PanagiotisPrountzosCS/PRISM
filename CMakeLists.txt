#====================PROJECT SETUP====================

cmake_minimum_required(VERSION 3.20)
project(PRISM LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(__DEBUG_MODE=1)
    message("Debug build enabled")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(__DEBUG_MODE=2)
    message("Release build enabled")
endif()

add_compile_options(-Wall -Wextra)

#====================SOURCES====================

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(IMGUI_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui)
set(IMPLOT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/implot)
set(CXXOPTS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cxxopts)

set(CORE_SRC 
    ${SRC_DIR}/options_parser.cpp
    ${SRC_DIR}/extras.cpp
    ${SRC_DIR}/mqtt_client.cpp
    ${SRC_DIR}/app.cpp
)

file(GLOB IMGUI_CORE_SRC
    ${IMGUI_ROOT_DIR}/imgui.cpp
    ${IMGUI_ROOT_DIR}/imgui_draw.cpp
    ${IMGUI_ROOT_DIR}/imgui_tables.cpp
    ${IMGUI_ROOT_DIR}/imgui_widgets.cpp
)

file(GLOB IMGUI_BACKEND_SRC 
    ${IMGUI_ROOT_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_ROOT_DIR}/backends/imgui_impl_opengl3.cpp
    )

file(GLOB IMPLOT_SRC
    ${IMPLOT_ROOT_DIR}/implot_items.cpp
    ${IMPLOT_ROOT_DIR}/implot.cpp
)

#====================DEPENDENCIES====================

#sdl2 / opengl
find_package(SDL2)
find_package(OpenGL)

if(SDL2_FOUND AND OpenGL_FOUND)
    set(BUILD_GUI TRUE)
else()
    set(BUILD_GUI FALSE)
endif()

if(BUILD_GUI)
    message("SDL2 + OpenGL found, adding gui target")
else()
    message("SDL2 or OpenGL not found, skipping gui target")
endif()

#cxxopts
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cxxopts)

#imgui
add_library(imgui STATIC 
    ${IMGUI_CORE_SRC} 
    ${IMGUI_BACKEND_SRC}
)
target_link_libraries(imgui SDL2::SDL2 OpenGL::GL)
target_include_directories(imgui PUBLIC 
    ${IMGUI_ROOT_DIR} 
    ${IMGUI_ROOT_DIR}/backends
)

#implot
add_library(implot STATIC
    ${IMPLOT_SRC}
)
target_link_libraries(implot imgui)
target_include_directories(implot PUBLIC 
    ${IMPLOT_ROOT_DIR} 
)

#====================BUILD TARGETS====================

add_executable(prism_cli main.cpp ${CORE_SRC})
target_include_directories(prism_cli PRIVATE ${INCLUDE_DIR})
target_link_libraries(prism_cli PRIVATE cxxopts mosquitto)
target_compile_definitions(prism_cli PUBLIC 
    __APP_MODE=1
    #__APP_MODE=1 means cli build
)

if(BUILD_GUI)
    add_executable(prism_gui main.cpp ${CORE_SRC})
    target_include_directories(prism_gui PRIVATE ${INCLUDE_DIR})
    target_link_libraries(prism_gui PRIVATE imgui implot cxxopts mosquitto)
    target_compile_definitions(prism_gui PUBLIC 
        __APP_MODE=2
        #__APP_MODE=2 means gui build
    )
endif()