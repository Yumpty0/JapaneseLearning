cmake_minimum_required(VERSION 3.28)
project(JapaneseLearning C)

set(CMAKE_C_STANDARD 11)

add_executable(JapaneseLearning # The name of the executable that's is build.
        main.c # The main class where the magic append :D.
        input_handler.c input_handler.h # The handler that will process the input of the user.
        utils.c utils.h # Only some utils terminal, and so on... .
        game_manager.c game_manager.h # The game manager that load every game input.
        string_utils.c string_utils.h # Utils for string management.
        math_utils.c math_utils.h # Utils for math.
)
