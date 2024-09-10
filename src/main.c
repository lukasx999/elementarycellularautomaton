#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include <raylib.h>
#include <raymath.h>

#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 1920
#define CELL_WIDTH 5
#define CELL_HEIGHT CELL_WIDTH
// #define CELL_COLOR (Color) { 30, 160, 245, 255}
#define CELL_COLOR (Color) { 100, 100, 100, 255}

#define ARRAY_SIZE SCREEN_WIDTH / CELL_WIDTH

#define BUFSIZE 100
#define FONT_SIZE 50
#define FONT_FG RED
#define FONT_BG BLACK



// convert 8 bit integer to array of 8 bools
bool* str_to_binary(int num) {

    num = (uint8_t) num;

    // uint8_t num = (uint8_t) atoi(str);
    const size_t bitcount = 8;
    bool *bin = malloc(bitcount);


    for (size_t i=0; num != 0; ++i) {
        bin[i] = num % 2 && 1;
        num /= 2;
    }

    return bin;

}




bool apply_rules(int rule, bool a, bool b, bool c) {

    /*
    a | b | c -> z
    - | - | - -> -
    0 | 0 | 0 -> 0
    0 | 0 | 1 -> 0
    0 | 1 | 0 -> 0
    0 | 1 | 1 -> 1
    1 | 0 | 0 -> 1
    1 | 0 | 1 -> 1
    1 | 1 | 0 -> 1
    1 | 1 | 1 -> 0
    */

    size_t row;
    if (!a && !b && !c) row = 0;
    if (!a && !b &&  c) row = 1;
    if (!a &&  b && !c) row = 2;
    if (!a &&  b &&  c) row = 3;
    if ( a && !b && !c) row = 4;
    if ( a && !b &&  c) row = 5;
    if ( a &&  b && !c) row = 6;
    if ( a &&  b &&  c) row = 7;

    bool *bin = str_to_binary(rule);

    return bin[row];

}



bool * next_generation(int rule, bool *last_gen) {

    bool *new_gen = malloc(ARRAY_SIZE);
    memset(new_gen, 0, ARRAY_SIZE);


    for (size_t i=0; i<ARRAY_SIZE; ++i) {

        size_t prev, current, next;

        // special case for outer most cells -> wraparound

        if (i == 0) {
            prev = ARRAY_SIZE-1;
            current = i;
            next = i+1;

        } else if (i == ARRAY_SIZE-1) {
            prev = i-1;
            current = i;
            next = 0;

        } else {
            prev = i-1;
            current = i;
            next = i+1;
        }

        new_gen[i] = apply_rules(rule, last_gen[prev], last_gen[current], last_gen[next]);

    }

    return new_gen;

}





void render_row(uint32_t row, bool *generation) {

    for (size_t i=0; i<ARRAY_SIZE; ++i) {
        if (generation[i])
            DrawRectangle(i * CELL_WIDTH, CELL_HEIGHT*row, CELL_WIDTH, CELL_HEIGHT, CELL_COLOR);
    }

}



void increment_rule(void *arg) {

    int *rule = (int*) arg;

    ++*rule;
    if (*rule > 255)
        *rule = 0;

}


void timeout(float seconds, void(*callback)(void*), void *arg) {

    static float time = 0;

    float frametime = GetFrameTime();
    time += frametime;

    if (time > seconds) {
        callback(arg);
        time = 0;
    }

}



void render_rule_number(int rule) {

    char buf[BUFSIZE];
    sprintf(buf, "rule: %d", rule);
    DrawRectangle(0, 0, 220, FONT_SIZE+20, FONT_BG);
    DrawText(buf, 10, 10, FONT_SIZE, FONT_FG);

}



int main(void) {

    // const char *rule = "30";
    const char *rule_str = "110";
    int rule = atoi(rule_str);

    // starting point
    bool start_gen[ARRAY_SIZE] = { 0 }; // set everything to 0
    start_gen[ARRAY_SIZE/2] = 1; // set the middle one to 1



    SetTraceLogLevel(LOG_ERROR);
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Elementary Cellular Automaton");



    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(BLACK);

            uint32_t units = SCREEN_HEIGHT / CELL_HEIGHT;
            bool *next_gen, *current_gen = start_gen;

            for (uint32_t i=0; i<units; ++i) {

                next_gen = next_generation(rule, current_gen);
                render_row(i, next_gen);
                current_gen = next_gen;

            }

            render_rule_number(rule);

            // increment the current rule every second
            timeout(0.5, increment_rule, &rule);

        }
        EndDrawing();
    }

    CloseWindow();

    return EXIT_SUCCESS;
}
