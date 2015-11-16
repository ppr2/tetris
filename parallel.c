#include "parallel.h"
#include "stack.h"
#include "state.h"

#define MSG_WORK_REQUEST 1000
#define MSG_WORK_SENT    1001
#define MSG_WORK_NOWORK  1002
#define MSG_TOKEN        1003
#define MSG_FINISH       1004

#define TOKEN_BLACK 1
#define TOKEN_WHITE 0

/************************************************
 * INIT
 ************************************************/

int token;
int tokenColor = TOKEN_WHITE;

int solutionSent = 0;

int workSenderRank = -1;

MPI_Request *solutionRequests;

MPI_Request incomingWorkRequest;
MPI_Request outcomingWorkRequest;

int *solutionArray;
int dataArrayLength = 16384;
int dataArray[16384];


void parallelInit(void){
    workSenderRank = (rank == 0) ? worldSize - 1 : rank - 1; // Set previous process as work donor
    solutionRequests = (MPI_Request*) malloc(worldSize * sizeof(MPI_Request));
    solutionArray = (int*) malloc(stateMaxLength * sizeof(int));
    token = (rank == 0) ? 1 : 0;
}

/************************************************
 * WORK functions
 ************************************************/

/**
 * Send part of the stack to CPU p_recipient
 * half - send half of stack row (1) or just one element and its subtree (0)?
 */
void sendWork(int p_recipient, int half){
    /* Get states from stack cutting */
    State *states;
    int statesCount = stackSplit(&states, half);
    /* Serialize the states to array, save that to dataArray */
    int *pointer = &dataArray[0];
    int statesDataSize = getArrayFromStates(&pointer, states, statesCount);
    if (statesDataSize > dataArrayLength){
        printf("ERROR - Stack split states array size larger than limit. (p-%d)\n", rank);
    }

    /* Send states array to p_recipient */
    MPI_Isend(dataArray, statesDataSize, MPI_INT, p_recipient, MSG_WORK_SENT, MPI_COMM_WORLD, &incomingWorkRequest);

    /* Color my state to BLACK if recipient is below */
    if(statesDataSize > 0 && rank > p_recipient){
        tokenColor = TOKEN_BLACK;
    }

    /* Free up memory */
    if(statesCount > 0){
        // Free states
        for(int i = 0; i < statesCount; i++){
            freeState(states[i]);
        }
        // Free states pointer
        free(states);
    }
}

/**
 * Request more work
 */
void requestWork(void) {
    /* Check for token */
    if(token){
        // Where should the token be sent?
        int p_recipient = (rank == worldSize - 1) ? 0 : rank + 1;
        if(rank == 0){tokenColor = WHITE;}
        // I am IDLE && have token => send it to the next one
        MPI_Send(&tokenColor, 1, MPI_INT, p_recipient, MSG_TOKEN, MPI_COMM_WORLD);
        token = 0; // Unset token, I don't have it anymore
    }
    tokenColor = WHITE; // I am IDLE -> color token white

    /* Send the work request */
    MPI_Isend(workRequestData, workRequestDataLength, MPI_CHAR, workSenderRank, MSG_WORK_REQUEST, MPI_COMM_WORLD, &outcomingWorkRequest);
}

/**
 * Work on incoming work, which we have requested
 */
int processIncomingWork(void) {
    /* Get incoming message */
    int receivedDataSize;
    MPI_Get_count(&status, MPI_INT, &receivedDataSize);
    int *receivedData = (int*)malloc(sizeof(int)*receivedDataSize);
    if(receivedData == 0){
        printf("ERROR - Receiving data empty / out of mem.");
        exit(EXIT_FAILURE);
    }
    // Receive
    MPI_Recv(receivedData, receivedDataSize, MPI_INT, workSenderRank, MSG_WORK_SENT, MPI_COMM_WORLD, &status);

    /* Work sender rank increment */
    int from = workSenderRank;
    workSenderRank++;
    if(workSenderRank == rank){workSenderRank++;}
    if(workSenderRank >= worldreceivedDataSize){workSenderRank = (rank == 0) ? 1 : 0;}

    /* Get State from array from received message */
    int index = 0, count = 0;
    State *previous = (State *) 0, *currentState = (State *) 0;
    while (index < receivedDataSize){
        State state = getStateFromArray(receivedData, &index);
        currentState = stackPushState(state);
        count++;
    }

    /* Free up memory */
    free(receivedData);
}







/******* TODO NOT YET USED/DONE, before implementation check if it can be used ***************/


void checkSolution(void) {
    int i;
    /* Check all processes for outcoming solutions */
    for(i = 0; i < worldSize; i++){
        if(i == rank) continue; // Current CPU => next
        int flag = 0;
        MPI_Iprobe(i, SOLUTION_TRANSMIT, MPI_COMM_WORLD, &flag, &status);
        if(flag){ // process i sends a solution
            int solutionArray[stateMaxLength];
            MPI_Recv(solutionArray, stateMaxLength, MPI_SHORT, i, SOLUTION_TRANSMIT, MPI_COMM_WORLD, &status);

            int index = 0;
            /* Convert solution array to State */
            State solution = getStateFromArray(solutionArray, &index);
//            if(!STAR || rank == 0){
//                printf("(%d) Incoming solution from process %d:\n", rank, i);
//            }
            //TODO(tetris.c) setBestSolution(solution);
        }
    }

}

void transmitSolution(State *solution) {
    int i;
    //musíme dokončit všechny předchozí poslání řešení
    if(solutionSent){
        for(i = 0; i < worldSize; i++){
            //dokončení non-blocking sendu (a čekání, pokud se ještě některá
            //data nestihla odeslat)
            if(i != rank){
                MPI_Wait(&solutionRequests[i], &status);
            }
        }
    }

    /* Convert solution State to array */
    getArrayFromState(solutionArray, solution, 0);
    /* Send solution array to everyone except me */
    for(i = 0; i < worldSize; i++){
        if(i != rank){
            MPI_Isend(solutionArray, stateMaxLength, MPI_SHORT, i, SOLUTION_TRANSMIT, MPI_COMM_WORLD, &solutionRequests[i]);
        }
    }
    solutionSent = 1;
}


void checkToken(void) {
    //spočítáme si, od koho by mohl pešek přijít
    int process = (rank == 0) ? worldSize - 1 : rank - 1;
    int flag = 0;
    MPI_Iprobe(process, TOKEN, MPI_COMM_WORLD, &flag, &status);
    if(flag){
        //přišel nám pešek
        MPI_Recv(&tokenColor, 1, MPI_INT, process, TOKEN, MPI_COMM_WORLD, &status);
        if(!STAR || rank == 0){
            printf("TOKEN (%d) Process %d received token with color %s from process %d.\n", rank, rank, tokenColor == TOKEN_WHITE ? "TOKEN_WHITE" : "TOKEN_BLACK", process);
        }
        token = 1;
        //pokud jsme 0 a příšel bílý pešek => máme padla, hoši
        if(rank == 0 && tokenColor == TOKEN_WHITE){
            transmitWorkEnd();
            endWork = 1;
        }
        if(processColor == TOKEN_BLACK){
            tokenColor = TOKEN_BLACK;
        }
    }
    tokenProbeCount -= tokenProbeThreshold + 1;
}