cmake_minimum_required(VERSION 3.16.3)
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


include_directories(include)

# Build the library
add_subdirectory(src)
# Build the examples
add_subdirectory(examples)

enable_testing()  # This needs to be here before calling add_subdirectory
add_subdirectory(tests)

if(DLPLAN_PYTHON)
  # For the dynamic library to be compatible with Python we need the -fPIC flags
  set_property(TARGET core PROPERTY POSITION_INDEPENDENT_CODE ON)
  set_property(TARGET generator PROPERTY POSITION_INDEPENDENT_CODE ON)
  add_subdirectory(api/python)
endif()

INSTALL(TARGETS core generator DESTINATION ${CMAKE_INSTALL_PREFIX}/dlplan/lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/dlplan/lib
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/dlplan/include
)
