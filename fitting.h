#ifndef TETRIS_FITTING_H
#define TETRIS_FITTING_H
#define MAX_HEIGHT 10

#include "state.h"
extern char ** map;
extern int WIDTH;
extern int HEIGHT;
extern const int INDEX_MAX;
extern int DEBUG;
extern const int shapeWidths[];

int fitable(Shape, int);
void fit(State *, Shape);
void printMap(char ** map);

#endif //TETRIS_FITTING_H
