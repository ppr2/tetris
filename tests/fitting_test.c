#include "fitting_test.h"
#include "../fitting.h"
#include "../state.h"
#include <stdio.h>
#include <stdlib.h>

#define _DEBUG 1
#define _WIDTH 5
#define _HEIGHT 5
#define _INDEX_MAX _WIDTH * _HEIGHT
const int WIDTH = _WIDTH;
const int HEIGHT = _HEIGHT;
const int DEBUG = _DEBUG;
const int INDEX_MAX = _INDEX_MAX;

char ** map;

int main() {
    int x = 0;
    int y = 0;
    int index = x + y*WIDTH;
    initMap();

    State state;
    state.index = index;
    state.shape = STICK2;
    State * p_state = &state;
    fit(p_state, EL1);
    if (fitable(STICK1, 4)) {
        printf("fitable!\n");
        state.index = 4;
        state.shape = STICK1;
        p_state = &state;
        fit(p_state, STICK1);
    }
    //printf("fitable? %d\n", fitable(STICK1, 4));

    freeMap();

    return 0;
}

inline void initMap() {
    int i;
    // Initialize map
    map = (char **) calloc(WIDTH, WIDTH * sizeof(char *));
    for (i = 0; i < WIDTH; i++) {
        map[i] = (char *) calloc(HEIGHT, HEIGHT * sizeof(char));
    }
}

inline void freeMap() {
    int i;

    for (i = 0; i < WIDTH; i++){
        free(map[i]);
    }
    free(map);
}