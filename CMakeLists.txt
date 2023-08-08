# We use the earliest cmake 3
cmake_minimum_required(VERSION 3.26)

# If it's available, use ccache to cache compilation results. The two ccache options
# allow sharing compilation results between different build directories.
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND AND NOT WIN32)  # Windows Github Actions find "ccache" --> ignore it.
    message("Using ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE
        "CCACHE_BASEDIR=${CMAKE_CURRENT_SOURCE_DIR} CCACHE_NOHASHDIR=true ccache")
endif(CCACHE_FOUND AND NOT WIN32)

project(dlplan VERSION 0.1 LANGUAGES C CXX)

# make cache variables for install destinations
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

#########
# Boost #
#########

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_MIN_VERSION "1.80.0")
if (DEFINED ENV{BOOST_ROOT})
    set(Boost_NO_SYSTEM_PATHS ON)
    set(BOOST_ROOT $ENV{BOOST_ROOT})
endif()
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS serialization)
include_directories(${Boost_INCLUDE_DIRS})

###########
# Targets #
###########

include_directories(include/)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(experiments)

###########
# Testing #
###########

# Compile tests.
option(ENABLE_TESTING "Enables compilation of tests." OFF)
if (ENABLE_TESTING)
    message("Building tests enabled.")
else()
    message("Building tests disabled.")
endif()

option(INSTALL_GMOCK "Install Googletest's GMock?" OFF)
option(INSTALL_GTEST "Install Googletest's GTest?" OFF)

if (ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()


##############
# Python API #
##############

if(DLPLAN_PYTHON)
  # For the dynamic library to be compatible with Python we need the -fPIC flags
  add_subdirectory(api/python)
endif()

############
# Installs #
############

# Install header files
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/include/dlplan"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/dlplan"
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
