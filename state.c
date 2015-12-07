#include "state.h"

const int shapeWidths[] = {0, 1, 2, 1, 1, 4, 4, 1, 1, 2, 2, 2, 2, 1, 1, 1, 3, 1, 1, 1, 4};

State* newState(int index, Shape shape, int depth) {
    State* state;
    state = (State*)malloc(sizeof(State));
    state->index = index;
    state->shape = shape;
    state->depth = depth;
    return state;
}

void getArrayFromState(int *arr, State *state, int *index){
    arr[(*index)++] = state->index;
    arr[(*index)++] = (int)state->shape;
    arr[(*index)++] = state->depth;
    free(state);
}

State getStateFromArray(int *arr, int *index){
    int i = *index;

    int stateIndex = arr[i++];
    Shape shape = (Shape)arr[i++];
    int depth = arr[i++];

    *index = i;
    return *newState(stateIndex, shape, depth);
}

int getSimpleShape(Shape shape) {
    switch (shape) {
        case EMPTY:
            return 0;
        case SPACE:
            return 1;
        case SQUARE:
            return 2;
        case EL1:
        case EL2:
        case EL3:
        case EL4:
        case EL5:
        case EL6:
        case EL7:
        case EL8:
            return 3;
        case STAIRS1:
        case STAIRS2:
        case STAIRS3:
        case STAIRS4:
            return 4;
        case TRIANGLE1:
        case TRIANGLE2:
        case TRIANGLE3:
        case TRIANGLE4:
            return 5;
        case STICK1:
        case STICK2:
            return 6;
        default:
            fprintf(stderr, "INVALID SHAPE shape=%d in getSimpleShape\n", shape);
            exit(1);
    }
}
