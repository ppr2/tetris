#ifndef PPR2_TETRIS_MAIN_H
#define PPR2_TETRIS_MAIN_H

/************************************************
 * TYPES DEFINITIONS
 ************************************************/
/*
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
 * --   ##     EL7     --    ##     EL8     --
 * --    #             --   #              --
 * --    #             --   #              --
 * --    #             --   #              --
 * --                  --                  --
 * ------------------------------------------
 * ------------------------------------------
 * --                  --                  --
 * --   ##    STAIRS1  --   ##     STAIRS3 --
 * --  ##              --    ##            --
 * --                  --                  --
 * --   #     STAIRS2  --     #    STAIRS4 --
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
 * --  #    STICK1     -- ##### STICK2     --
 * --  #               --                  --
 * --  #               --                  --
 * --                  --                  --
 * ------------------------------------------
 * ------------------------------------------
 *
 * */

typedef enum {
    SQUARE, EL, STAIRS, TRIANGLE, STICK
} Shape;
typedef enum {
    SQUARE,
    EL1, EL2, EL3, EL4,
    STAIRS1, STAIRS2, STAIRS3, STAIRS4,
    TRIANGLE1, TRIANGLE2, TRIANGLE3, TRIANGLE4,
    STICK1, STICK2
} Rotation;
typedef struct operation {
    unsigned int index;

};
typedef struct state {
    char branched = 0;
    char width;         /* Width of */
};

/************************************************
 * FUNCTION PROTOTYPES
 ************************************************/
/* Stack can be global variable or local passed every time, up to you */
int isStackEmpty();
State * peekFromStack();
void removeFromStack();
void pushToStack(Stack * state);

void branchFrom(State * state);
int isLeaf(State state);
void storeBestScore();
void deleteLastOperation();

#endif //PPR2_TETRIS_MAIN_H
