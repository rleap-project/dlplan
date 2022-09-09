# We use the earliest cmake 3
cmake_minimum_required(VERSION 3.13)
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
    add_test(NAME MatmulPython
        COMMAND ${Python_EXECUTABLE} -m pytest ${CMAKE_CURRENT_SOURCE_DIR} / "api/python"
    )
endif()

if(DLPLAN_PYTHON)
  # For the dynamic library to be compatible with Python we need the -fPIC flags
  add_subdirectory(api/python)
endif()

## Compile the scorpion planer
# https://stackoverflow.com/questions/2937128/cmake-add-custom-command-not-being-run
find_package(Python3 COMPONENTS Interpreter REQUIRED)
message("${PYTHON_EXECUTABLE}")
message("${PROJECT_SOURCE_DIR}/libs/scorpion/build.py")
add_custom_command(
  OUTPUT compile_scorpion
  COMMAND "${PYTHON_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/libs/scorpion/build.py"
)
add_custom_target(
  scorpion
  DEPENDS compile_scorpion
)
add_dependencies(dlplanstatespace scorpion)
