add_executable(
    serialization_tests
)
target_sources(
    serialization_tests
    PRIVATE
        serialization.cpp
)
target_link_libraries(serialization_tests
    PRIVATE
        dlplanserialization
        dlplanstatespace
        Boost::serialization
        GTest::GTest)

add_test(serialization_gtests serialization_tests)

add_subdirectory(gripper)
