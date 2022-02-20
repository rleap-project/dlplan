add_library(dlplanevaluator SHARED)
target_sources(dlplanevaluator
    PRIVATE
        evaluator.cpp
        ../utils/per_state_bitset.cpp)
target_link_libraries(dlplanevaluator dlplancore)

install(
    TARGETS
        dlplanevaluator
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_PREFIX}/lib/dlplan)

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include)