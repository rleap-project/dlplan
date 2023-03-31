# We use the earliest cmake 3
cmake_minimum_required(VERSION 3.13)

# If it's available, use ccache to cache compilation results. The two ccache options
# allow sharing compilation results between different build directories.
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND AND NOT WIN32)  # Windows Github Actions find "ccache" --> ignore it.
    message("Using ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE
        "CCACHE_BASEDIR=${CMAKE_CURRENT_SOURCE_DIR} CCACHE_NOHASHDIR=true ccache")
endif(CCACHE_FOUND AND NOT WIN32)

project(dlplan VERSION 0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compilation flags, some configuration-specific
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -DDEBUG")

# Set a default build type if none was specified
set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}', as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
endif()

OPTION(ENABLE_SPARSE "Enables sparse representation for ConceptDenotation and RoleDenotation" OFF)
if (ENABLE_SPARSE)
    MESSAGE("Building with sparse set representation. Recommended for large number of objects.")
else()
    MESSAGE("Building with bitset representation. Recommended for small number of objects.")
endif()

OPTION(ENABLE_TESTING "Enables compilation of tests." OFF)
if (ENABLE_TESTING)
    MESSAGE("Building tests enabled.")
else()
    MESSAGE("Building tests disabled.")
endif()

include_directories(include/)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(experiments)

if (ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
    # TODO: does not work yet
    #add_test(NAME MatmulPython
    #    COMMAND ${Python_EXECUTABLE} -m pytest ${CMAKE_CURRENT_SOURCE_DIR} / "api/python"
    #)
endif()

if(DLPLAN_PYTHON)
  # For the dynamic library to be compatible with Python we need the -fPIC flags
  add_subdirectory(api/python)
endif()
