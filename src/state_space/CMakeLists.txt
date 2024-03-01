add_library(dlplanstatespace STATIC)

file(GLOB_RECURSE STATE_SPACE_SRC_FILES
    "*.cpp" "**/*.cpp")
file(GLOB_RECURSE STATE_SPACE_PRIVATE_HEADER_FILES
    "*.h" "**/*.h")
file(GLOB_RECURSE STATE_SPACE_PUBLIC_HEADER_FILES
    "../include/dlplan/state_space.h"
    "../include/dlplan/state_space/*.h" "../include/dlplan/state_space/**/*.h")

target_sources(dlplanstatespace
    PRIVATE
        ${STATE_SPACE_SRC_FILES} ${STATE_SPACE_PRIVATE_HEADER_FILES} ${STATE_SPACE_PUBLIC_HEADER_FILES}
)

target_link_libraries(dlplanstatespace
    PUBLIC
        dlplan::core)

# Create an alias for simpler reference
add_library(dlplan::statespace ALIAS dlplanstatespace)
# Export component with simple name
set_property(TARGET dlplanstatespace PROPERTY EXPORT_NAME statespace)

# Use include depending on building or using from installed location
target_include_directories(dlplanstatespace
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Install the target and create export-set
install(
    TARGETS dlplanstatespace
    EXPORT dlplanstatespaceTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export file
install(EXPORT dlplanstatespaceTargets
    NAMESPACE dlplan::
    COMPONENT statespace
    FILE dlplanstatespaceTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dlplan"
)

# Generate build tree export file
export(EXPORT dlplanstatespaceTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/dlplanstatespaceTargets.cmake"
       NAMESPACE dlplan::
)
