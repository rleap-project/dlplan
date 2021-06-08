cmake_minimum_required(VERSION 3.16.3)
project(dlp VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Uncomment for debug build. Alternatively, build with `cmake -DCMAKE_BUILD_TYPE=Debug .`
#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS "-g -DDEBUG")
#set(CMAKE_CXX_FLAGS "-O3 -DNDEBUG")  # -Wall -Wextra

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
  add_subdirectory(api/python)
endif()
