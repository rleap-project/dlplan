add_library(dlplanpolicy STATIC)
target_sources(dlplanpolicy
    PRIVATE
        condition.cpp
        effect.cpp
        policy_factory.cpp
        policy_impl.cpp
        policy_minimizer.cpp
        policy.cpp
        rule.cpp
        parsers/policy/stage_1/parser_instantiations.cpp
        parsers/policy/stage_2/context.cpp
        parsers/policy/stage_2/parser.cpp
        ../common/parsers/utility.cpp
        ../common/parsers/filesystem.cpp)

target_link_libraries(dlplanpolicy
    PRIVATE
        Boost::serialization
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
