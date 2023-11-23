add_library(dlplancore STATIC)
target_sources(dlplancore
    PRIVATE
        atom.cpp
        concept_denotation.cpp
        role_denotation.cpp
        state.cpp
        constant.cpp
        core.cpp
        denotations_caches.cpp
        element_factory.cpp
        elements/booleans/nullary.cpp
        elements/concepts/all.cpp
        elements/concepts/and.cpp
        elements/concepts/bot.cpp
        elements/concepts/diff.cpp
        elements/concepts/equal.cpp
        elements/concepts/not.cpp
        elements/concepts/one_of.cpp
        elements/concepts/or.cpp
        elements/concepts/primitive.cpp
        elements/concepts/projection.cpp
        elements/concepts/some.cpp
        elements/concepts/subset.cpp
        elements/concepts/top.cpp
        elements/numericals/concept_distance.cpp
        elements/numericals/role_distance.cpp
        elements/numericals/sum_concept_distance.cpp
        elements/numericals/sum_role_distance.cpp
        elements/roles/and.cpp
        elements/roles/compose.cpp
        elements/roles/diff.cpp
        elements/roles/identity.cpp
        elements/roles/inverse.cpp
        elements/roles/not.cpp
        elements/roles/or.cpp
        elements/roles/primitive.cpp
        elements/roles/restrict.cpp
        elements/roles/top.cpp
        elements/roles/transitive_closure.cpp
        elements/roles/transitive_reflexive_closure.cpp
        instance_info.cpp
        vocabulary_info.cpp
        predicate.cpp
        object.cpp
        parsers/syntactic/parser_instantiations.cpp
        parsers/semantic/parser.cpp
        elements/utils.cpp
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