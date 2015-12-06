#ifndef PPR2_TETRIS_TETRIS_H
#define PPR2_TETRIS_TETRIS_H

#include "state.h"
#include "parallel.h"
#include "fitting.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <mpi.h>
#include <unistd.h>

void printMap(char ** map);
char ** newMap();
void freeMap(char **);
void copyMap(char ** dest, char ** source);
void copyMapToIntArray(int * dest, char ** source);
void branchFrom(State *);
void computeScore();
void branchUntilStackSizeIsBigEnoughToSplit(void);
void parseOuterMessages(void);

#endif //PPR2_TETRIS_TETRIS_H
