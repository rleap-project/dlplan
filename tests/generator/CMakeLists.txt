add_executable(
    generator_tests
)
target_sources(
    generator_tests
    PRIVATE
        generator.cpp
)
target_link_libraries(generator_tests dlplancore dlplangenerator gtest_main)
gtest_discover_tests(generator_tests)