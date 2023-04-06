find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_dlplan MODULE)
target_sources(_dlplan
    PRIVATE
        src/main.cpp
        src/core.cpp
        src/generator.cpp
        src/novelty.cpp
        src/policy.cpp
        src/state_space.cpp)
target_link_libraries(_dlplan
    PRIVATE
        pybind11::module
        dlplancore
        dlplangenerator
        dlplannovelty
        dlplanpolicy
        dlplanstatespace)
target_compile_definitions(_dlplan PUBLIC DLPLAN_VERSION_INFO="${DLPLAN_VERSION_INFO}")

# In case of moving the library (as done in the setup.py),
# we need the library to find the component libraries in origin directory.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set_target_properties(_dlplan PROPERTIES INSTALL_RPATH $ORIGIN)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_target_properties(_dlplan PROPERTIES INSTALL_RPATH @loader_path)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message( FATAL_ERROR "Windows is not supported." )
endif()
set_target_properties(_dlplan PROPERTIES BUILD_WITH_INSTALL_RPATH true)
