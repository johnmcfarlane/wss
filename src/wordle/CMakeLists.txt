find_package(fmt REQUIRED)
find_package(lyra REQUIRED)
find_package(Microsoft.GSL REQUIRED)

add_executable(wordle input.cpp main.cpp run.cpp)
target_link_libraries(wordle PRIVATE common fmt::fmt Microsoft.GSL::GSL
                                     bfg::lyra wordle_lexicon)

add_test(NAME wordle COMMAND wordle TRIAL21000,THROW22110)
add_test(NAME wordle-debug COMMAND wordle TRIAL21000,THROW22110 --debug)
