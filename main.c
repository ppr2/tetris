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

// TODO stack of states
// TODO operations array + index
Shape operations[OP_ARR_LENGTH];
unsigned int operationIndex = 0;

int main() {
    char map[WIDTH][HEIGHT] = {0};
    unsigned int index = 0;
/*    int x = index / WIDTH - 1;
    int y = index % WIDTH - 1;*/

    Node * currentNode;

    /* TODO push not branched node */
    // currentState = (State) malloc(sizeof(State));
    // index ma 0, shape = 0
    // pushToStack(newState);

    while ( ! isStackEmpty()) {
        currentNode = peekFromStack();
        fit(currentNode->value, &map);
        currentNode->
        if (currentNode->popped || isLeaf(currentNode->value)) {
            removeFromStack();
            //TODO unFit(currentNode);

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

int branchFrom(State state, char * map[][], int index) {
    int x = index / WIDTH - 1;
    int y = index % WIDTH - 1;
    int hasBranched = 0;
    Shape shape;
    int shapeWidth;

    for (shape = EMPTY; shape <= STICK2; shape++) {
        /* Will it fit? */
        if (fit(shape, map, index, 0)) {
            shapeWidth = getShapeWidth(shape);
            /* Create new state */
            State * newState; // TODO = (State) malloc(sizeof(State));
            newState->index = index + shapeWidth;
            newState->shape = shape;
            /* Shut it up to stack */
            pushToStack(newState);
            hasBranched = 1;
        }
    }
    return hasBranched;
}

int getShapeWidth(Shape shape) {
    switch (shape) {
        case EL1:
        case EL2:
        case EL3:
        case EL4:
            return 5;
        /* TODO case all remaining shapes */
    }
}

int fit(Shape shape, char * map[][], int index, int forReal) {
    int x = index / WIDTH - 1;
    int y = index % WIDTH - 1;

    switch(shape) {
        case EL1:
            if (forReal) {
                *map[x+3][y]   = EL1; /*        */
                *map[x][y+1]   = EL1; /*     #  */
                *map[x+1][y+1] = EL1; /*  ####  */
                *map[x+2][y+1] = EL1; /*        */
                *map[x+3][y+1] = EL1; /*        */
                return 0; // whatever
            } else {
                return x + 3 < WIDTH && y + 1 < HEIGHT
                        && *map[x][y] == 0     && *map[x+1][y] == 0
                        && *map[x+1][y] == 0   && *map[x+2][y] == 0
                        && *map[x+3][y] == 0   && *map[x][y+1] == 0
                        && *map[x+1][y+1] == 0 && *map[x+2][y+1] == 0
                        && *map[x+3][y+1];
            }
        /* TODO case all remaining shapes */
        /* In case of initial state where shape is EMPTY do nothing */
        default:
            return 0;
    }
}
int isLeaf(State state) {

}
void storeBestScore() {

}
void deleteLastOperation() {

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
