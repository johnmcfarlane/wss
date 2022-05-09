set(CMAKE_CXX_FLAGS_INIT
    "-Wall -Werror -Wextra -Wno-maybe-uninitialized -Wno-restrict -pedantic"
)
set(CMAKE_CXX_FLAGS_COVERAGE_INIT
    "-coverage -fno-exceptions -DWSS_DISABLE_ASSERTS"
)
set(CMAKE_CXX_FLAGS_TEST_INIT
    "-D_GLIBCXX_ASSERTIONS -DNDEBUG -O3 -fsanitize=address,undefined"
)
