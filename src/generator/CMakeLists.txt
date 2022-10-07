add_library(dlplangenerator SHARED)

target_sources(dlplangenerator
    PRIVATE
        feature_generator.cpp
        generator.cpp
        rules/booleans/empty.cpp
        rules/booleans/inclusion.cpp
        rules/booleans/nullary.cpp
        rules/numericals/concept_distance.cpp
        rules/numericals/count.cpp
        rules/concepts/all.cpp
        rules/concepts/and.cpp
        rules/concepts/bot.cpp
        rules/concepts/diff.cpp
        rules/concepts/equal.cpp
        rules/concepts/not.cpp
        rules/concepts/one_of.cpp
        rules/concepts/or.cpp
        rules/concepts/primitive.cpp
        rules/concepts/projection.cpp
        rules/concepts/some.cpp
        rules/concepts/subset.cpp
        rules/concepts/top.cpp
        rules/roles/and.cpp
        rules/roles/compose.cpp
        rules/roles/diff.cpp
        rules/roles/identity.cpp
        rules/roles/inverse.cpp
        rules/roles/not.cpp
        rules/roles/or.cpp
        rules/roles/primitive.cpp
        rules/roles/restrict.cpp
        rules/roles/top.cpp
        rules/roles/transitive_closure.cpp
        rules/roles/transitive_reflexive_closure.cpp
        ../utils/logging.cpp
        ../utils/countdown_timer.cpp
)
target_link_libraries(dlplangenerator dlplancore pthread)
target_include_directories(dlplangenerator
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

install(
    TARGETS
        dlplangenerator
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_PREFIX}/lib/dlplan)

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include/dlplan/
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/dlplan)