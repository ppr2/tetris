#include "parallel.h"


#define BLACK 0
#define WHITE 1

#define WORK_REQUEST 0
#define WORK_TRANSMIT 1
#define SOLUTION_TRANSMIT 2
#define END_WORK 3
#define TOKEN 4

int solutionSent = 0;

int tokenColor = WHITE;

MPI_Request *solutionRequests = (MPI_Request *) malloc(worldSize * sizeof(MPI_Request));

short *solutionArray = (short *) malloc(stateMaxLength * sizeof(short));


void checkSolution(void) {
    int i;
    /* Check all processes for outcoming solutions */
    for(i = 0; i < worldSize; i++){
        if(i == rank) continue; // Current CPU => next
        int flag = 0;
        MPI_Iprobe(i, SOLUTION_TRANSMIT, MPI_COMM_WORLD, &flag, &status);
        if(flag){ // process i sends a solution
            short solutionArray[stateMaxLength];
            MPI_Recv(solutionArray, stateMaxLength, MPI_SHORT, i, SOLUTION_TRANSMIT, MPI_COMM_WORLD, &status);

            int index = 0;
            /* Convert solution array to State */
            State solution = convertStateFromArray(solutionArray, &index);
//            if(!STAR || rank == 0){
//                printf("(%d) Incoming solution from process %d:\n", rank, i);
//            }
            //TODO(tetris.c) setSolution(solution);
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
    getArrayFromState(solutionArray, *solution, 0, 0);
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
            printf("TOKEN (%d) Process %d received token with color %s from process %d.\n", rank, rank, tokenColor == WHITE ? "WHITE" : "BLACK", process);
        }
        token = 1;
        //pokud jsme 0 a příšel bílý pešek => máme padla, hoši
        if(rank == 0 && tokenColor == WHITE){
            transmitWorkEnd();
            endWork = 1;
        }
        if(processColor == BLACK){
            tokenColor = BLACK;
        }
    }
    tokenProbeCount -= tokenProbeThreshold + 1;
}