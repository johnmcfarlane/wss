set(CMAKE_CXX_FLAGS_COVERAGE_INIT
    "-coverage -fno-exceptions -DWSS_DISABLE_ASSERTS")
set(CMAKE_CXX_FLAGS_TEST_INIT
    "-Werror -Wall -Wextra -Wpedantic -Wno-maybe-uninitialized -Wno-restrict \
-fsanitize=address,undefined")
