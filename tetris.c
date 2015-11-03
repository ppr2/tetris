#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"
#include "state.h"
#include "stack.h"
#include "fitting.h"



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
    stackPush(newState(0, 0));
    int oldIndex;

    while ( ! isStackEmpty()) {
        currentNode = peekFromStack();
        /* Fit shape *currentNode->value->shape in map on place currentNode->value->index
         * and with map values of *currentNode->value->shape
         */
        fit(currentNode->value->shape, currentNode->value->index, currentNode->value->shape);
        if (currentNode->popped || isLeaf(currentNode->value)) {

            /* Unfit: replace current shape at old index with empty */
            // TODO remove stuff bellow? I think we should!
            // oldIndex = currentNode->value->index - shapeWidths[currentNode->value->shape];
            fit(currentNode->value->shape, currentNode->value->index, EMPTY);

            if (isLeaf(currentNode->value)) {
                storeBestScore();
            }
            /* Remove must be here since we are removing space allocated for currentNode
             * hence we can not use it afterwards */
            removeFromStack();
        } else {
            branchFrom(currentState);
            currentState->branched = 1;
        }
        /* Check for other thread's requests here */
    }

    return 0;
}

void branchFrom(State * state) {
    int hasBranched = 0;
    Shape shape;

    for (shape = EMPTY; shape <= STICK2; shape++) {
        /* Will it fit? */
        if (fitable(shape, state->index)) {
            // New index is next free field in map
            State * createdState = newState(state->index + shapeWidths[shape], shape);
            pushToStack(createdState);
        }
    }
}

/* Add tree cuting logic here */
// TODO can be inline function?
int isLeaf(State * state) {
    return state->index + 1 >= INDEX_MAX;
}
void storeBestScore() {
 // TODO
}