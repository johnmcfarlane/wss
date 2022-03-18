# Nearly all source in this directory is in huge, auto-generated files. Static
# analysis is of less utility here and takes much longer too.
set(CMAKE_CXX_CLANG_TIDY "")
set(CMAKE_CXX_CPPCHECK "")
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "")

# Convert the given list of words into C++ code and compile into a library
function(make_lexicon_library library_name wordlist)
  add_library(${library_name}
              ${CMAKE_BINARY_DIR}/src/lexicon/${library_name}.cpp)
  target_include_directories(
    ${library_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}
                           ${CMAKE_BINARY_DIR}/src/lexicon)
  target_link_libraries(${library_name} PRIVATE common)
  add_dependencies(${library_name} generate)

  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/src/lexicon/${library_name}.cpp
           ${CMAKE_BINARY_DIR}/src/lexicon/${library_name}.h
    DEPENDS ${PROJECT_SOURCE_DIR}/src/lexicon/node.h ${wordlist}
            $<TARGET_FILE:generate>
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src
    COMMAND ${CMAKE_COMMAND} -E make_directory lexicon
    COMMAND ${CMAKE_COMMAND} -E chdir lexicon $<TARGET_FILE:generate>
            ${wordlist} ${library_name} ${library_name}
    COMMENT "Building ${library_name}")
endfunction(make_lexicon_library)

make_lexicon_library(all_lexicon ${PROJECT_SOURCE_DIR}/words/all.txt)
make_lexicon_library(scrabble_lexicon ${PROJECT_SOURCE_DIR}/words/scrabble.txt)
make_lexicon_library(wordle_lexicon ${PROJECT_SOURCE_DIR}/words/wordle.txt)
make_lexicon_library(wwf_lexicon ${PROJECT_SOURCE_DIR}/words/wwf.txt)
