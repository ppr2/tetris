#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "fitting.h"
#include <limits.h>
#include "parallel.h"
#include <mpi.h>

/*
 *
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 * TODO
 *
 * jen aby sis vsimnul. jeste na to kouknu, ale jediny co jeste neni poreseny
 * je realny posilani zprav. nekoukal jsem jeste jak serializovat zasobnik
 * a jak posilat ten zbytek, podle me to bude vzdycky jinak podle tagu zpravy
 * se budou lisity type a delky zprav
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * */

/************************************************
 * LOCAL FUNCTIONS
 ************************************************/
char ** newMap(void);
void freeMap(char **);
void copyMap(char ** dest, char ** source);
int isLeaf(State *);
void branchFrom(State *);
void computeScore(void);
void branchIfYouCan(void);
void parseInnerMessages(void);
int processFinish(void);
void processToken(void);
void branchUntilStackSizeIsBigEnoughToSplit(void); // used in parallelInit

/************************************************
 * GLOBAL VARIABLES
 ************************************************/
#define _DEBUG 0
#define _DEBUG_STEPS 0
#define _WIDTH 5
#define _HEIGHT 4
#define _INDEX_MAX _WIDTH * _HEIGHT

int WIDTH  = 4;//= _WIDTH;
int HEIGHT = 5;//= _HEIGHT;
int DEBUG  = 0;//= _DEBUG;
const int DEBUG_STEPS = _DEBUG_STEPS;
const int INDEX_MAX = _INDEX_MAX;

char ** map;

/************************************************
 * LOCAL VARIABLES
 ************************************************/
char ** bestMap;
long double bestScore = 9999999;
char frequencies[7] = {0};
int p_cnt; // processor count
int msg_arrived;
int my_rank;
MPI_Status status;

#define MSG_WORK_REQUEST 1000
#define MSG_WORK_SENT    1001
#define MSG_WORK_NOWORK  1002
#define MSG_TOKEN        1003
#define MSG_FINISH       1004

#define TOKEN_BLACK 1
#define TOKEN_WHITE 0


int main(int argc, char** argv) {
    int p_index,
        work_requested;
    MPI_Status status;

    // P0 only
    int token_sent;
    token_sent = 0;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p_cnt);
    map = newMap();
    parallelInit(my_rank);


    p_index = 0;        // index of work giver
    work_requested = 0; // is this process waiting for more work?
    while (1) {
        branchIfYouCan();

        // Is there any process I haven't asked for work yet?
        if (p_index < p_cnt) {
            // request work from process p_index
            if (p_index == my_rank) p_index++;
            requestWork(p_index); // TODO
            work_requested = 1;
        } else {
            // I asked everyone. If I'm p0 send token
            if (my_rank == 0 && !token_sent) {
                sendTokenToNeighbour(TOKEN_WHITE); // TODO
                token_sent = 1;
            }
        }

        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &msg_arrived, &status);
        if (msg_arrived) {
            switch (status.MPI_TAG) {
                case MSG_WORK_SENT:
                    processIncomingWork();
                    p_index = 0;
                    break;
                case MSG_WORK_NOWORK:
                    p_index++;
                    break;
                case MSG_FINISH:
                    if (processFinish()) {
                        return 0;
                    }
                    break;
                case MSG_TOKEN:
                    processToken(msg_arrived);
                    break;
                default:
                    raiseError("Invalid MPI tag: %d\n", status.MPI_TAG); // TODO
                    return 1;
            }
        }
    }
      
      
    /* Results output */
    printf("\n--- RESULTS ---\n");
    if (bestMap == NULL) {
        printf("bestMap is null, it definitely won't blend :(\n");
    } else {
        printMap(bestMap);
        printf("score -> %Lf\n", bestScore);
        printf("\n");
    }

    /* Free global structures */
    freeMap(map);
    freeMap(bestMap);

    MPI_Finalize(); // TODO

    return 0;
}

void processToken(some_type msg_arrived) {
    if (my_rank == 0) {
        // TODO parse receivedToken from msg_arrived
        // It came through the whole circle
        if (receivedToken == TOKEN_WHITE) {
            // End calculation send MSG_FINISH
            sendFinishToNeighbour(my_rank);
        } else {
            sendTokenToNeighbour(TOKEN_WHITE);
            token_sent = 1;
        }
    } else {
        int tokenToSend =
                receivedToken == TOKEN_BLACK ? TOKEN_BLACK : (work_requested == 0 ? TOKEN_WHITE : TOKEN_BLACK);
        sendTokenToNeighbour(tokenToSend);
    }
}


void branchUntilStackSizeIsBigEnoughToSplit(void) {
    /* Init */
    Node *currentNode;
    State *currentState;
    stackPushState(newState(0, EMPTY, 0));

    /* Main cycle */
    while (!isStackEmpty() || stackSize() < p_cnt + 1) {
        currentNode = stackPeek();
        currentState = currentNode->state;

        /* Process current node */
        if ( !currentNode->isBranched) {
            frequencies[getSimpleShape(currentState->shape)]++;
        }
        if(DEBUG){printf("fit i%d s%d\n", currentState->index, currentState->shape);}

        fit(currentState, currentState->shape); // Fit: Fit shape to place index
        if (currentNode->isBranched || isLeaf(currentNode->state)) {
            if(DEBUG){printf("unfit i%d s%d\n", currentState->index, currentState->shape);}

            fit(currentState, EMPTY); // Unfit: Remove shape at index (replace with EMPTY=0)

            if (isLeaf(currentNode->state)) {
            }

            frequencies[getSimpleShape(currentState->shape)]--;
            stackDeleteTop();
        } else { // Expand current node
            branchFrom(currentState);
            currentNode->isBranched = 1;
        }

        if (DEBUG && DEBUG_STEPS) {getc(stdin);}
    }
}

void branchIfYouCan(void) {
    /* Init */
    Node *currentNode;
    State *currentState;

    int passes_cnt = 0;     // how many loop passes were between MPI queue checking

    /* Main cycle */
    while (!isStackEmpty()) {
        currentNode = stackPeek();
        currentState = currentNode->state;

        /* Process current node */
        if ( !currentNode->isBranched) {
            frequencies[getSimpleShape(currentState->shape)]++;
        }
        if(DEBUG){printf("fit i%d s%d\n", currentState->index, currentState->shape);}

        fit(currentState, currentState->shape); // Fit: Fit shape to place index
        if (currentNode->isBranched || isLeaf(currentNode->state)) {
            if(DEBUG){printf("unfit i%d s%d\n", currentState->index, currentState->shape);}

            fit(currentState, EMPTY); // Unfit: Remove shape at index (replace with EMPTY=0)

            if (isLeaf(currentNode->state)) {
            }

            frequencies[getSimpleShape(currentState->shape)]--;
            stackDeleteTop();
        } else { // Expand current node
            branchFrom(currentState);
            currentNode->isBranched = 1;
        }

        if (DEBUG && DEBUG_STEPS) {getc(stdin);}

        if (passes_cnt > 100) {
            parseInnerMessages();
            passes_cnt = 0;
        }
        passes_cnt++;
    }
}

void parseInnerMessages(void) {
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &msg_arrived, &status);
    if (msg_arrived) {
        switch (status.MPI_TAG) {
            case MSG_WORK_REQUEST:
                // Someone asked for work - send part of stack/NO_WORK
                if (isStackSplittable()) { // TODO
                    sendWork(status.MPI_SOURCE, 1);
                } else {
                    reply(NO_WORK); // TODO
                }
                break;
            case MSG_TOKEN:
                // P0 asked for token
                // respond black
                sendTokenToNeighbour(TOKEN_BLACK); //TODO
                break;
            default:
                raiseError("Invalid MPI tag=%d from p=%d\n", status.MPI_TAG, status.MPI_SOURCE); // TODO
        }
    }
}


// deal with MSG_FINISH tag
int processFinish(void) {
    if (my_rank == 0) {
        compareBestResults(); // TODO compare and choose the best
        // TODO free memory

        return doIHaveResultsFromAllProcesses ? 1 : 0;
    } else {
        sendResultsToP0(); // TODO
        // TODO free memory
        return 1;
    }
}


void branchFrom(State * state) {
    Shape shape;
    int changed = 0;
    int nextFreeIndex = state->index + shapeWidths[state->shape];

    if(DEBUG){printf("pred branchem\n"); printMap(map); stackPrintOutCompact(); }

    for (shape = SPACE; shape <= STICK2; shape++) {
        /* Will it fit? WILL IT BLEND??? That is the question. */
        if (DEBUG && fitable(shape, nextFreeIndex)) {
            printf("STACK++ -> shape=%d, fitable=%d, stack+\n", shape, fitable(shape, nextFreeIndex));
        }

        if (fitable(shape, nextFreeIndex)) {
            // New index is next free field in map
            State *createdState = newState(nextFreeIndex, shape, state->depth+1);
            stackPushState(createdState);
            changed = 1;
        }
    }

    if (!changed && nextFreeIndex <= INDEX_MAX) {
        State *createdState = newState(state->index + 1, EMPTY, state->depth+1);
        stackPushState(createdState);
    }
}

/* Add tree cuting logic here */
int isLeaf(State *state) {
    return state->index >= INDEX_MAX;
}

int getShapeCountsSum() {
    int i, foo;
    int sum = 0;
    int minimum = INT_MAX;

    for (i = 2; i < 7; i++) {
        if (frequencies[i] < minimum) {
            minimum = frequencies[i];
        }
    }
    for (i = 2; i < 7; i++) {
        foo = frequencies[i] - minimum;
        sum += foo < 0 ? -foo : foo;
    }
    return sum;
}

int countEmpty() {
    int x,y, empties = 0;
    for (x = 0; x < WIDTH; x++) {
        for (y = 0; y < HEIGHT; y++) {
            if (map[x][y] < 2) {
                empties++;
            }
        }
    }

    return empties;
}

// p jsou nezaplnena policka
// C je cetnost zaplneni - shapeCounts
// Q(a,b) = ( 1 + p/ab ) * ( 1 + Σ|Ci - min (C)| )
void computeScore() {
    int i;
    long double p = countEmpty();
    long double newScore = (1.0 + p/(WIDTH*HEIGHT)) * (1.0 + getShapeCountsSum());

    if(DEBUG) {
        printf("--- LEAF ---\n");
        printf("FREQUENCIES\n");
        for (i = 0; i < 7; i++) printf(" %d", frequencies[i]);
        printf("\n");
        printMap(map);
        printf("emptyCount=%Lf, shapeCountsSum=%d\n", p, getShapeCountsSum());
        printf("TESTING SCORE: new=%Lf old=%Lf\n", newScore, bestScore);
        printf("--- /LEAF ---\n");
    }

    if (newScore != 2) {
        if (newScore < bestScore) {
            if (bestMap == NULL) {
                bestMap = newMap();
            }
            copyMap(bestMap, map);
            bestScore = newScore;
        }
    }
}


char ** newMap() {
    int i;
    char ** newMap;
    // Initialize map
    newMap = (char **) calloc(WIDTH, WIDTH * sizeof(char *));
    for (i = 0; i < WIDTH; i++) {
        newMap[i] = (char *) calloc(HEIGHT, HEIGHT * sizeof(char));
    }

    return newMap;
}

void freeMap(char ** someMap) {
    int i;

    if (someMap != NULL) {
        for (i = 0; i < WIDTH; i++){
            if (someMap[i] != NULL) {
                free(someMap[i]);
            }
        }
        free(someMap);
    }
}

void copyMap(char ** dest, char ** source) {
    int i;

    for (i = 0; i < WIDTH; i++) {
        memcpy(dest[i], source[i], HEIGHT);
    }
}
