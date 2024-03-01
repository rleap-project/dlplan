find_package(pybind11 CONFIG REQUIRED PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)
# Set result variables
find_package(pybind11)

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
