#ifndef TETRIS_PARALLEL_H
#define TETRIS_PARALLEL_H

#include "stack.h"
#include "state.h"
#include "tetris.h"
#include <mpi.h>

extern int token_sent;
extern int my_rank, p_cnt;
extern int DEBUG_PARALLEL;
extern char ** bestMap;
extern long double bestScore;
extern int *results;
extern int workRequested; // 0/1 whether work was requested

void parallelInit(int);

void sendNoWork(int requester);
void sendWork(int p_recipient, int half);
void requestWork(int);
void processIncomingWork(MPI_Status status);
void receiveSolution(int sender);
void transmitSolution();

void processToken(int my_rank, int p_cnt, int token);
void sendTokenToNeighbour(int token, int my_rank, int p_cnt);
void sendFinishToNeighbour(int my_rank, int p_cnt);

#endif //TETRIS_PARALLEL_H
