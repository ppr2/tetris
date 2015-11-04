#ifndef PPR2_TETRIS_TETRIS_H
#define PPR2_TETRIS_TETRIS_H

#include "state.h"
#include "stack.h"
#include "fitting.h"

void branchFrom(State * state);
int isLeaf(State * state);
void computeScore();
void printMap();

#endif //PPR2_TETRIS_TETRIS_H
