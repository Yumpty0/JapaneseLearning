#include <pthread.h>
#include <stdio.h>

#include "game_manager.h"
#include "input_handler.h"
#include "utils.h"

#define INPUT_BUFFER_SIZE 50

pthread_t input_thread;
pthread_t render_thread;

int running = 1, should_rerender = 0;

void stop(char* reason) {
    running = 0;
    system("cls");
    printf("Stopping process !\n");
    printf("Given reason: %s\n", reason);
    system("pause");
    exit(0);
}

void render_input() {
    int amount_of_space = (int) (get_terminal_width() / 2) - (get_buffer_active_length() / 2) - 3;
    char* input = get_buffer();
    if (input == 0) {
        input = "";
    }
    move_cursor_to_row(get_terminal_height() / 10 * 7);
    printf("\r%s\r", repeat(get_terminal_width(), " "));
    printf("%s->%s<-", repeat(amount_of_space, " "), input);
}

void update_render() {
    should_rerender = 1;
}

void* render_thread_function(void* args) {
    free(args);
    while (running) {
        if (!should_rerender) {
            continue;
        } else {
            should_rerender = 0;
        }
        system("cls");

        printf("\033[?25l");
        printf("\033[H");

        render_input();

        printf("\033[?25h");
    }

    return 0;
}

void prepare_terminal() {
    system("chcp 65001");
    system("cls");
}

void* input_thread_function(void* args) {
    free(args);
    while (running) {
        validate_buffer();
        char c = ask_input();

        if (c == ENTER_KEYCODE) {
            clear_buffer();
        } else if (c == BACKSPACE_KEYCODE) {
            pop_buffer();
        } else {
            append_to_buffer(&c);
        }

        update_render();

        if (equals_string(get_buffer(), "exit", 4)) {
            stop("Input 'exit'.");
            break;
        }
    }

    pthread_exit(0);
    return 0;
}

void game_load() {
    int is_valid = 0;
    load_game_object(&is_valid);
    if (!is_valid) {
        stop("Error on loading game object !");
    }
    system("pause");
}

int main(void) {
    game_load();
    prepare_terminal();
    load_buffer(INPUT_BUFFER_SIZE);

    pthread_create(&input_thread, NULL, input_thread_function, NULL);
    pthread_create(&render_thread, NULL, render_thread_function, NULL);

    while (running) {
    }

    return 0;
}
