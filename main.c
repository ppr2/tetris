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

    while ( ! isStackEmpty()) {
        currentState = peekFromStack();
        //index += currentState->width;

        if (currentState->branched || isLeaf(currentState)) {
            removeFromStack();
            deleteLastOperation();
            currentRotation = operations[operationIndex];
            index--;

            // Is it leaf?
            if ( ! currentState->branched) {
                storeBestScore();
            }
        } else {
            branchFrom(currentState);
            currentState->branched = 1;
            index++;
        }
        /* Check for other thread's requests here */
    }


    /*
     *     *
     * stack s
     * s.push(prazdna mapa)
     * while stack.notEmpty?
     *  x = stack.pop()
     *  for all brick_shapes    // state je nafitovana brick
     *    if fitable_for? x
     *      state = fit(brick)
     *      stack.push(state)
     *
     *
     *
     * s = stack
     * s.push(map)
     * while s not empty
     *  for index=1..n
     *      stav = s.pop
     *      for brick = brick1..brick-n     *
     *          funkce(stav, index, brick)

     * fn1
     *  while neco na zasobniku
     *   mapZeStacku = peek ze stacku
     *
     *   fit1()
     *   fit2()
     *   fit3()
     *   ....
     *   odeber ze stacku
     * */

/*
    for (i = 0; i < WIDTH * HEIGHT; i++) {

        fitSquare();
        fitEl();
        //...
    }*/

    return 0;
}

void branchFrom(State state, char map[][], int index) {
    int x = index / WIDTH - 1;
    int y = index % WIDTH - 1;

    canFit

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
