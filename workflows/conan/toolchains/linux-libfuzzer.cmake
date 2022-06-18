include(ProcessorCount)
ProcessorCount(WSS_NUM_PROCESSORS)

set(WSS_FUZZER_FLAGS "-fsanitize=address,fuzzer,undefined;-DWSS_USE_LIBFUZZER")
set(WSS_FUZZER_LINKER_FLAGS "-fsanitize=fuzzer")
set(WSS_FUZZER_TEST_PARAMETERS -jobs=${WSS_NUM_PROCESSORS} -max_total_time=30)
