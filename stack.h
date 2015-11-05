#ifndef TETRIS_STACK_H
#define TETRIS_STACK_H

#include "state.h"
typedef struct{
    State *state;
    int isBranched;
    void *previous;
    void *next;
} Node;

void stackPrintOut(void);
void stackPrintOutCompact(void);
// Operace na stacku
Node *stackPeek(void);
void stackDeleteTop(void);
State *stackPushState(State* state);
State *stackPushStateWithPoppedInfo(State* state, int isBranched);
int stackSplit(State **states);

int stackSize(void);
int isStackEmpty();

void freeNode(Node *node);


#endif
