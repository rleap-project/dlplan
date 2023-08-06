add_library(dlplancore SHARED)
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
        instance_info.cpp
        numerical.cpp
        vocabulary_info.cpp
        predicate.cpp
        object.cpp
        parser/parser.cpp
        parser/expression_factory.cpp
        elements/utils.cpp
        ../utils/logging.cpp
        ../utils/MurmurHash3.cpp
        ../utils/system.cpp
        ../utils/timer.cpp
        ../utils/hash.cpp)

# Create an alias for simpler reference
add_library(dlplan::core ALIAS dlplancore)

# Use include depending on building or using from installed location
target_include_directories(dlplancore
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Add current directory for library to be relocatable (for python api)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set_target_properties(dlplancore PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_target_properties(dlplancore PROPERTIES INSTALL_NAME_DIR "@loader_path")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message( FATAL_ERROR "Windows is not supported." )
endif()

# Install the target and create export-set
install(
    TARGETS dlplancore
    EXPORT dlplancoreTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install header file
install(
    FILES "${PROJECT_SOURCE_DIR}/include/dlplan/core.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/dlplan"
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