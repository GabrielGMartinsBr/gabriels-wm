cmake_minimum_required(VERSION 3.0.0)
project(FirstWM VERSION 0.1.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Debug
include(CTest)
enable_testing()

# Source Files
# file(GLOB source_files src/*.cpp)
file(GLOB_RECURSE source_files src/*.cpp)

# CPack 
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# XLib
find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})

# Cairo
find_package(Cairo REQUIRED)
link_libraries(${CAIRO_LIBRARIES})
include_directories(${CAIRO_INCLUDE_DIRS})

# Executable
add_executable(${PROJECT_NAME} ${source_files})
