#ifndef TETRIS_FITTING_H
#define TETRIS_FITTING_H

#include "state.h"
#include <stdio.h>

extern char ** map;
extern int WIDTH;
extern int HEIGHT;
extern const int INDEX_MAX;
extern int DEBUG;
extern const int shapeWidths[];

int fitable(Shape, int);
void fit(State *, Shape);

#endif //TETRIS_FITTING_H
