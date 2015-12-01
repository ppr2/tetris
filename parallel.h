#ifndef TETRIS_PARALLEL_H
#define TETRIS_PARALLEL_H

extern int token_sent;

void parallelInit(int);

void sendNoWork(int requester);
void sendWork(int p_recipient, int half);
void requestWork(int);
int processIncomingWork(int workSource);

void processToken(int my_rank, int p_cnt);
void sendTokenToNeighbour(int token, int my_rank, int p_cnt);
void sendFinishToNeighbour(int my_rank, int p_cnt);

#endif //TETRIS_PARALLEL_H
