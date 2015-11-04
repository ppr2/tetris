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
