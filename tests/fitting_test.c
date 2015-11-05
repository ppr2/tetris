#include "fitting_test.h"
#include "../fitting.h"
#include "../state.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _DEBUG 1
#define _WIDTH 3
#define _HEIGHT 3
#define _INDEX_MAX _WIDTH * _HEIGHT
const int WIDTH = _WIDTH;
const int HEIGHT = _HEIGHT;
const int DEBUG = _DEBUG;
const int INDEX_MAX = _INDEX_MAX;

char ** map;
char ** newMap();
void freeMap(char **);
void copyMap(char ** dest, char ** source);

int main() {
    char map_val[3][3] = {
            {1, 1, 1},
            {1, 1, 1},
            {1, 1, 0}
    };
    map = newMap();
/*    for(int i = 0; i < WIDTH; i++) {
        map[i] = &map_val[i];
    }*/

    int i;

    for (i = 0; i < WIDTH; i++) {
        memcpy(map[i], map_val[i], HEIGHT);
    }
    printMap(map);
    State state;
    state.index = 8;
    state.shape = SPACE;
    printf("fitable? %d\n", fitable(SPACE, 8));

    fit(&state, EL2);
    printMap(map);
    /* State * p_state = &state;
     fit(p_state, EL1);
     if (fitable(STICK1, 4)) {
         printf("fitable!\n");
         state.index = 4;
         state.shape = STICK1;
         p_state = &state;
         fit(p_state, STICK1);
     }*/
    //printf("fitable? %d\n", fitable(STICK1, 4));


    return 0;
}

char ** newMap() {
    int i;
    char ** newMap;
    // Initialize map
    newMap = (char **) calloc(WIDTH, WIDTH * sizeof(char *));
    for (i = 0; i < WIDTH; i++) {
        newMap[i] = (char *) calloc(HEIGHT, HEIGHT * sizeof(char));
    }

    return newMap;
}

void freeMap(char ** someMap) {
    int i;

    if (someMap != NULL) {
        for (i = 0; i < WIDTH; i++){
            if (someMap[i] != NULL) {
                free(someMap[i]);
            }
        }
        free(someMap);
    }
}