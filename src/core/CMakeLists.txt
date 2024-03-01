add_library(dlplancore STATIC)

file(GLOB_RECURSE CORE_SRC_FILES
    "*.cpp" "**/*.cpp")
file(GLOB_RECURSE CORE_PRIVATE_HEADER_FILES
    "*.h" "**/*.h")
file(GLOB_RECURSE CORE_PUBLIC_HEADER_FILES
    "../include/dlplan/core.h"
    "../include/dlplan/core/*.h" "../include/dlplan/core/**/*.h")

target_sources(dlplancore
    PRIVATE ${CORE_SRC_FILES} ${CORE_PRIVATE_HEADER_FILES} ${CORE_PUBLIC_HEADER_FILES}
        ../utils/logging.cpp
        ../utils/MurmurHash3.cpp
        ../utils/system.cpp
        ../utils/timer.cpp
        ../common/parsers/utility.cpp
        ../common/parsers/filesystem.cpp)

# Create an alias for simpler reference
add_library(dlplan::core ALIAS dlplancore)
# Export component with simple name
set_property(TARGET dlplancore PROPERTY EXPORT_NAME core)

# Use include depending on building or using from installed location
target_include_directories(dlplancore
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Install the target and create export-set
install(
    TARGETS dlplancore
    EXPORT dlplancoreTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export file
install(EXPORT dlplancoreTargets
    NAMESPACE dlplan::
    COMPONENT core
    FILE dlplancoreTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dlplan"
)

# Generate build tree export file
export(EXPORT dlplancoreTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/dlplancoreTargets.cmake"
       NAMESPACE dlplan::
)