set(CMAKE_CXX_FLAGS_INIT "-Wall -Werror -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_COVERAGE_INIT
    "-coverage -fno-exceptions -DWSS_DISABLE_ASSERTS"
)
set(CMAKE_CXX_FLAGS_TEST_INIT
    "-D_LIBCPP_DEBUG=1 -DNDEBUG -O3 -fsanitize=address,undefined"
)
