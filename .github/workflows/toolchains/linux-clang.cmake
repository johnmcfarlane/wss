set(CMAKE_CXX_FLAGS_COVERAGE_INIT
    "-coverage -fno-exceptions -DWSS_DISABLE_ASSERTS")
set(CMAKE_CXX_FLAGS_TEST_INIT
    "-DNDEBUG -O3 -Wall -Werror -Wextra -Wpedantic -fsanitize=address,undefined"
)
