add_library(common
        letter_set.cpp
        letter_set.h
        scores.h
        ssize.h
        tile.h
)
set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(common PUBLIC cxx_std_17)
target_link_libraries(common "${CONAN_LIBS}")
target_include_directories(common INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
