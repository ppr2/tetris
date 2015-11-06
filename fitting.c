#include "fitting.h"
#include "state.h"
#include <stdio.h>


/*
 * return 0 if space for shape starting at index is not enough
 * */
int fitable(Shape shape, int index) {
    int x = index % WIDTH;
    int y = index / WIDTH;
    int plusX, plusY;

    /* Don't need to check if we are not out of map already
     * since isLeaf function is callled before */

    switch(shape) {
        case SPACE:
            return x < WIDTH && map[x][y] == 0;
        case SQUARE:
            return x + 1 < WIDTH && y + 1 < HEIGHT
                    && map[x][y] == 0             /*        */
                    && map[x+1][y]   == 0         /*   ##   */
                    && map[x][y+1]   == 0         /*   ##   */
                    && map[x+1][y+1] == 0;        /*        */
        case EL1:
            return x - 3 >= 0 && y + 1 < HEIGHT
                    && map[x][y]     == 0         /*     #  EL1 */
                    && map[x][y+1]   == 0         /*  ####      */
                    && map[x-3][y+1] == 0
                    && map[x-2][y+1] == 0
                    && map[x-1][y+1] == 0;
        case EL2:
            return  x + 3 < WIDTH && y + 1 < HEIGHT
                    && map[x][y]       == 0       /*  #     EL2 */
                    && map[x][y+1]     == 0       /*  ####      */
                    && map[x+1][y+1]   == 0
                    && map[x+2][y+1]   == 0
                    && map[x+3][y+1]   == 0;
        case EL3:
        case EL4:
            plusX = shape == EL3 ? 3 : 0;
            return x + 3 < WIDTH && y + 1 < HEIGHT
                    && map[x+plusX][y+1] == 0     /*  ####  EL3 */
                    && map[x][y]         == 0     /*     #      */
                    && map[x+1][y]       == 0
                    && map[x+2][y]       == 0     /*  ####  EL4 */
                    && map[x+3][y]       == 0;    /*  #         */
        case EL5:
            return x - 1 >= 0 && y + 3 < HEIGHT
                   && map[x][y]         == 0      /*   #  EL5   */
                   && map[x][y+1]       == 0      /*   #        */
                   && map[x][y+2]       == 0      /*   #        */
                   && map[x][y+3]       == 0      /*  ##        */
                   && map[x-1][y+3]     == 0;
        case EL6:
            return x + 1 < WIDTH && y + 3 < HEIGHT
                    && map[x][y]         == 0     /*  #  EL6    */
                    && map[x][y+1]       == 0     /*  #         */
                    && map[x][y+2]       == 0     /*  #         */
                    && map[x][y+3]       == 0     /*  ##        */
                    && map[x+1][y+3]     == 0;
        case EL7:
        case EL8:
            plusX = shape == EL7 ? 1 : 0;
            return x + 1 < WIDTH && y + 3 < HEIGHT
                    && map[x][y]         == 0     /* ##      ##     */
                    && map[x+1][y]       == 0     /*  #      #      */
                    && map[x+plusX][y+1] == 0     /*  #      #      */
                    && map[x+plusX][y+2] == 0     /*  # EL7  # EL8  */
                    && map[x+plusX][y+3] == 0;
        case STAIRS1:
            return x + 1 < WIDTH && x - 1 >= 0 && y + 1 < HEIGHT
                    && map[x][y]   == 0
                    && map[x+1][y]   == 0         /*  ## */
                    && map[x-1][y+1]   == 0         /* ##  */
                    && map[x][y+1] == 0;
        case STAIRS2:
            return x + 2 < WIDTH && y + 1 < HEIGHT
                    && map[x][y]     == 0         /* ##  */
                    && map[x+1][y]   == 0         /*  ## */
                    && map[x+1][y+1] == 0
                    && map[x+2][y+1] == 0;
        case STAIRS3:
            return x + 1 < WIDTH && y +2 < HEIGHT
                    && map[x][y]     == 0         /* #  */
                    && map[x][y+1]   == 0         /* ## */
                    && map[x+1][y+1] == 0         /*  # */
                    && map[x+1][y+2] == 0;
        case STAIRS4:
            return x - 1 >= 0 && x < WIDTH && y + 2 < HEIGHT
                    && map[x][y]         == 0       /*  # */
                    && map[x][y+1]       == 0       /* ## */
                    && map[x-1][y+1]     == 0       /* #  */
                    && map[x-1][y+2]     == 0;
        case TRIANGLE1:
            return x - 1 >= 0 && x + 1 < WIDTH && y + 1 < HEIGHT
                    && map[x][y]         == 0     /*  #             */
                    && map[x-1][y+1]     == 0     /* ###            */
                    && map[x][y+1]       == 0     /*     TRIANGLE1  */
                    && map[x+1][y+1]     == 0;
        case TRIANGLE2:
            return x + 2 < WIDTH && y + 1 < HEIGHT
                    && map[x][y]       == 0     /*    ###         */
                    && map[x+1][y]     == 0     /*     #          */
                    && map[x+2][y]     == 0     /*   TRIANGLE2    */
                    && map[x+1][y+1]   == 0;
        case TRIANGLE3:
            return x + 1 < WIDTH && y + 2 < HEIGHT
                   && map[x][y+1]       == 0     /* #               */
                   && map[x+1][y+1]     == 0     /* ##              */
                   && map[x][y]         == 0     /* #               */
                   && map[x][y+2]       == 0;    /*    TRIANGLE3    */
        case TRIANGLE4:
            return x - 1 >= 0 && x < WIDTH && y + 2 < HEIGHT
                    && map[x][y]       == 0     /*                 */
                    && map[x-1][y+1]   == 0     /*  TRIANGLE4   #  */
                    && map[x][y+1]     == 0     /*             ##  */
                    && map[x][y+2]     == 0;    /*              #  */
        case STICK1:
            return x < WIDTH && y + 3 < HEIGHT
                    && map[x][y]   == 0           /*  #    */
                    && map[x][y+1] == 0           /*  #    */
                    && map[x][y+2] == 0           /*  #    */
                    && map[x][y+3] == 0;          /*  #    */
        case STICK2:
            return x + 3 < WIDTH && y < HEIGHT
                    && map[x][y]   == 0           /*       */
                    && map[x+1][y] == 0           /* ####  */
                    && map[x+2][y] == 0           /*       */
                    && map[x+3][y] == 0;          /*       */
        default:
            fprintf(stderr, "INVALID SHAPE TO VERIFY FITTABILITY! shapeToFit=%d, index=%d",
                    shape, index);
            return -1;
    }
}
/*
 * Used to fit or unfit, depends on index and newValue.
 * Replace shape - shapeToFit - at index - index - with shape - newValue
 * */
void fit(State * state, Shape newValue) {
    Shape shapeToFit = state->shape;
    int index = state->index;
    int x = index % WIDTH;
    int y = index / WIDTH;
    int plusX, plusY;

    if (DEBUG) {
        //printf("FITTING! shape=%d at index=%d with newVal=%d\n",
               //state->shape, state->index, newValue);
        //printf("Map before fitting\n");
        //printMap(map);
    }

    switch(shapeToFit) {
        case EMPTY:
            break;
        case SPACE:
            map[x][y]     = newValue;
            break;
        case SQUARE:
            map[x][y]     = newValue;         /*        */
            map[x+1][y]   = newValue;         /*   ##   */
            map[x][y+1]   = newValue;         /*   ##   */
            map[x+1][y+1] = newValue;         /*        */
            break;
        case EL1:
            map[x][y]     = newValue;         /*     #  EL1 */
            map[x][y+1]   = newValue;         /*  ####      */
            map[x-3][y+1] = newValue;
            map[x-2][y+1] = newValue;
            map[x-1][y+1] = newValue;
            break;
        case EL2:
            map[x][y]       = newValue;      /*  #     EL2 */
            map[x][y+1]     = newValue;      /*  ####      */
            map[x+1][y+1]   = newValue;
            map[x+2][y+1]   = newValue;
            map[x+3][y+1]   = newValue;
            break;
        case EL5:
            map[x][y]         = newValue;      /*   #  EL5   */
            map[x][y+1]       = newValue;      /*   #        */
            map[x][y+2]       = newValue;      /*   #        */
            map[x][y+3]       = newValue;      /*  ##        */
            map[x-1][y+3]     = newValue;
            break;
        case EL6:
            map[x][y]         = newValue;     /*  #  EL6    */
            map[x][y+1]       = newValue;     /*  #         */
            map[x][y+2]       = newValue;     /*  #         */
            map[x][y+3]       = newValue;     /*  ##        */
            map[x+1][y+3]     = newValue;
            break;
        case EL7:
        case EL8:
            plusX = shapeToFit == EL7 ? 1 : 0;
            map[x][y]         = newValue;     /* ##      ##     */
            map[x+1][y]       = newValue;     /*  #      #      */
            map[x+plusX][y+1] = newValue;     /*  #      #      */
            map[x+plusX][y+2] = newValue;     /*  # EL7  # EL8  */
            map[x+plusX][y+3] = newValue;
            break;
        case STAIRS1:
            map[x][y]       = newValue;
            map[x+1][y]     = newValue;        /*  ## */
            map[x-1][y+1]   = newValue;        /* ##  */
            map[x][y+1]     = newValue;
            break;
        case STAIRS2:
            map[x][y]     = newValue;         /* ##  */
            map[x+1][y]   = newValue;         /*  ## */
            map[x+1][y+1] = newValue;
            map[x+2][y+1] = newValue;
            break;
        case STAIRS3:
            map[x][y]     = newValue;         /* #  */
            map[x][y+1]   = newValue;         /* ## */
            map[x+1][y+1] = newValue;         /*  # */
            map[x+1][y+2] = newValue;
            break;
        case STAIRS4:
            map[x][y]        = newValue;      /*  # */
            map[x][y+1]      = newValue;      /* ## */
            map[x-1][y+1]    = newValue;      /* #  */
            map[x-1][y+2]    = newValue;
            break;
        case TRIANGLE1:
            map[x][y]         = newValue;    /*  #             */
            map[x-1][y+1]     = newValue;    /* ###            */
            map[x][y+1]       = newValue;    /*     TRIANGLE1  */
            map[x+1][y+1]     = newValue;
            break;
        case TRIANGLE2:
            map[x][y]       = newValue;    /*    ###         */
            map[x+1][y]     = newValue;    /*     #          */
            map[x+2][y]     = newValue;    /*   TRIANGLE2    */
            map[x+1][y+1]   = newValue;
            break;
        case TRIANGLE3:
            map[x][y+1]       = newValue;    /* #               */
            map[x+1][y+1]     = newValue;    /* ##              */
            map[x][y]         = newValue;    /* #               */
            map[x][y+2]       = newValue;    /*    TRIANGLE3    */
            break;
        case TRIANGLE4:
            map[x][y]       = newValue;    /*                 */
            map[x-1][y+1]   = newValue;    /*  TRIANGLE4   #  */
            map[x][y+1]     = newValue;    /*             ##  */
            map[x][y+2]     = newValue;    /*              #  */
            break;
        case STICK1:
            map[x][y]   = newValue;           /*  #    */
            map[x][y+1] = newValue;           /*  #    */
            map[x][y+2] = newValue;           /*  #    */
            map[x][y+3] = newValue;           /*  #    */
            break;
        case STICK2:
            map[x][y]   = newValue;           /*       */
            map[x+1][y] = newValue;           /* ####  */
            map[x+2][y] = newValue;           /*       */
            map[x+3][y] = newValue;           /*       */
            break;
        default:
            //fprintf(stderr, "INVALID SHAPE TO FIT! shapeToFit=%d, index=%d, newValue=%d",
                    //shapeToFit, index, newValue);
            break;
    }
    if (DEBUG) {
        //printf("Map after fitting\n");
        //printMap(map);
    }
}
void printMap(char ** map) {
    int x,y;
    for (x = 0; x < WIDTH*4; x++) {
        printf("=");
    }
    printf("\n");
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            if (map[x][y] < 2) {
                printf("|  |");
            } else {
                printf(map[x][y] > 9 ? "|%d|" : "| %d|", map[x][y]);
            }
        }
        printf("\n");
    }
    for (x = 0; x < WIDTH*4; x++) {
        printf("=");
    }
    printf("\n");
}