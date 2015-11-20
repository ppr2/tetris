#ifndef PPR2_TETRIS_TETRIS_H
#define PPR2_TETRIS_TETRIS_H

#include "state.h"

inline char ** newMap();
inline void freeMap(char **);
inline void copyMap(char ** dest, char ** source);
inline int isLeaf(State *);
void branchFrom(State *);
void computeScore();
void branchUntilStackSizeIsBigEnoughToSplit(void);

#endif //PPR2_TETRIS_TETRIS_H
