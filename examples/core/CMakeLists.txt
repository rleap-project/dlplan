add_executable(simple simple.cpp)
target_link_libraries(simple dlp)

add_executable(generator generator.cpp)
target_link_libraries(generator dlp)
