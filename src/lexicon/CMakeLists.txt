add_library(lexicon
        ${CMAKE_BINARY_DIR}/src/lexicon/wwf_lexicon.cpp)
target_include_directories(lexicon PUBLIC
        ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_BINARY_DIR}/src/lexicon)
target_link_libraries(lexicon common)
target_compile_options(lexicon PRIVATE ${WSS_LEXICON_COMPILE_OPTIONS})

add_custom_command(
    COMMENT "Building WWF lexicon"
    DEPENDS
        ${PROJECT_SOURCE_DIR}/words/wwf.txt
        ${CMAKE_BINARY_DIR}/src/generate/generate
    OUTPUT
        ${CMAKE_BINARY_DIR}/src/lexicon/wwf_lexicon.cpp
        ${CMAKE_BINARY_DIR}/src/lexicon/wwf_lexicon.h
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src
    COMMAND
        mkdir -p lexicon &&
        cd lexicon &&
        ${CMAKE_BINARY_DIR}/src/generate/generate
            ${PROJECT_SOURCE_DIR}/words/wwf.txt wwf_lexicon
)
