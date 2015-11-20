#ifndef TETRIS_PARALLEL_H
#define TETRIS_PARALLEL_H

void parallelInit(int);

void sendWork(int p_recipient, int half);
void requestWork(int);
int processIncomingWork(void);

void sendTokenToNeighbour(int token); //
void sendFinishToNeighbour(int my_rank);

#endif //TETRIS_PARALLEL_H
