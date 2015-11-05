#include <stdio.h>
#include <stdlib.h>

#include "state.h"

const int shapeWidths[] = {1, 2, 4, 4, 4, 4, 2, 2, 2, 2, 3, 3, 2, 2, 3, 3, 2, 2, 1, 4};

State* newState(int index, Shape shape) {
    State* state;
    state = (State*)malloc(sizeof(State));
    state->index = index;
    state->shape = shape;
    return state;
}

void freeState(State* state) {
    free(state);
}

int getSimpleShape(Shape shape) {
    switch (shape) {
        case EMPTY:
            return 0;
        case SQUARE:
            return 1;
        case EL1:
        case EL2:
        case EL3:
        case EL4:
        case EL5:
        case EL6:
        case EL7:
        case EL8:
            return 2;
        case STAIRS1:
        case STAIRS2:
        case STAIRS3:
        case STAIRS4:
            return 3;
        case TRIANGLE1:
        case TRIANGLE2:
        case TRIANGLE3:
        case TRIANGLE4:
            return 4;
        case STICK1:
        case STICK2:
            return 5;
        default:
            fprintf(stderr, "INVALID SHAPE shape=%d in getSimpleShape\n", shape);
            exit(1);
    }
}
