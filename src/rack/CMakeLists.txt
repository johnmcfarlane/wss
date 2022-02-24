find_package(fmt REQUIRED)
find_package(lyra REQUIRED)

add_executable(rack main.cpp)
target_link_libraries(rack PRIVATE bfg::lyra common fmt::fmt wwf_lexicon)
set_target_properties(rack PROPERTIES CXX_STANDARD 20)

add_test(NAME rack COMMAND rack abc)
