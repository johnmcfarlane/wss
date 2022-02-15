set(CMAKE_CXX_CPPCHECK "")
set(CMAKE_CXX_CLANG_TIDY "")

# Convert the given list of words into C++ code and compile into a library
function(make_lexicon_library library_name wordlist)
  add_library(${library_name}
              ${CMAKE_BINARY_DIR}/src/lexicon/${library_name}.cpp)
  target_include_directories(
    ${library_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}
                           ${CMAKE_BINARY_DIR}/src/lexicon)
  set_target_properties(${library_name} PROPERTIES CXX_STANDARD 20)
  target_link_libraries(${library_name} common)
  add_dependencies(${library_name} generate)

  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/src/lexicon/${library_name}.cpp
           ${CMAKE_BINARY_DIR}/src/lexicon/${library_name}.h
    DEPENDS ${PROJECT_SOURCE_DIR}/src/lexicon/node.h ${wordlist}
            ${CMAKE_BINARY_DIR}/src/generate/generate
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src
    COMMAND
      mkdir -p lexicon && cd lexicon &&
      ${CMAKE_BINARY_DIR}/src/generate/generate ${wordlist} ${library_name}
      ${library_name}
    COMMENT "Building ${library_name}")
endfunction(make_lexicon_library)

make_lexicon_library(scrabble_lexicon ${PROJECT_SOURCE_DIR}/words/scrabble.txt)
make_lexicon_library(all_lexicon ${PROJECT_SOURCE_DIR}/words/all.txt)
make_lexicon_library(wwf_lexicon ${PROJECT_SOURCE_DIR}/words/wwf.txt)
