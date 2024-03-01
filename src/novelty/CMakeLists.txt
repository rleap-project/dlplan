add_library(dlplannovelty STATIC)

file(GLOB_RECURSE NOVELTY_SRC_FILES
    "*.cpp" "**/*.cpp")
file(GLOB_RECURSE NOVELTY_PRIVATE_HEADER_FILES
    "*.h" "**/*.h")
file(GLOB_RECURSE NOVELTY_PUBLIC_HEADER_FILES
    "../include/dlplan/novelty.h"
    "../include/dlplan/novelty/*.h" "../include/dlplan/novelty/**/*.h")

target_sources(dlplannovelty
    PRIVATE
        ${NOVELTY_SRC_FILES} ${NOVELTY_PRIVATE_HEADER_FILES} ${NOVELTY_PUBLIC_HEADER_FILES}
        ../utils/math.cpp
    )
target_link_libraries(dlplannovelty
    PUBLIC
        dlplan::core
        dlplan::statespace)

# Create an alias for simpler reference
add_library(dlplan::novelty ALIAS dlplannovelty)
# Export component with simple name
set_property(TARGET dlplannovelty PROPERTY EXPORT_NAME novelty)

# Use include depending on building or using from installed location
target_include_directories(dlplannovelty
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Install the target and create export-set
install(
    TARGETS dlplannovelty
    EXPORT dlplannoveltyTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export file
install(EXPORT dlplannoveltyTargets
    NAMESPACE dlplan::
    COMPONENT novelty
    FILE dlplannoveltyTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dlplan"
)

# Generate build tree export file
export(EXPORT dlplannoveltyTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/dlplangeneratorTargets.cmake"
       NAMESPACE dlplan::
)
