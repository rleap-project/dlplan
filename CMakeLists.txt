cmake_minimum_required(VERSION 3.21)

##############################################################
# Language setup
##############################################################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

##############################################################
# Establish project
##############################################################

project(dlplan VERSION 0.1 LANGUAGES C CXX)

# Compilation flags, some configuration-specific
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -DDEBUG")

# Set a default build type if none was specified
set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}', as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
endif()

option(BUILD_TESTS "Enables compilation of tests." OFF)
if (BUILD_TESTS)
    message("Building tests enabled.")
else()
    message("Building tests disabled.")
endif()

##############################################################
# CMake modules and macro files
##############################################################

# make cache variables for install destinations
include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/cmake"
)
include("configure_boost")
include("configure_ccache")

# CCache
configure_ccache()

# Boost
message(${CMAKE_PREFIX_PATH})
configure_boost()
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  message(STATUS "Found Boost: ${Boost_DIR} (found version ${Boost_VERSION})")
endif()

##############################################################
# Add library and executable targets
##############################################################

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(experiments)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

set(DLPLAN_PYTHON On)
if(DLPLAN_PYTHON)
  add_subdirectory(api/python)
endif()

###########
# Install #
###########

# Install header files
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Install cmake scripts
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dlplan/cmake"
)

###########
# Exports #
###########

# https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html

include(CMakePackageConfigHelpers)

# Generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/dlplanConfigVersion.cmake"
  VERSION "${dlplan_VERSION_MAJOR}.${dlplan_VERSION_MINOR}"
  COMPATIBILITY ExactVersion
)

# Create config file
# https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/importing-exporting/index.html
# https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html#generating-a-package-configuration-file
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/dlplanConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dlplan"
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# Install config files
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/dlplanConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/dlplanConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dlplan"
)
