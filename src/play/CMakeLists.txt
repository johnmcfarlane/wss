add_executable(play
        board_premiums.cpp
        board_tiles.cpp
        csv.cpp
        load_buffer.cpp
        main.cpp)
target_link_libraries(play common lexicon)
