cmake_minimum_required(VERSION 3.0.0)
project(FirstWM VERSION 0.1.0)

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

# Executable
add_executable(${PROJECT_NAME} ${source_files})
