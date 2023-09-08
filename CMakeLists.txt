cmake_minimum_required(VERSION 3.12)
project(GabrielsWM VERSION 0.1.0)

# Set additional module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include debug and testing settings
include(CTest)
enable_testing()

# List all source files
file(GLOB_RECURSE source_files src/*.cpp src/*.hpp)


# Create the executable target
add_executable(${PROJECT_NAME} ${source_files})

# Set the required C++ standard version
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# CPack settings for packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# XLib
find_package(X11 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${X11_INCLUDE_DIR})

# XComposite
find_library(XCOMPOSITE_LIBRARIES NAMES Xcomposite)
find_path(XCOMPOSITE_INCLUDE_DIRS NAMES X11/extensions/Xcomposite.h)
target_link_libraries(${PROJECT_NAME} PRIVATE ${XCOMPOSITE_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${XCOMPOSITE_INCLUDE_DIRS})

# XRENDER
find_library(XRENDER_LIBRARIES NAMES Xrender)
find_path(XRENDER_INCLUDE_DIRS NAMES X11/extensions/Xrender.h)
target_link_libraries(${PROJECT_NAME} PRIVATE ${XRENDER_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${XRENDER_INCLUDE_DIRS})

# XDamage (Manually set the variables)
find_library(XDAMAGE_LIBRARIES NAMES Xdamage)
find_path(XDAMAGE_INCLUDE_DIRS NAMES X11/extensions/Xdamage.h)

# Include the XDamage variables
target_link_libraries(${PROJECT_NAME} PRIVATE ${XDAMAGE_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${XDAMAGE_INCLUDE_DIRS})

# Cairo
find_package(Cairo REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CAIRO_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CAIRO_INCLUDE_DIRS})
