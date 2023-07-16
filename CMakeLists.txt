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

# # XComposite
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(XCOMPOSITE REQUIRED xcomposite)
# link_libraries(${XCOMPOSITE_LIBRARIES})
# include_directories(${XCOMPOSITE_INCLUDE_DIRS})
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${XCOMPOSITE_CFLAGS_OTHER}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${XCOMPOSITE_CFLAGS_OTHER}")

# XComposite
find_library(XCOMPOSITE_LIBRARIES NAMES Xcomposite)
find_path(XCOMPOSITE_INCLUDE_DIRS NAMES X11/extensions/Xcomposite.h)
link_libraries(${XCOMPOSITE_LIBRARIES})
include_directories(${XCOMPOSITE_INCLUDE_DIRS})

find_library(XRENDER_LIBRARIES NAMES Xrender)
find_path(XRENDER_INCLUDE_DIRS NAMES X11/extensions/Xrender.h)
link_libraries(${XRENDER_LIBRARIES})
include_directories(${XRENDER_INCLUDE_DIRS})

# XDamage (Manually set the variables)
find_library(XDAMAGE_LIBRARIES NAMES Xdamage)
find_path(XDAMAGE_INCLUDE_DIRS NAMES X11/extensions/Xdamage.h)

# Include the XDamage variables
link_libraries(${XDAMAGE_LIBRARIES})
include_directories(${XDAMAGE_INCLUDE_DIRS})

# Cairo
find_package(Cairo REQUIRED)
link_libraries(${CAIRO_LIBRARIES})
include_directories(${CAIRO_INCLUDE_DIRS})

# Executable
add_executable(${PROJECT_NAME} ${source_files})
