add_library(dlplanweisfeilerlehman SHARED)
target_sources(dlplanweisfeilerlehman
    PRIVATE
        color.cpp
        weisfeiler_lehman.cpp)
target_link_libraries(dlplanweisfeilerlehman dlplancore dlplanstatespace)
target_include_directories(dlplanweisfeilerlehman
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

install(
    TARGETS
    dlplanweisfeilerlehman
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_PREFIX}/lib/dlplan)

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include/dlplan/
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/dlplan)
