set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE
    "iwyu;\
-Xiwyu;--cxx17ns;\
-Xiwyu;--mapping_file=${CMAKE_CURRENT_LIST_DIR}/linux-iwyu.cmake.imp;\
-Xiwyu;--quoted_includes_first")
