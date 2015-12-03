#ifndef TETRIS_STATE_H
#define TETRIS_STATE_H

/************************************************
 * TYPES DEFINITIONS
 ************************************************/
/*
 * ------------------------------------------
 * ------------------------------------------
 * --                  --                  --
 * --   EMPTY is 1x1   --   ##   SQUARE    --
 * --   blank field    --   ##             --
 * --                  --                  --
 * --                  --                  --
 * --                  --                  --
 * ------------------------------------------
 * ------------------------------------------
 * --                  --                  --
 * --      #   EL1     --   #      EL2     --
 * --   ####           --   ####           --
 * --                  --                  --
 * --   ####   EL3     --   ####   EL4     --
 * --      #           --   #              --
 * --                  --                  --
 * --    #     EL5     --   #      EL6     --
 * --    #             --   #              --
 * --    #             --   #              --
 * --   ##             --   ##             --
 * --                  --                  --
 * --   ##     EL7     --   ##     EL8     --
 * --    #             --   #              --
 * --    #             --   #              --
 * --    #             --   #              --
 * --                  --                  --
 * ------------------------------------------
 * ------------------------------------------
 * --                  --                  --
 * --   ##    STAIRS1  --   ##     STAIRS2 --
 * --  ##              --    ##            --
 * --                  --                  --
 * --   #     STAIRS3  --     #    STAIRS4 --
 * --   ##             --    ##            --
 * --    #             --    #             --
 * --                  --                  --
 * ------------------------------------------
 * ------------------------------------------
 * --                  --                  --
 * --  #    TRIANGLE1  --  ###   TRIANGLE2 --
 * -- ###              --   #              --
 * --                  --                  --
 * --                  --                  --
 * --  #    TRIANGLE3  --   #   TRIANGLE4  --
 * --  ##              --  ##              --
 * --  #               --   #              --
 * --                  --                  --
 * ------------------------------------------
 * ------------------------------------------
 * --                  --                  --
 * --  #               --                  --
 * --  #    STICK1     -- #### STICK2      --
 * --  #               --                  --
 * --  #               --                  --
 * --                  --                  --
 * ------------------------------------------
 * ------------------------------------------
 *
 * */

typedef enum {
    EMPTY = 0, SPACE, SQUARE,
    EL1, EL2, EL3, EL4, EL5, EL6, EL7, EL8,
    STAIRS1, STAIRS2, STAIRS3, STAIRS4,
    TRIANGLE1, TRIANGLE2, TRIANGLE3, TRIANGLE4,
    STICK1, STICK2 // =20
} Shape;

/*
 * e.g. for map[3][3]: index = 0, shape = SQUARE
 * | 1|| 1|| 0|
 * | 1|| 1|| 0|
 * | 0|| 0|| 0|
 * */
typedef struct{
    int index;   // Index of top left corner where shape is placed
    Shape shape; // Shape placed at index
    int depth; // Depth on stack
} State;


State* newState(int index, Shape shape, int depth);
State getStateFromArray(int *arr, int *index);
void getArrayFromState(int *arr, State *state, int *index);
int getSimpleShape(Shape shape);


#endif //TETRIS_STATE_H
