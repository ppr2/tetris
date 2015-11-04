#include "stack_test.h"
#include "../stack.h"
#include <stdio.h>

int simpleTest(){
    printf("--- Pushing elements to stack \n");
    for(int i=0; i<10; i++){
        stackPushState(newState(i,i));
        stackPrintOut();
    }
    printf("--- Popping elements from stack \n");
    for(int i=0; i<10; i++){
        stackDeleteTop();
        stackPrintOut();
    }
    return 0;
}

int descendantsTest(){
    stackPushState(newState(0,0));
    Node* currentNode;
    int i=0;
    while(!isStackEmpty()){
        currentNode = stackPeek();
        if(currentNode->isBranched) {
            stackDeleteTop();
        } else {
            currentNode->isBranched = 1;
            if(i<3){
                stackPushState(newState(i*3+1,1));
                stackPushState(newState(i*3+2,i));
                stackPushState(newState(i*3+3,i));
                i++;
            }
        }
        stackPrintOut();
    }
    return 0;
}

int main(){
    //printf("\n\n-------- Stack simple test\n");
    //simpleTest();
    printf("\n\n-------- Stack descendants test\n");
    descendantsTest();

    return 0;
}