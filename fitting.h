#ifndef TETRIS_FITTING_H
#define TETRIS_FITTING_H
#define MAX_HEIGHT 10

#include "state.h"

extern char map[5][5];
extern const int WIDTH;
extern const int HEIGHT;
extern const int INDEX_MAX;
extern const int DEBUG;
extern const int shapeWidths[];

int fitable(Shape, int);
void fit(Shape, int, Shape);
void printMap();

#endif //TETRIS_FITTING_H
