find_package(Catch2 REQUIRED)

add_library(unit-test unit-test.cpp)
target_link_libraries(unit-test PUBLIC Catch2::Catch2)

# add unit test executable target and test
function(add_unit_test name source_file)
  add_executable(${name} ${source_file})
  target_link_libraries(${name} PRIVATE ${ARGN} unit-test)
  add_test(${name} ${name})
endfunction()
