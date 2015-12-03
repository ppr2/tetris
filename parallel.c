#include "parallel.h"
#include "stack.h"
#include "state.h"

#define MSG_WORK_REQUEST 1000
#define MSG_WORK_BATCH    1001
#define MSG_WORK_NOWORK  1002
#define MSG_TOKEN        1003
#define MSG_FINISH       1004

#define TOKEN_BLACK 1
#define TOKEN_WHITE 0

#define MAX_MSG_LENGTH (WIDTH*HEIGHT*20)/2

/************************************************
 * INIT
 ************************************************/

int token;
int tokenColor = TOKEN_WHITE;

int solutionSent = 0;

//int workSenderRank = -1;

MPI_Request *solutionRequests;

MPI_Request incomingWorkRequest;
MPI_Request outcomingWorkRequest;

int *solutionArray;

void processToken(int my_rank, int p_cnt) {
    MPI_Status status;
    int token;

    MPI_Recv(&token, 1, MPI_INT, MPI_ANY_SOURCE, MSG_TOKEN, MPI_COMM_WORLD, &status);
    waitForUnfinishedSending();

    if (my_rank == 0) {
        // It came through the whole circle
        if (token == TOKEN_WHITE) {
            sendFinishToNeighbour(my_rank, p_cnt);
        } else {
            sendTokenToNeighbour(TOKEN_WHITE, my_rank, p_cnt);
        }
    } else {
        int tokenToSend;

        if (token == TOKEN_BLACK) {
            tokenToSend = TOKEN_BLACK;
        } else {
            tokenToSend = work_requested == 0 ? TOKEN_WHITE : TOKEN_BLACK;
        }
        sendTokenToNeighbour(tokenToSend, my_rank, p_cnt);
    }
}

void sendTokenToNeighbour(int token, int my_rank, int p_cnt) {
    int neighbourRank = my_rank == p_cnt - 1 ? 0 : my_rank + 1;

    waitForUnfinishedSending();
    MPI_send(&token, 1, MPI_INT, neighbourRank, MSG_TOKEN, MPI_COMM_WORLD);
}

void sendFinishToNeighbour(int my_rank, int p_cnt) {
    int neighbourRank = my_rank == p_cnt - 1 ? 0 : my_rank + 1;
    int emptyBuffer = 0;

    waitForUnfinishedSending();
    MPI_send(&emptyBuffer, 1, MPI_INT, neighbourRank, MSG_FINISH, MPI_COMM_WORLD);
}

void parallelInit(int my_rank) {

    if (my_rank == 0) {
        branchUntilStackSizeIsBigEnoughToSplit();
        for (int p_i = 1; p_i < stackSize() - 1; p_i++) {
            sendWork(p_i, 0); // Send one node to p_i
        }
    } else {
        int *dataArray;
        int dataLength;
        int flag;
        MPI_Status status;

        printf("RANK=%d waiting for msg\n", my_rank);
        // Wait for initial message, can receive MSG_FINISH or MSG_TOKEN
        do {
            MPI_Iprobe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        } while (!flag);
        MPI_Get_count(&status, MPI_INT, &dataLength);
        dataArray = (int) malloc(sizeof(int) * dataLength);
        MPI_Recv(dataArray, dataLength, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        switch (status.MPI_TAG) {
            case MSG_FINISH:
                printf("RANK=%d received finish\n", my_rank);
                MPI_Finalize();
                exit(0);
                break;
            case MSG_WORK_SENT:
                printf("RANK=%d received token %d\n", my_rank, message);
                createStackAndMapFromReceived(dataArray, dataLength);
                break;
            default:
                //TODO ERROR
                printf("error aaaaaaaaaaaaaaaaaaaa\n");
                MPI_Finalize();
                exit(1);
        }
    }
}

/************************************************
 * WORK functions
 ************************************************/


void sendNoWork(int requester) {
    int emptyBuffer = 0;

    waitForUnfinishedSending();
    MPI_send(&emptyBuffer, 1, MPI_INT, requester, MSG_WORK_NOWORK, MPI_COMM_WORLD);
}
/**
 * Send part of the stack to CPU p_recipient
 * half - send half of stack row (1) or just one element and its subtree (0)?
 */
void sendWork(int p_recipient, int half) {
    /* Get states from stack cutting */
    State *states;
    int statesCount = stackSplit(&states, half);
    int dataArray[statesCount*3 + WIDTH*HEIGHT]; // *3 because state consists of 3 integers
    /* Serialize the states to array, save that to dataArray */
    int *p_dataArray = &dataArray[0];
    int statesDataSize = getArrayFromStackAndMap(&p_dataArray1, states, statesCount);

    waitForUnfinishedSending();

    /* Send states array to p_recipient */
    MPI_Isend(dataArray, statesDataSize, MPI_INT, p_recipient, MSG_WORK_SENT, MPI_COMM_WORLD, &incomingWorkRequest);
    previousIncomingWorkRequestRank = p_recipient;

    /* Free up memory if I sent something */
    if(statesCount > 0) {
        // Free states
        for(int i = 0; i < statesCount; i++) {
            free(states[i]);
        }
        // Free states pointer
        free(states);
    }
}

/**
 * Request more work
 */
void requestWork(int workSenderRank) {
    int emptyBuffer = 0;

    waitForUnfinishedSending();
    /* Send the work request */
    MPI_send(&emptyBuffer, 1, MPI_INT, workSenderRank, MSG_WORK_REQUEST, MPI_COMM_WORLD);
}

/**
 * Work on incoming work, which we have requested
 */
int processIncomingWork(int workSource) {
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
void receiveSolution(void) {

}

void transmitSolution() {
    long double * solutionArray = (double*)malloc((WIDTH*HEIGHT+1)*sizeof(double));
    int counter = 1;
    solutionArray[0] = bestScore;
    for(int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            solutionArray[counter] = (long double)bestMap[i][j];
            counter++;
        }
    }
    MPI_send(solutionArray, counter, MPI_LONG_DOUBLE, 0, MSG_FINISH, MPI_COMM_WORLD);
}
