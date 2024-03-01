add_library(dlplanpolicy STATIC)

file(GLOB_RECURSE POLICY_SRC_FILES
    "*.cpp" "**/*.cpp")
file(GLOB_RECURSE POLICY_PRIVATE_HEADER_FILES
    "*.h" "**/*.h")
file(GLOB_RECURSE POLICY_PUBLIC_HEADER_FILES
    "../include/dlplan/policy.h"
    "../include/dlplan/policy/*.h" "../include/dlplan/policy/**/*.h")

target_sources(dlplanpolicy
    PRIVATE
        ${POLICY_SRC_FILES} ${POLICY_PRIVATE_HEADER_FILES} ${POLICY_PUBLIC_HEADER_FILES}
        ../common/parsers/utility.cpp
        ../common/parsers/filesystem.cpp)

target_link_libraries(dlplanpolicy
    PUBLIC
        dlplan::core)

# Create an alias for simpler reference
add_library(dlplan::policy ALIAS dlplanpolicy)
# Export component with simple name
set_property(TARGET dlplanpolicy PROPERTY EXPORT_NAME policy)

# Use include depending on building or using from installed location
target_include_directories(dlplanpolicy
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Install the target and create export-set
install(
    TARGETS dlplanpolicy
    EXPORT dlplanpolicyTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export file
install(EXPORT dlplanpolicyTargets
    NAMESPACE dlplan::
    COMPONENT policy
    FILE dlplanpolicyTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dlplan"
)

# Generate build tree export file
export(EXPORT dlplanpolicyTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/dlplan/dlplanpolicyTargets.cmake"
    NAMESPACE dlplan::
)
