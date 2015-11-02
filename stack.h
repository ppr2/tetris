#ifndef TETRIS_STACK_H
#define TETRIS_STACK_H

#include "state.h"
typedef struct{
    State *value;
    int popped;
    void *previous;
    void *next;
} Node;

// Operace na stacku
State *stackPop(void);
State *stackPush(State state);
State *stackPushPopped(State state, int popped);
int stackCutInHalf(State **states);

int stackSize(void);

void deleteNode(Node *node);


#endif
