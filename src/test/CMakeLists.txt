find_package(Catch2 REQUIRED)

add_library(unit-test unit-test.cpp)
target_link_libraries(unit-test PUBLIC Catch2::Catch2)
