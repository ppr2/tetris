#include "tetris.h"

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
void branchUntilStackSizeIsBigEnoughToSplit(void); // used in parallelInit
int processFinish(int sourceRank);

/************************************************
 * GLOBAL VARIABLES
 ************************************************/
#define _DEBUG 0
#define _DEBUG_STEPS 0
#define _WIDTH 7
#define _HEIGHT 7
#define _INDEX_MAX _WIDTH * _HEIGHT

int WIDTH  = _WIDTH;
int HEIGHT = _HEIGHT;
int DEBUG  = 0;//= _DEBUG;
int DEBUG_PARALLEL  = 0;//= _DEBUG;
const int DEBUG_STEPS = _DEBUG_STEPS;
const int INDEX_MAX = _INDEX_MAX;
int workRequested; // 0/1 whether work was requested

char ** map;

int token_sent;

char ** bestMap;
long double bestScore = 9999999;
int *results;      // 0/1 whether p[i] sent results
/************************************************
 * LOCAL VARIABLES
 ************************************************/
char frequencies[7] = {0};
int p_cnt;         // processor count
int my_rank;       // rank of this process
int p_index;       // index of work giving process
int workRequested; // 0/1 whether work was requested
double t1, t2;      // p0 only for measuring time

#define MSG_WORK_REQUEST 1000
#define MSG_WORK_BATCH    1001
#define MSG_WORK_NOWORK  1002
#define MSG_TOKEN        1003
#define MSG_FINISH       1004

#define TOKEN_BLACK 1
#define TOKEN_WHITE 0


int main(int argc, char** argv) {

    // P0 only
    token_sent = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p_cnt);
    MPI_Barrier(MPI_COMM_WORLD);
    if (my_rank == 0) {
        t1 = MPI_Wtime();
    }
    map = newMap();
    results = (int*) calloc(p_cnt, sizeof(int));

    parallelInit(my_rank);
    if(DEBUG_PARALLEL){printf("---(%d) Initialized\n", my_rank);}

    p_index = 0;        // index of work giver
    workRequested = 0; // is this process waiting for more work?

    while (1) {
        //printf("---(%d) stack size=%d\n", my_rank, stackSize());
        branchIfYouCan();
        // Is there any process I haven't asked for work yet?
        if (!workRequested && p_index < p_cnt) {
            // request work from process p_index
            if (p_index == my_rank) p_index++;
            if (p_index < p_cnt) {
                requestWork(p_index);
                workRequested = 1;
            }
        } else if (my_rank == 0 && p_index >= p_cnt && !token_sent) {
            // I asked everyone. If I'm p0 send token
            sendTokenToNeighbour(TOKEN_WHITE, my_rank, p_cnt);
            token_sent = 1;
        }
        parseOuterMessages();
    }
}


void branchUntilStackSizeIsBigEnoughToSplit(void) {
    if(DEBUG_PARALLEL){printf("---(%d) Branching until stack size is big enough to split \n", my_rank);}
    /* Init */
    Node *currentNode;
    State *currentState;
    stackPushState(newState(0, EMPTY, 0));

    /* Main cycle */
    while (!isStackEmpty() && stackSize() < p_cnt + 1) {
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
    if(DEBUG_PARALLEL && !isStackEmpty()){printf("---(%d) Branching 'cause I can!\n", my_rank);stackPrintOutCompact();}
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
                computeScore();
            }

            frequencies[getSimpleShape(currentState->shape)]--;
            stackDeleteTop();
        } else { // Expand current node
            branchFrom(currentState);
            currentNode->isBranched = 1;
        }

        if (passes_cnt > 100) {
            parseInnerMessages();
            passes_cnt = 0;
        }
        passes_cnt++;
    }
}
void parseOuterMessages(void) {
    MPI_Status status;
    int msg_arrived, dump;

    //printf("---(%d) Probing p_index=%d\n", my_rank, p_index);
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &msg_arrived, &status);
    if (msg_arrived) {
        if(DEBUG_PARALLEL){printf("---(%d) Message arrived! MSG_TAG=%d from %d\n", my_rank, status.MPI_TAG, status.MPI_SOURCE);}
        switch (status.MPI_TAG) {
            case MSG_WORK_BATCH:
                if(DEBUG_PARALLEL){printf("---(%d) Parse Outer Messages -> MSG_WORK_BATCH \n", my_rank);}
                processIncomingWork(status);
                p_index = 0; // reset index of work giver
                workRequested = 0;
                break;
            case MSG_WORK_NOWORK:
                if(DEBUG_PARALLEL){printf("---(%d) Parse Outer Messages -> MSG_WORK_NOWORK \n", my_rank);}
                // receive so it's not stuck in queue
                MPI_Recv(&dump, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
                p_index++;   // ask another process next round
                workRequested = 0;
                break;
            case MSG_FINISH:
                if(DEBUG_PARALLEL){printf("---(%d) Parse Outer Messages -> MSG_FINISH \n", my_rank);}
                if (processFinish(status.MPI_SOURCE)) {
                    exit(0);
                }
                break;
            case MSG_TOKEN:
                // receive so it's not stuck in queue
                MPI_Recv(&dump, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
                if(DEBUG_PARALLEL){printf("---(%d) Parse Outer Messages -> MSG_TOKEN (%s)\n", my_rank, dump ? "TOKEN_BLACK" : "TOKEN_WHITE");}
                processToken(my_rank, p_cnt, dump);
                break;
            case MSG_WORK_REQUEST:
                if(DEBUG_PARALLEL){printf("---(%d) Parse Outer Messages -> MSG_WORK_REQUEST \n", my_rank);}
                MPI_Recv(&dump, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
                sendNoWork(status.MPI_SOURCE);
                break;
            default:
                printf("---(%d) Invalid MPI tag: %d\n", my_rank, status.MPI_TAG);
                exit(1);
        }
    }
}

void parseInnerMessages(void) {
    MPI_Status status;
    int msg_arrived, dump;

    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &msg_arrived, &status);
    if (msg_arrived) {
        // receive so it's not stuck in queue
        MPI_Recv(&dump, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
        switch (status.MPI_TAG) {
            case MSG_WORK_REQUEST:
                // Someone asked for work - send part of stack/NO_WORK
                if(DEBUG_PARALLEL){printf("---(%d) Parse Inner Messages -> MSG_WORK_REQUEST \n", my_rank);}
                if (isStackSplittable()) {
                    sendWork(status.MPI_SOURCE, 1);
                } else {
                    sendNoWork(status.MPI_SOURCE);
                }
                break;
            case MSG_TOKEN:
                // P0 asked for token, respond black. I'm not done yet
                if(DEBUG_PARALLEL){printf("---(%d) Parse Inner Messages -> MSG_TOKEN \n", my_rank);}
                sendTokenToNeighbour(TOKEN_BLACK, my_rank, p_cnt);
                break;
            default:
                printf("---(%d) Invalid MPI tag: %d\n", my_rank, status.MPI_TAG);
                exit(1);
        }
    }
}

int doIHaveResultsFromAllProcesses() {
    for (int i = 1; i < p_cnt; i++) {
        if (results[i] == 0) {
            return 0;
        }
    }
    return 1;
}

// deal with MSG_FINISH tag
int processFinish(int sourceRank) {
    int dump;
    MPI_Status status;

    if (my_rank == 0) {
        receiveSolution(sourceRank);
        if (doIHaveResultsFromAllProcesses()) {
            /* Results output */
            printf("\n--- RESULTS ---\n");
            if (bestMap == NULL) {
                printf("bestMap is null, it definitely won't blend :(\n");
            } else {
                printMap(bestMap);
                printf("score -> %Lf\n", bestScore);
                printf("\n");
            }
            t2 = MPI_Wtime();
            printf("---(0) Parallel time: %f\n",t2-t1);

            /* Free global structures */
            freeMap(map);
            freeMap(bestMap);
            free(results);
            MPI_Finalize();
            return 1;
        } else {

            return 0;
        }
    } else {
        MPI_Recv(&dump, 1, MPI_INT, sourceRank, MSG_FINISH, MPI_COMM_WORLD, &status);
        transmitSolution();
        sendFinishToNeighbour(my_rank, p_cnt);
        /* Free global structures */
        freeMap(map);
        freeMap(bestMap);
        free(results);

        MPI_Finalize();
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

    //if(DEBUG_PARALLEL){printf("---(%d) Computing score. newScore=%Lf\n", my_rank, newScore);}

    if (newScore != 2) {
        if (newScore < bestScore) {
            if(DEBUG_PARALLEL){printf("---(%d) Replacing old score=%Lf\n", my_rank, bestScore);}
            if (bestMap == NULL) {
                bestMap = newMap();
            }
            copyMap(bestMap, map);
            bestScore = newScore;
        }
    }
}

void printMap(char ** map) {
    int x,y;
    if (DEBUG_PARALLEL){printf("---(%d) ", my_rank);}
    for (x = 0; x < WIDTH*4; x++) {
        printf("=");
    }
    printf("\n");
    for (y = 0; y < HEIGHT; y++) {
        if (DEBUG_PARALLEL){printf("---(%d) ", my_rank);}
        for (x = 0; x < WIDTH; x++) {
            if (map[x][y] < 2) {
                printf("|  |");
            } else {
                printf(map[x][y] > 9 ? "|%d|" : "| %d|", map[x][y]);
            }
        }
        printf("\n");
    }
    if (DEBUG_PARALLEL){printf("---(%d) ", my_rank);}
    for (x = 0; x < WIDTH*4; x++) {
        printf("=");
    }
    printf("\n");
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

void copyMapToIntArray(int * dest, char ** source) {
    int i, j;
    for(i = 0; i < WIDTH; i++) {
        for(j = 0; j < HEIGHT; j++) {
            dest[i*WIDTH+j] = (int)source[i][j];
        }
    }
}
