add_library(dlplanserialization STATIC)
target_link_libraries(dlplanserialization
    PRIVATE
        Boost::serialization
    PUBLIC
        dlplan::novelty
        dlplan::statespace
        dlplan::novelty
        dlplan::policy)

# Create an alias for simpler reference
add_library(dlplan::serialization ALIAS dlplanserialization)
# Export component with simple name
set_property(TARGET dlplanserialization PROPERTY EXPORT_NAME serialization)

target_sources(dlplanserialization
    PRIVATE
        serialization.cpp)

# Use include depending on building or using from installed location
target_include_directories(dlplanserialization
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Install the target and create export-set
install(
    TARGETS dlplanserialization
    EXPORT dlplanserializationTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export file
install(EXPORT dlplanserializationTargets
    NAMESPACE dlplan::
    COMPONENT serialization
    FILE dlplanserializationTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dlplan"
)

# Generate build tree export file
export(EXPORT dlplanserializationTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/dlplanserializationTargets.cmake"
       NAMESPACE dlplan::
)
