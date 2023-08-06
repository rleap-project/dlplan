add_library(dlplanstatespace SHARED)
target_sources(dlplanstatespace
    PRIVATE
        generator.cpp
        reader.cpp
        state_space.cpp)

target_link_libraries(dlplanstatespace
    PUBLIC
        dlplan::core)

# Create an alias for simpler reference
add_library(dlplan::statespace ALIAS dlplanstatespace)

# Use include depending on building or using from installed location
target_include_directories(dlplanstatespace
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Add current directory for library to be relocatable
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set_target_properties(dlplanstatespace PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_target_properties(dlplanstatespace PROPERTIES INSTALL_RPATH "@loader_path")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message( FATAL_ERROR "Windows is not supported." )
endif()
set_target_properties(dlplanstatespace PROPERTIES BUILD_WITH_INSTALL_RPATH true)

# Install the target and create export-set
install(
    TARGETS dlplanstatespace
    EXPORT dlplanstatespaceTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install header file
install(
    FILES "${PROJECT_SOURCE_DIR}/include/dlplan/state_space.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/dlplan"
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
