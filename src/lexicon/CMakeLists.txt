add_library(lexicon
        ${CMAKE_BINARY_DIR}/src/lexicon/lexicon.cpp)
target_include_directories(lexicon PUBLIC
        ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_BINARY_DIR}/src/lexicon)
target_link_libraries(lexicon common)
add_dependencies(lexicon generate)
target_compile_options(lexicon PRIVATE ${WSS_LEXICON_COMPILE_OPTIONS})

add_custom_command(
    COMMENT "Building lexicons"
    DEPENDS
        ${PROJECT_SOURCE_DIR}/src/lexicon/node.h
        ${PROJECT_SOURCE_DIR}/words/scrabble.txt
        ${PROJECT_SOURCE_DIR}/words/wwf.txt
        ${CMAKE_BINARY_DIR}/src/generate/generate
    OUTPUT
        ${CMAKE_BINARY_DIR}/src/lexicon/lexicon.cpp
        ${CMAKE_BINARY_DIR}/src/lexicon/lexicon.h
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src
    COMMAND
        mkdir -p lexicon &&
        cd lexicon &&
        ${CMAKE_BINARY_DIR}/src/generate/generate
            ${PROJECT_SOURCE_DIR}/words/scrabble.txt scrabble_lexicon
            ${PROJECT_SOURCE_DIR}/words/wwf.txt wwf_lexicon
            lexicon
)
