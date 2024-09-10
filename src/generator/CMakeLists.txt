add_library(dlplangenerator STATIC)

file(GLOB_RECURSE GENERATOR_SRC_FILES
    "*.cpp" "**/*.cpp")
file(GLOB_RECURSE GENERATOR_PRIVATE_HEADER_FILES
    "*.h" "**/*.h")
file(GLOB_RECURSE GENERATOR_PUBLIC_HEADER_FILES
    "../include/dlplan/generator.h"
    "../include/dlplan/generator/*.h" "../include/dlplan/generator/**/*.h")

target_sources(dlplangenerator
    PRIVATE
        ${GENERATOR_SRC_FILES} ${GENERATOR_PRIVATE_HEADER_FILES} ${GENERATOR_PUBLIC_HEADER_FILES}
        "../utils/logging.h" "../utils/logging.cpp"
        "../utils/countdown_timer.h" "../utils/countdown_timer.cpp"
)
target_link_libraries(dlplangenerator
    PUBLIC
        dlplan::core)

# Create an alias for simpler reference
add_library(dlplan::generator ALIAS dlplangenerator)
# Export component with simple name
set_property(TARGET dlplangenerator PROPERTY EXPORT_NAME generator)

# Use include depending on building or using from installed location
target_include_directories(dlplangenerator
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Install the target and create export-set
install(
    TARGETS dlplangenerator
    EXPORT dlplangeneratorTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export file
install(EXPORT dlplangeneratorTargets
    NAMESPACE dlplan::
    COMPONENT generator
    FILE dlplangeneratorTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dlplan"
)

# Generate build tree export file
export(EXPORT dlplangeneratorTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/dlplangeneratorTargets.cmake"
       NAMESPACE dlplan::
)
