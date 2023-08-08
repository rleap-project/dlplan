find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_dlplan MODULE)
target_sources(_dlplan
    PRIVATE
        src/main.cpp
        src/core.cpp
        src/generator.cpp
        src/novelty.cpp
        src/policy.cpp
        src/serialization.cpp
        src/state_space.cpp)
target_link_libraries(_dlplan
    PRIVATE
        pybind11::module
        dlplancore
        dlplangenerator
        dlplannovelty
        dlplanpolicy
        dlplanserialization
        dlplanstatespace)
target_compile_definitions(_dlplan PUBLIC DLPLAN_VERSION_INFO="${DLPLAN_VERSION_INFO}")
