#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "fitting.h"
#include <limits.h>



/************************************************
 * LOCAL FUNCTIONS
 ************************************************/
char ** newMap();
void freeMap(char **);
void copyMap(char ** dest, char ** source);
int isLeaf(State *);
void branchFrom(State *);
void computeScore();

/************************************************
 * GLOBAL VARIABLES
 ************************************************/
#define _DEBUG 0
#define _DEBUG_STEPS 0
#define _WIDTH 5
#define _HEIGHT 4
#define _INDEX_MAX _WIDTH * _HEIGHT

int WIDTH  ;//= _WIDTH;
int HEIGHT ;//= _HEIGHT;
int DEBUG  ;//= _DEBUG;
const int DEBUG_STEPS = _DEBUG_STEPS;
const int INDEX_MAX = _INDEX_MAX;

char ** map;

/************************************************
 * LOCAL VARIABLES
 ************************************************/
char ** bestMap;
long double bestScore = 9999999;
char frequencies[7] = {0};
int workRequested = 0; // Have  we been asked for work?

int main(int argc, char** argv) {
    //TODO MPI_Init(&argc, &argv);
    //TODO initMPI();
      
    p_index = 0;
    while (status != FINISHED) {
      while (!isStackEmpty()) {
        // do sequential stuff
        if (more than 100 passes) {
          tag = get_mpi_tag
          switch (tag) {
            case MSG_WORK_REQUEST:
              // someone asked for work - send part of stack/NO_WORK
            case MSG_TOKEN:
              // P0 asked for token
              // respond black
            default:
              error
          }
        }
      }

      if (hasnt asked everybody yet) {
        requestWork(p_index); // one processor at time
      } else {
        if ( my_rank == 0 ) {
          if (token hasn't been sent) {
            sendToken() 
          }
        }
      }
      switch (get_mpi_tag) {
        case MSG_WORK_SENT:
          // prisel zasobnik, nacti
          p_index = 0;
        case MSG_WORK_NOWORK:
          p_index++;
        case MSG_FINISH:
          if ( my_rank == 0) {
            compareBestresults()
          } else {
            sendResultsToP0();
            return 0;
          }
        case MSG_TOKEN:
          if ( my_rank == 0) {
            if (white) {
              // for all processes send MSG_FINISH
            }
          } else {
            // return black if can't request any more work
          }
      }


    }
      
      
      
      
    if (argc < 3 || argc > 4) {
      fprintf(stderr, "Invalid number of arguments. Call tetris WIDTH HEIGHT [DEBUG]\n");
      return 1;
    }
    WIDTH = atoi(argv[1]);
    HEIGHT = atoi(argv[2]);
    DEBUG = argc == 4 ? atoi(argv[3]) : 0;
    /* Init */
    Node *currentNode;
    State *currentState;
    map = newMap();
    stackPushState(newState(0, EMPTY, 0));

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

            if (isLeaf(currentNode->state)) {computeScore();}

            frequencies[getSimpleShape(currentState->shape)]--;
            stackDeleteTop();
        } else { // Expand current node
            branchFrom(currentState);
            currentNode->isBranched = 1;
        }

        if (DEBUG && DEBUG_STEPS) {getc(stdin);}
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

    //TODO MPI_Finalize();

    return 0;
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
