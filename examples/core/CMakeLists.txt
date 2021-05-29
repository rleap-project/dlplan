add_executable(simple simple.cpp)
target_link_libraries(simple core)

add_executable(generator generator.cpp)
target_link_libraries(generator core)
