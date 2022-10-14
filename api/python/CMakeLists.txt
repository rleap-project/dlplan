
# We capture the output of pybind11.get_cmake_dir() to know where the pybind11
# libraries are located. This of course requires that the Python pybind11 module
# is installed in the Python environment where ${PYTHON_EXECUTABLE} lives, but
# the setup.py and pyproject.toml files will take care of that.
execute_process(
        COMMAND
        "${PYTHON_EXECUTABLE}" -c "import pybind11; print(pybind11.get_cmake_dir())"
        OUTPUT_VARIABLE _tmp_dir
        OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT)
list(APPEND CMAKE_PREFIX_PATH "${_tmp_dir}")

# Now we can find pybind11
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_dlplan MODULE)
target_sources(_dlplan
    PRIVATE
        src/main.cpp
        src/core.cpp
        src/generator.cpp
        src/novelty.cpp
        src/policy.cpp
        src/state_space.cpp
        src/weisfeiler_lehman.cpp)
target_link_libraries(_dlplan
    PRIVATE
        pybind11::module
        dlplancore
        dlplangenerator
        dlplannovelty
        dlplanpolicy
        dlplanstatespace
        dlplanweisfeilerlehman)
target_compile_definitions(_dlplan PUBLIC DLPLAN_VERSION_INFO="${DLPLAN_VERSION_INFO}")

# This would copy the generated dynamic library into a given path, but
# at the moment this logic is performed by the setup script
#add_custom_command(TARGET _dlplan POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:_dlplan> "${CMAKE_CURRENT_SOURCE_DIR}/src/"
#)