#ifndef PPR2_TETRIS_MAIN_H
#define PPR2_TETRIS_MAIN_H

typedef struct operation {
    unsigned int index;

};

/************************************************
 * FUNCTION PROTOTYPES
 ************************************************/

int branchFrom(State * state);
int isLeaf(State state);
void storeBestScore();
void deleteLastOperation();

#endif //PPR2_TETRIS_MAIN_H
