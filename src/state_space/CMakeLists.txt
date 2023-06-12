add_library(dlplanstatespace SHARED)
target_sources(dlplanstatespace
    PRIVATE
        generator.cpp
        reader.cpp
        state_space.cpp)

target_link_libraries(dlplanstatespace dlplancore)
target_include_directories(dlplanstatespace
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

if (APPLE)
    set_target_properties(dlplanstatespace PROPERTIES
        INSTALL_NAME_DIR "@loader_path"
    )
elseif (UNIX)
    set_target_properties(dlplanstatespace PROPERTIES
        INSTALL_RPATH "$ORIGIN"
    )
endif()

install(
    TARGETS
        dlplanstatespace
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_PREFIX}/lib/dlplan)

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include/dlplan/
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/dlplan)