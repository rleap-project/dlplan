add_library(dlplanserialization SHARED)

target_link_libraries(dlplanserialization
    PUBLIC
        dlplan::statespace
        Boost::serialization)

# Create an alias for simpler reference
add_library(dlplan::serialization ALIAS dlplanserialization)

target_sources(dlplanserialization
    PRIVATE
        serialization.cpp)

# Use include depending on building or using from installed location
target_include_directories(dlplanserialization
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Add current directory for library to be relocatable
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set_target_properties(dlplanserialization PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_target_properties(dlplanserialization PROPERTIES INSTALL_RPATH "@loader_path")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message( FATAL_ERROR "Windows is not supported." )
endif()
set_target_properties(dlplanserialization PROPERTIES BUILD_WITH_INSTALL_RPATH true)

# Install the target and create export-set
install(
    TARGETS dlplanserialization
    EXPORT dlplanserializationTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install header file
install(
    FILES "${PROJECT_SOURCE_DIR}/include/dlplan/serialization.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/dlplan"
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
