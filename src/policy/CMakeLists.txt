add_library(dlplanpolicy SHARED)
target_sources(dlplanpolicy
    PRIVATE
        condition.cpp
        effect.cpp
        policy_builder.cpp
        policy_impl.cpp
        policy_minimizer.cpp
        policy.cpp
        reader.cpp
        rule.cpp
        writer.cpp
        parser/expression_factory.cpp
        parser/parser.cpp
        parser/utils.cpp)
target_link_libraries(dlplanpolicy dlplancore)
target_include_directories(dlplanpolicy
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/
)

if (APPLE)
    set_target_properties(dlplanpolicy PROPERTIES
        INSTALL_NAME_DIR "@loader_path"
    )
elseif (UNIX)
    set_target_properties(dlplanpolicy PROPERTIES
        INSTALL_RPATH "$ORIGIN"
    )
endif()

install(
    TARGETS
        dlplanpolicy
    LIBRARY DESTINATION
        lib/dlplan)

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include/dlplan/
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/dlplan)