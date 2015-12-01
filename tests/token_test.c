#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

MPI_Status status;
int my_rank;
int p_cnt; // processor count



#define MSG_WORK_REQUEST 1000
#define MSG_WORK_SENT    1001
#define MSG_WORK_NOWORK  1002
#define MSG_TOKEN        1003
#define MSG_FINISH       1004

#define TOKEN_BLACK 1
#define TOKEN_WHITE 0

int main(int argc, char** argv) {
    MPI_Status status;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p_cnt);

    /* Parallel initialization */
    if (my_rank == 0) {
        //branchUntilStackSizeIsBigEnoughToSplit();
        int white_token = TOKEN_WHITE;
        for (int p_i = 1; p_i < p_cnt - 1; p_i++) {
            MPI_Send(&white_token, 1, MPI_CHAR, p_i, MSG_TOKEN, MPI_COMM_WORLD);
            printf("sending token to %d\n", p_i);
            //sendWork(p_i, 0); // Send one node to p_i
            //sendTokenToNeighbour(TOKEN_WHITE);
        }
        printf("rank0 done sending\n");
    } else {
        int message;
        printf("RANK=%d waiting for msg\n", my_rank);
        // Can receive MSG_FINISH or MSG_TOKEN
        MPI_Recv(&message, 1, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        switch (status.MPI_TAG) {
            case MSG_FINISH:
                printf("RANK=%d received finish\n", my_rank);
                break;
            case MSG_TOKEN:
                printf("RANK=%d received token %d\n", my_rank, message);
                break;
            default:
                //TODO ERROR
                printf("error\n");
                MPI_Finalize();
                exit(1);
        }
    }
    MPI_Finalize();

    return 0;
}