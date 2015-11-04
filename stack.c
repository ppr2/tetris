#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define NULL_NODE (Node *)0

extern int size = 0;

extern Node *stackTop = NULL_NODE;
extern Node *stackBottom = NULL_NODE;

void stackPrintOut(void){
    if(isStackEmpty()){printf("\nStack is empty.\n"); return;}
    Node *node = stackBottom;
    printf("--- Stack bottom\n");
    while(node != NULL_NODE){
        printf("isBranched - %d, index - %d, shape - %d\n", node->isBranched, node->state->index, node->state->shape);
        node = node->next;
    }
    printf("--- Stack top\n\n");
}

Node *stackPeek(void){
    return stackTop;
}

void stackDeleteTop(void){
    if(stackTop==NULL_NODE){return;}
    int removedIndex = stackTop->state->index;
    Node *previous = stackTop->previous;
    freeNode(stackTop);
    stackTop = previous;
    if(stackTop != NULL_NODE){
        stackTop->next = NULL_NODE;
    }
    size--;
    printf("removed index - %d\n", removedIndex);
}

State *stackPushState(State* state){
    return stackPushStateWithPoppedInfo(state, 0);
}

State *stackPushStateWithPoppedInfo(State* currentState, int isBranched){
    Node *currentNode = (Node *)malloc(sizeof(Node));
    if(currentNode == NULL){printf("Out of memory during push!"); exit(EXIT_FAILURE);}

    if(size > 0){ // stack neni prazdny -> navazat
        stackTop->next = currentNode;
        currentNode->previous = stackTop;
    } else { // stack je prazdny -> nastavit dno
        stackBottom = currentNode;
        currentNode->previous = NULL_NODE;
    }

    currentNode->next = NULL_NODE; // je vrchol
    currentNode->state = currentState;
    currentNode->isBranched = isBranched;

    stackTop = currentNode;
    size++;

    return currentState;
}

int stackSplit(State **states) {
    //TODO
    return 1;
}

int stackSize(void){
    return size;
}

int isStackEmpty(){
    return (stackSize()==0)?1:0;
}

void freeNode(Node *node){
    freeState(node->state);
    free(node);

    //TODO pred zavolanim zavolat freeState(), aby se smazal i obsah
}