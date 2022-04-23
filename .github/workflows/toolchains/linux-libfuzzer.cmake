include(ProcessorCount)
ProcessorCount(WSS_NUM_PROCESSORS)

set(CMAKE_CXX_FLAGS_INIT
    "-Wall -Werror -Wextra -fsanitize=address,undefined -g -pedantic")
set(WSS_FUZZER_FLAGS "-fsanitize=address,fuzzer,undefined;-DWSS_USE_LIBFUZZER")
set(WSS_FUZZER_LINKER_FLAGS "-fsanitize=fuzzer")
set(WSS_FUZZER_TEST_PARAMETERS
    -jobs=${WSS_NUM_PROCESSORS} -workers=${WSS_NUM_PROCESSORS}
    -max_total_time=60)
