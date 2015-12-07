#include "parallel.h"

#define MSG_WORK_REQUEST 1000
#define MSG_WORK_BATCH   1001
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
MPI_Request *solutionRequests;
int *solutionArray;
extern Node *stackTop;

void parallelInit(int my_rank) {
    //if(DEBUG_PARALLEL){printf("---(%d) Parallel init \n", my_rank);}

    if (my_rank == 0) {
        branchUntilStackSizeIsBigEnoughToSplit();
        for (int p_i = 1; p_i < p_cnt; p_i++) {
            //printf("SENDING WORK TO %d\n", p_i);
            sendWork(p_i, 0); // Send one node to p_i
            //printf("WORK SENT TO %d\n", p_i);
        }
    } else {
        int dataLength;
        int flag;
        MPI_Status status;

        if(DEBUG_PARALLEL){printf("---(%d) Waiting for MSG\n", my_rank);}
        // Wait for initial message, can receive MSG_FINISH or MSG_TOKEN
        do {
            MPI_Iprobe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        } while (!flag);
        MPI_Get_count(&status, MPI_INT, &dataLength);
        int dataArray[dataLength];
        MPI_Recv(&dataArray, dataLength, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        switch (status.MPI_TAG) {
            case MSG_FINISH:
                if(DEBUG_PARALLEL){printf("---(%d) received finish\n", my_rank);}
                MPI_Finalize();
                exit(0);
            case MSG_WORK_BATCH:
                if(DEBUG_PARALLEL){printf("---(%d) received work_batch\n", my_rank);}
                createStackAndMapFromReceived(dataArray, dataLength);
                if(DEBUG_PARALLEL){printf("---(%d) Incoming serialized array: ,[", my_rank); for(int i=0;i<dataLength;i++){printf("%d,",dataArray[i]);} printf("]\n");stackPrintOutCompact();}
                break;
            default:
                //TODO ERROR
                printf("error aaaaaaaaaaaaaaaaaaaa\n");
                MPI_Finalize();
                exit(1);
        }
    }
}


void processToken(int my_rank, int p_cnt, int token) {
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
            tokenToSend = workRequested == 0 ? TOKEN_WHITE : TOKEN_BLACK;
        }
        sendTokenToNeighbour(tokenToSend, my_rank, p_cnt);
    }
}

void sendTokenToNeighbour(int token, int my_rank, int p_cnt) {
    int neighbourRank = my_rank == p_cnt - 1 ? 0 : my_rank + 1;

    if(DEBUG_PARALLEL){printf("---(%d) Sending token %s to neighbour %d\n", my_rank, token ? "TOKEN_BLACK" : "TOKEN_WHITE", neighbourRank);}
    MPI_Send(&token, 1, MPI_INT, neighbourRank, MSG_TOKEN, MPI_COMM_WORLD);
}

void sendFinishToNeighbour(int my_rank, int p_cnt) {
    int neighbourRank = my_rank == p_cnt - 1 ? 0 : my_rank + 1;
    int emptyBuffer = 0;

    if(DEBUG_PARALLEL){printf("---(%d) Sending finish to neighbour %d\n", my_rank, neighbourRank);}
    MPI_Send(&emptyBuffer, 1, MPI_INT, neighbourRank, MSG_FINISH, MPI_COMM_WORLD);
}

/************************************************
 * WORK functions
 ************************************************/


void sendNoWork(int requester) {
    int emptyBuffer = 0;

    MPI_Send(&emptyBuffer, 1, MPI_INT, requester, MSG_WORK_NOWORK, MPI_COMM_WORLD);
}
/**
 * Send part of the stack to CPU p_recipient
 * half - send half of stack row (1) or just one element and its subtree (0)?
 */
void sendWork(int p_recipient, int half) {
    if(DEBUG_PARALLEL){printf("---(%d) Sending work to %d \n", my_rank, p_recipient);}
    /* Get states from stack cutting */
    State *states;
    MPI_Request request;
    MPI_Status status;

    stackPrintOutCompact();
    int statesCount = stackSplit(&states, half);
    stackPrintOutCompact();
    int dataArray[statesCount*3 + WIDTH*HEIGHT]; // *3 because state consists of 3 integers
    /* Serialize the states to array, save that to dataArray */
    if(DEBUG_PARALLEL){printf("---(%d) Serializing array from stack and map, #states=%d\n", my_rank, statesCount);}
    int statesDataSize = getArrayFromStackAndMap(dataArray, states, statesCount);
    if(DEBUG_PARALLEL){printf("---(%d) Serialized array to send (length=%d): ,[", my_rank, statesDataSize); for(int i=0;i<statesDataSize;i++){printf("%d,",dataArray[i]);} printf("]\n");stackPrintOutCompact();}


    /* Send states array to p_recipient */
    MPI_Isend(&dataArray, statesDataSize, MPI_INT, p_recipient, MSG_WORK_BATCH, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
}

/**
 * Request more work
 */
void requestWork(int workSenderRank) {
    if(DEBUG_PARALLEL){printf("---(%d) Requesting work from p_%d\n", my_rank, workSenderRank);}
    int emptyBuffer = 0;

    /* Send the work request */
    MPI_Send(&emptyBuffer, 1, MPI_INT, workSenderRank, MSG_WORK_REQUEST, MPI_COMM_WORLD);
}

/**
 * Work on incoming work, which we have requested
 */
void processIncomingWork(MPI_Status status) {
    int workSource = status.MPI_SOURCE;
    if(DEBUG_PARALLEL){printf("---(%d) Processing incoming work from p_%d\n", my_rank, workSource);}
    /* Get incoming message */
    int receivedDataSize;
    MPI_Get_count(&status, MPI_INT, &receivedDataSize);
    if(DEBUG_PARALLEL){printf("---(%d) Data length=%d from p_%d\n", my_rank, receivedDataSize, workSource);}
    int receivedData[receivedDataSize];

    // Receive
    MPI_Recv(receivedData, receivedDataSize, MPI_INT, workSource, MSG_WORK_BATCH, MPI_COMM_WORLD, &status);

    /* Get State from array from received message */
    createStackAndMapFromReceived(receivedData, receivedDataSize);
}
void receiveSolution(int sender) {
    const int dataLength = WIDTH*HEIGHT+1;
    MPI_Status status;
    if(DEBUG_PARALLEL){printf("---(%d) Receiving solution from %d\n", my_rank, sender);}

    long double dataArray[dataLength];
    MPI_Recv(dataArray, dataLength, MPI_LONG_DOUBLE, sender, MSG_FINISH, MPI_COMM_WORLD, &status);

    if (bestScore > dataArray[0]) {
        int counter = 1;
        bestScore = dataArray[0];
        for(int i=0;i<WIDTH;i++){
            for(int j=0;j<HEIGHT;j++){
                map[i][j] = (char) *(dataArray+counter);
                counter++;
            }
        }
    } else {
        // do nothing :)
    }
    // Mark that I have results from this sender
    results[sender] = 1;
}

void transmitSolution() {
    int counter = 1;
    long double solutionArray[WIDTH*HEIGHT+1];

    solutionArray[0] = bestScore;
    if(DEBUG_PARALLEL){printf("---(%d) Finish array allocated\n", my_rank);}
    for(int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if(DEBUG_PARALLEL){printf("---(%d) counter=%d\n", my_rank, counter);}
            solutionArray[counter++] = (long double) bestMap[i][j];
        }
    }
    if(DEBUG_PARALLEL){printf("---(%d) Finish array filled\n", my_rank);}
    MPI_Send(&solutionArray, counter, MPI_LONG_DOUBLE, 0, MSG_FINISH, MPI_COMM_WORLD);
    if(DEBUG_PARALLEL){printf("---(%d) Finish array sent\n", my_rank);}
}