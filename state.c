#include <stdio.h>
#include <stdlib.h>

#include "state.h"

State* newState(int index, Shape shape) {
    State* state;
    state = (State*)malloc(sizeof(State));
    state->index = index;
    state->shape = shape;
    return state;
}

void freeState(State state) {
    // free(state->neco)
}
