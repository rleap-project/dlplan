add_library(dlplancore SHARED)
target_sources(dlplancore
    PRIVATE
        atom.cpp
        state.cpp
        constant.cpp
        core.cpp
        element_factory.cpp
        instance_info.cpp
        vocabulary_info.cpp
        predicate.cpp
        object.cpp
        parser/parser.cpp
        parser/expression_factory.cpp
        elements/utils.cpp
        ../utils/logging.cpp
        ../utils/MurmurHash3.cpp
        ../utils/system.cpp
        ../utils/timer.cpp)
target_include_directories(dlplancore
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

install(
    TARGETS
        dlplancore
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_PREFIX}/lib/dlplan)

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include/dlplan/
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/dlplan)