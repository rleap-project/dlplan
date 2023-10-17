add_library(dlplancore STATIC)
target_sources(dlplancore
    PRIVATE
        atom.cpp
        base_element.cpp
        boolean.cpp
        concept_denotation.cpp
        concept.cpp
        role_denotation.cpp
        role.cpp
        state.cpp
        constant.cpp
        core.cpp
        denotations_caches.cpp
        element_factory.cpp
        elements/roles/primitive.cpp
        instance_info.cpp
        numerical.cpp
        vocabulary_info.cpp
        predicate.cpp
        object.cpp
        parsers/elements/stage_1/parser_instantiations.cpp
        parsers/elements/stage_2/parser.cpp
        elements/utils.cpp
        ../utils/logging.cpp
        ../utils/MurmurHash3.cpp
        ../utils/system.cpp
        ../utils/timer.cpp
        ../utils/hash.cpp
        ../common/parsers/utility.cpp
        ../common/parsers/filesystem.cpp)
target_link_libraries(dlplancore
    PRIVATE
        Boost::serialization)

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