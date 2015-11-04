#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"


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
char map[WIDTH][HEIGHT] = {0};

int main() {
    unsigned int index = 0;

    Node * currentNode;
    State* currentState;
    stackPushState(newState(0, 0));
    int oldIndex;

    while (!isStackEmpty()) {
        currentNode = stackPeek();
        currentState = currentNode->state;
        /* Fit shape *currentState->shape in map on place currentState->index
         * and with map values of *currentState->shape
         */
        fit(currentState->shape, currentState->index, currentState->shape);
        if (currentNode->isBranched || isLeaf(currentNode->state)) {
            /* Unfit: replace current shape at old index with empty */
            // TODO remove stuff bellow? I think we should!
            // oldIndex = currentNode->value->index - shapeWidths[currentNode->value->shape];
            fit(currentState->shape, currentState->index, EMPTY);

            if (isLeaf(currentNode->state)) {
                computeScore();
            }
            stackDeleteTop();
        } else {
            branchFrom(currentState);
            currentNode->isBranched = 1;
        }
        /* Check for other thread's requests here */
    }

    return 0;
}

void branchFrom(State * state) {
    int hasBranched = 0;
    Shape shape;

    for (shape = EMPTY; shape <= STICK2; shape++) {
        /* Will it fit? WILL IT BLEND??? That is the question. */
        if (fitable(shape, state->index)) {
            // New index is next free field in map
            State * createdState = newState(state->index + shapeWidths[shape], shape);
            stackPushState(createdState);
        }
    }
}

/* Add tree cuting logic here */
// TODO can be inline function?
int isLeaf(State * state) {
    return state->index + 1 >= INDEX_MAX;
}

void computeScore() {
    int emptyCount = 0;
    for(int h=0;h<HEIGHT;h++){
        for(int w=0;w<WIDTH;w++){
            if(map[h][w]==EMPTY){emptyCount++;}
        }
        printf("\n");
    }
    //TODO if (emptyCount<bestEmptyCount && shapeCountsDifference<bestShapeCountsDifference) then
    // memcpy(map) to bestMap; memcpy(shapeCounts) to bestShapeCounts; bestEmptyCount = emptyCount
}

void printMap(){
    for(int h=0;h<HEIGHT;h++){
        for(int w=0;w<WIDTH;w++){
            printf("%d,", map[h][w]);
        }
        printf("\n");
    }
    printf("-------");
}