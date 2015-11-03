#include "fitting_test.h"
#include "../fitting.h"
#include <stdio.h>

#define _DEBUG 0
#define _WIDTH 5
#define _HEIGHT 5
#define _INDEX_MAX _WIDTH * _HEIGHT
const int WIDTH = _WIDTH;
const int HEIGHT = _HEIGHT;
const int DEBUG = _DEBUG;
const int INDEX_MAX = _INDEX_MAX;

char map[_WIDTH][_HEIGHT] = {0};

int main() {
    int x = 0;
    int y = 0;
    int index = x + y*WIDTH;
    fit(STICK2, index, EL1);
    printMap();
    printf("fitable? %d\n", fitable(EMPTY, 5));

    return 0;
}