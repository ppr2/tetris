#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "fitting.h"
#include "state.h"
#include <limits.h>



/************************************************
 * LOCAL FUNCTIONS
 ************************************************/
char ** newMap();
void freeMap(char **);
void copyMap(char ** dest, char ** source);
int isLeaf(State *);
void branchFrom(State *);
void computeScore();

/************************************************
 * GLOBAL VARIABLES
 ************************************************/
#define _DEBUG 0
#define _WIDTH 4
#define _HEIGHT 4
#define _INDEX_MAX _WIDTH * _HEIGHT

const int WIDTH = _WIDTH;
const int HEIGHT = _HEIGHT;
const int DEBUG = _DEBUG;
const int INDEX_MAX = _INDEX_MAX;

char ** map;

/************************************************
 * LOCAL VARIABLES
 ************************************************/
char ** bestMap;
long double bestScore = 9999999;
char frequencies[7] = {0};

int main() {
    Node *currentNode;
    State *currentState;
    stackPushState(newState(0, EMPTY));
    map = newMap();

    while (!isStackEmpty()) {
        currentNode = stackPeek();
        currentState = currentNode->state;
        if ( !currentNode->isBranched) {
            frequencies[getSimpleShape(currentState->shape)]++;
        }
        /* Fit shape *currentState->shape in map on place currentState->index
         * and with map values of *currentState->shape
         */
        if (DEBUG) {
            stackPrintOut();
        }

        fit(currentState, currentState->shape);
        if (currentNode->isBranched || isLeaf(currentNode->state)) {
            /* Unfit: replace current shape at old index with empty */
            fit(currentState, SPACE);

            if (isLeaf(currentNode->state)) {
                computeScore();
            }
            frequencies[getSimpleShape(currentState->shape)]--;
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
    printf("\n\n\n\n");
    if (bestMap == NULL) {
        printf("shit happend\n");
    } else {
        printMap(bestMap);
        printf("SCORE =%Lf\n", bestScore);
    }

    freeMap(map);
    freeMap(bestMap);

    return 0;
}

void branchFrom(State * state) {
    Shape shape;
    int changed = 0;
    int nextFreeIndex = state->index + shapeWidths[state->shape];

/*    if (nextFreeIndex == INDEX_MAX - 1) {
        printMap(map);
        printf("fitable? %d\n", fitable(SPACE, nextFreeIndex));
        int x = nextFreeIndex % WIDTH;
        int y = nextFreeIndex / WIDTH;
        printf("x=%d y=%d %d", x, y, x < WIDTH);
    }*/

    for (shape = SPACE; shape <= STICK2; shape++) {
        /* Will it fit? WILL IT BLEND??? That is the question. */
        if (DEBUG) {
            printf("trying shape=%d. fitable=%d\n", shape, fitable(shape, state->index));
        }

        if (fitable(shape, nextFreeIndex)) {
            // New index is next free field in map
            State *createdState = newState(nextFreeIndex, shape);
            stackPushState(createdState);
            changed = 1;
        }
    }
    if (nextFreeIndex == INDEX_MAX - 1) {
        printf("changed? %d\n", changed);
    }

    if (!changed && state->index <= INDEX_MAX) {
        /*printf("NOT CHANGED MAP\n");
        printf("fitable? %d at index=%d\n", fitable(SPACE, nextFreeIndex), nextFreeIndex);
        printMap(map);*/
        State *createdState = newState(state->index + 1, EMPTY);
        stackPushState(createdState);
    }
}

/* Add tree cuting logic here */
int isLeaf(State *state) {
    return state->index >= INDEX_MAX;
}

int getShapeCountsSum() {
    int i, foo;
    int sum = 0;
    int minimum = INT_MAX;

    printf("FREQUENCIES\n");
    for (i=0;i<7;i++) printf(" %d", frequencies[i]);
    printf("\n");

    for (i = 2; i < 7; i++) {
        if (frequencies[i] < minimum) {
            minimum = frequencies[i];
        }
    }
    for (i = 2; i < 7; i++) {
        foo = frequencies[i] - minimum;
        sum += foo < 0 ? -foo : foo;
    }

    return sum;
}

int countEmpty() {
    int x,y, empties = 0;
    for (x = 0; x < WIDTH; x++) {
        for (y = 0; y < HEIGHT; y++) {
            if (map[x][y] < 2) {
                empties++;
            }
        }
    }

    return empties;
}

// p jsou nezaplnena policka
// C je cetnost zaplneni - shapeCounts
// Q(a,b) = ( 1 + p/ab ) * ( 1 + Σ|Ci - min (C)| )
void computeScore() {
    long double p = countEmpty();
    printf("counts=%Lf\n, shapes=%d\n", p, getShapeCountsSum());
    long double newScore = (1.0 + p/(WIDTH*HEIGHT)) * (1.0 + getShapeCountsSum());

    if (DEBUG) {
        printf("TESTING SCORE: new=%Lf old=%Lf\n", newScore, bestScore);
    }

    printMap(map);
    printf("EMPTIES=%Lf NEW_SCORE=%Lf OLD_SCORE=%Lf\n", p, newScore, bestScore);

    if (newScore != 2) {
        if (newScore < bestScore) {
            if (bestMap == NULL) {
                bestMap = newMap();
            }
            copyMap(bestMap, map);
            bestScore = newScore;
        }
    }
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

void copyMap(char ** dest, char ** source) {
    int i;

    for (i = 0; i < WIDTH; i++) {
        memcpy(dest[i], source[i], HEIGHT);
    }
}
