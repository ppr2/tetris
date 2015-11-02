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
State *stackPush(State* state);
State *stackPushWithPoppedInfo(State* state, int popped);
int stackSplit(State **states);

int stackSize(void);
int isStackEmpty();

void deleteNode(Node *node);


#endif
