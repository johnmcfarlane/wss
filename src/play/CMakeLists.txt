add_executable(play
        board_premiums.cpp
        board_tiles.cpp
        grid.cpp
        letter_set.cpp
        load_buffer.cpp
        main.cpp
        search.cpp
        solve.cpp
)
target_link_libraries(play common lexicon)
