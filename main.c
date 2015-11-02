#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "state.h"
#include "stack.h"

#define WIDTH 5
#define HEIGHT 5
#define OP_ARR_LENGTH (WIDTH * HEIGHT) / 2
#define INDEX_MAX WIDTH * HEIGHT


/************************************************
 * GLOBAL VARIABLES
 ************************************************/
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
            removeFromStack();

            /* Unfit: replace current shape at old index with empty */
            oldIndex = currentNode->value->index - shapeWidths[currentNode->value->shape];
            fit(currentNode->value->shape, oldIndex, EMPTY);

            if (isLeaf(currentState)) {
                storeBestScore();
            }
        } else {
            branchFrom(currentState);
            currentState->branched = 1;
        }
        /* Check for other thread's requests here */
    }

    return 0;
}

void branchFrom(State * state) {
    int x = state->index / WIDTH - 1;
    int y = state->index % WIDTH - 1;
    int hasBranched = 0;
    Shape shape;

    for (shape = EMPTY; shape <= STICK2; shape++) {
        /* Will it fit? */
        if (fitable(shape, state->index)) {
            State * createdState = newState(state->index + shapeWidths[shape], shape);
            pushToStack(createdState);
        }
    }
}

/*
 * return 0 if it doesn't fit
 * */
int fitable(Shape shape, int index) {
    int x = index / WIDTH - 1;
    int y = index % WIDTH - 1;

    switch(shape) {
        case EMPTY:
            return index + 1 < INDEX_MAX;
        case SQUARE:
            return x + 2 < WIDTH && y + 1 < HEIGHT
                   && map[x][y] == 0   && map[x+1][y] == 0
                   && map[x][y+1] == 0 && map[x+1][y+1] == 0;
        case EL1:
            return x + 3 < WIDTH && y + 1 < HEIGHT
                   && map[x][y] == 0     && map[x+1][y] == 0
                   && map[x+1][y] == 0   && map[x+2][y] == 0
                   && map[x+3][y] == 0   && map[x][y+1] == 0
                   && map[x+1][y+1] == 0 && map[x+2][y+1] == 0
                   && map[x+3][y+1];
    }
}
/*
 * Used to fit or unfit, depends on index and newValue.
 * Replace shape - shapeToFit - at index - index - with shape - newValue
 * */
void fit(Shape shapeToFit, int index, Shape newValue) {
    int x = index / WIDTH - 1;
    int y = index % WIDTH - 1;

    switch(shapeToFit) {
        case EMPTY:
            break;
        case SQUARE:
            map[x][y]     = newValue;
            map[x+1][y]   = newValue;
            map[x][y+1]   = newValue;
            map[x+1][y+1] = newValue;
            break;
        case EL1:
            map[x+3][y]   = newValue; /*        */
            map[x][y+1]   = newValue; /*     #  */
            map[x+1][y+1] = newValue; /*  ####  */
            map[x+2][y+1] = newValue; /*        */
            map[x+3][y+1] = newValue; /*        */
            break;
        /* TODO case all remaining shapes */
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

/*


*/
/*
 * index je modulo sirka mapy
 *
 *
 * *//*

state * fit(struct current, shape brickType) {
    // for all bricks
    switch(brickType) {
        case SQUARE:
            if () {
                new = fitSquare
                new.map = ;// COPY of curren.
                return new;// nafituj vsechny konfigurace
            }
            break;
        case EL:
            break;
        case STAIRS:
            break;
        case TRIANGLE:
            break;
        case STICK:
            break;
    }
    return NULL;
}

state * fitSquare(struct state current, shape brickType) {
    if (1) {
        state new;
        new.index = current.index + 1;
        map
        new.brickCounts = //COpy of current.brickCounts
                    new.brickCounts[0] += 1;
        return &new;
    }
    // pro vsechny konfigurace
    // hod na zasobnik novou mapu, pokud tam jde nafitovat
    jdeNafitovat?
    jo: naser to do new; return 1;
    ne: return NULL;
    stack.push(zkopirovany/pole);


}

*/
/*
 * fit1()
 *  if possible
 *   hod novy na stack
 *   fn1()
 * *//*


struct State {
    char index;
    char map[][];
    char brickCounts[];
};
typedef struct node {
    struct state;
    struct node * next;
} node_t;

node_t * head = malloc(sizeof(node_t));
*/
