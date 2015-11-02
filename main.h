#ifndef PPR2_TETRIS_MAIN_H
#define PPR2_TETRIS_MAIN_H

typedef struct operation {
    unsigned int index;

};

/************************************************
 * FUNCTION PROTOTYPES
 ************************************************/

void branchFrom(State * state);
int isLeaf(State * state);
void storeBestScore();
void fit(Shape shapeToFit, int index, Shape newValue);
int fitable(Shape shape, int index);

#endif //PPR2_TETRIS_MAIN_H
