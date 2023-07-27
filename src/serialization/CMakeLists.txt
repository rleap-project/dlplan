add_library(dlplanserialization SHARED)
target_link_libraries(dlplanserialization dlplanstatespace Boost::serialization)
target_sources(dlplanserialization
    PRIVATE
        serialization.cpp)
target_include_directories(dlplanserialization
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

install(
    TARGETS
    dlplanserialization
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_PREFIX}/lib/dlplan)

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include/dlplan/
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/dlplan)