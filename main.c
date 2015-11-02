#include "main.h"

#define WIDTH 5
#define HEIGHT 5
#define OP_ARR_LENGTH (WIDTH * HEIGHT) / 2
#define INDEX_MAX WIDTH * HEIGHT


/************************************************
 * GLOBAL VARIABLES
 ************************************************/

// TODO stack of states
// TODO operations array + index
Rotation operations[OP_ARR_LENGTH];
unsigned int operationIndex = 0;

int main() {
    char map[WIDTH][HEIGHT] = {0};
    unsigned int index = 0;
/*    int x = index / WIDTH - 1;
    int y = index % WIDTH - 1;*/




    State currentState;
    Rotation currentRotation;

    /* TODO push not branched node */
    // currentState = (State) malloc(sizeof(State));
    // pushToStack(newState);

    while ( ! isStackEmpty()) {
        currentState = peekFromStack();
        fit(currentState->rotation, &map, currentState->index, 1);
        // TODO mark currentState operation to map

        if (currentState->branched || isLeaf(currentState)) {
            removeFromStack();
            deleteLastOperation();
            currentRotation = operations[operationIndex];
            operationIndex--; /* Going up the DFS tree */
            // TODO undo last operation from map

            // Is it leaf?
            if ( ! currentState->branched) {
                storeBestScore();
            }
        } else {
            if (branchFrom(currentState)) {
                operationIndex++; /* Going down the DFS tree */
            }
            currentState->branched = 1;
        }
        /* Check for other thread's requests here */
    }

    return 0;
}

int branchFrom(State state, char * map[][], int index) {
    int x = index / WIDTH - 1;
    int y = index % WIDTH - 1;
    Rotation rotation;
    int rotationWidth;

    for (rotation = EL1; rotation <= STICK2; rotation++) {
        /* Will it fit? */
        if (fit(rotation, map, index, 0)) {
            rotationWidth = getRotationWidth(rotation);
            /* Create new state */
            State * newState; // TODO = (State) malloc(sizeof(State));
            newState->index = index + rotationWidth;
            newState->rotation = rotation;
            /* Shut it up to stack */
            pushToStack(newState);
        }
    }

    /* Add new state with index + 1 and no rotation */
    if (index + 1 < INDEX_MAX) {
        /* Create new state */
        State * newState; // TODO = (State) malloc(sizeof(State));
        newState->index = index + 1;
        /* Shut it up to stack */
        pushToStack(newState);
    }
}
int getRotationWidth(Rotation rotation) {
    switch (rotation) {
        case EL1:
        case EL2:
        case EL3:
        case EL4:
            return 5;
        /* TODO case all remaining rotations */
    }
}

int fit(Rotation rotation, char * map[][], int index, int forReal) {
    int x = index / WIDTH - 1;
    int y = index % WIDTH - 1;

    switch(rotation) {
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
        /* TODO case all remaining rotations */
        /* In case of initial state where rotation is 0 do nothing */
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
