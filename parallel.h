#ifndef TETRIS_PARALLEL_H
#define TETRIS_PARALLEL_H

void parallelInit(void);

void sendWork(int p_recipient, int half);
void requestWork(void);
int processIncomingWork(void);

#endif //TETRIS_PARALLEL_H
