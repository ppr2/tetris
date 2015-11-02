#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define NULL_NODE (Node *)0

int size = 0;

Node *stackTop = NULL_NODE;
Node *stackBottom = NULL_NODE;

State *stackPeek(void){
    return stackTop->value;
}

State *stackPop(void){
    if(stackSize() <= 0){ return (State *)0; }

    if(stackTop->popped){
        Node *previous = stackTop->previous;
        freeState(*(stackTop->value));
        deleteNode(stackTop);
        stackTop = previous;
        if(stackTop != NULL_NODE){
            stackTop->next = NULL_NODE;
        }
        size--;
        return stackPop();
    }

    State *state = stackTop->value;
    stackTop->popped = 1;

    return state;
}

State *stackPush(State state){
    return stackPushPopped(state, 0);
}

State *stackPushPopped(State state, int popped){
    State *currentState = (State *)malloc(sizeof(State));
    if(currentState == NULL){printf("Out of memory during push!"); exit(EXIT_FAILURE);}
    
    *currentState = state;

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
    currentNode->value = currentState;
    currentNode->popped = popped;

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

void deleteNode(Node *node){
    free(node->value);
    free(node);

    //TODO pred zavolanim zavolat freeState(), aby se smazal i obsah
}