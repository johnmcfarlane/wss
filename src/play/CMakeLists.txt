add_executable(play
        board_premiums.cpp
        board_tiles.cpp
        grid.cpp
        load_buffer.cpp
        main.cpp
        solve.cpp
)
target_link_libraries(play common lexicon)
