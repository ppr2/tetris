#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "fitting.h"
#include "state.h"



/************************************************
 * LOCAL FUNCTIONS
 ************************************************/
inline char ** newMap();
inline void freeMap(char **);
inline void copyMap(char ** dest, char ** source);
inline int getEmptyCount();
inline int isLeaf(State * state);
void branchFrom(State * state);
void computeScore();

/************************************************
 * GLOBAL VARIABLES
 ************************************************/
#define _DEBUG 0
#define _WIDTH 5
#define _HEIGHT 5
#define _INDEX_MAX _WIDTH * _HEIGHT
const int WIDTH = _WIDTH;
const int HEIGHT = _HEIGHT;
const int DEBUG = _DEBUG;
const int INDEX_MAX = _INDEX_MAX;
char ** map;
char ** bestMap;
unsigned double bestScore;

int main() {
    Node *currentNode;
    State *currentState;
    stackPushState(newState(0, 0));
    map = newMap();

    while (!isStackEmpty()) {
        currentNode = stackPeek();
        currentState = currentNode->state;
        /* Fit shape *currentState->shape in map on place currentState->index
         * and with map values of *currentState->shape
         */
        fit(currentState, currentState->shape);
        if (currentNode->isBranched || isLeaf(currentNode->state)) {
            /* Unfit: replace current shape at old index with empty */
            fit(currentState, EMPTY);

            if (isLeaf(currentNode->state)) {
                computeScore();
            }
            stackDeleteTop();
        } else {
            branchFrom(currentState);
            currentNode->isBranched = 1;
        }
        if (DEBUG) {
            getc(stdin);
        }
        /* Check for other thread's requests here */
    }
    freeMap(map);

    return 0;
}

inline char ** newMap() {
    int i;
    char ** newMap;
    // Initialize map
    newMap = (char **) calloc(WIDTH, WIDTH * sizeof(char *));
    for (i = 0; i < WIDTH; i++) {
        newMap[i] = (char *) calloc(HEIGHT, HEIGHT * sizeof(char));
    }

    return newMap;
}

inline void freeMap(char ** someMap) {
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

inline void copyMap(char ** dest, char ** source) {
    int i;

    for (i = 0; i < WIDTH; i++) {
        memcpy(dest[i], source[i], HEIGHT);
    }
}

void branchFrom(State *state) {
    int newIndex;
    Shape shape;

    for (shape = EMPTY; shape <= STICK2; shape++) {
        /* Will it fit? WILL IT BLEND??? That is the question. */
        if (fitable(shape, state->index)) {
            // New index is next free field in map
            newIndex = state->index + shapeWidths[shape];
            State *createdState = newState(newIndex, shape);
            stackPushState(createdState);
        }
    }
}

/* Add tree cuting logic here */
inline int isLeaf(State *state) {
    return state->index + 1 >= INDEX_MAX;
}

inline int getShapeCountsSum() {
    char * shapeCounts = (char) calloc(5, 5 * sizeof(char));


    // TODO
    free(shapeCounts);

}
/*
 *
 */
inline int getEmptyCount() {
    int emptyCount = 0;
    int h, w;
    for (h = 0; h < HEIGHT; h++) {
        for (w = 0; w < WIDTH; w++) {
            if (map[h][w] == EMPTY) { emptyCount++; }
        }
    }
}


// p jsou nezaplnena policka
// C je cetnost zaplneni - shapeCounts
// Q(a,b) = ( 1 + p/ab ) * ( 1 + Σ|Ci - min (C)| )
void computeScore() {
    int p = getEmptyCount();
    int shapeCountsSum = getShapeCountsSum();
    unsigned double newScore = (1.0 + p/(WIDTH*HEIGHT)) * (1.0 + getShapeCountsSum());

    if (DEBUG) {
        printf("TESTING SCORE: new=%f old=%f\n", newScore, bestScore);
    }

    if (newScore > bestScore) {
        if (bestMap == NULL) {
            bestMap = newMap();
        }
        copyMap(bestMap, map);
        bestScore = newScore;
    }
    //TODO if (emptyCount<bestEmptyCount && shapeCountsDifference<bestShapeCountsDifference) then
    // memcpy(map) to bestMap; memcpy(shapeCounts) to bestShapeCounts; bestEmptyCount = emptyCount
}