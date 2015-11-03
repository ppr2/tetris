#include "fitting.h"
#include "tetris.h"
#include <stdio.h>


/*
 * return 0 if it doesn't fit
 * */
int fitable(Shape shape, int index) {
    int x = index / WIDTH - 1;
    int y = index % WIDTH - 1;

    switch(shape) {
        case EMPTY:
            return index + 1 < INDEX_MAX;
        case SQUARE:
            return x + 2 < WIDTH && y + 1 < HEIGHT
                   && map[x][y] == 0   && map[x+1][y] == 0
                   && map[x][y+1] == 0 && map[x+1][y+1] == 0;
        case EL1:
            return x + 3 < WIDTH && y + 1 < HEIGHT
                   && map[x][y] == 0     && map[x+1][y] == 0
                   && map[x+1][y] == 0   && map[x+2][y] == 0
                   && map[x+3][y] == 0   && map[x][y+1] == 0
                   && map[x+1][y+1] == 0 && map[x+2][y+1] == 0
                   && map[x+3][y+1];
            // TODO
    }
}
/*
 * Used to fit or unfit, depends on index and newValue.
 * Replace shape - shapeToFit - at index - index - with shape - newValue
 * */
void fit(Shape shapeToFit, int index, Shape newValue) {
    int x = index / WIDTH - 1;
    int y = index % WIDTH - 1;
    int plusX, plusY;

    switch(shapeToFit) {
        case EMPTY:
            break;
        case SQUARE:
            map[x][y]     = newValue;         /*        */
            map[x+1][y]   = newValue;         /*   ##   */
            map[x][y+1]   = newValue;         /*   ##   */
            map[x+1][y+1] = newValue;         /*        */
            break;
        case EL1:
        case EL2:
            plusX = shapeToFit == EL1 ? 3 : 0;
            map[x+plusX][y] = newValue;       /*     #  EL1 */
            map[x][y+1]     = newValue;       /*  ####      */
            map[x+1][y+1]   = newValue;
            map[x+2][y+1]   = newValue;       /*  #     EL2 */
            map[x+3][y+1]   = newValue;       /*  ####      */
            break;
        case EL3:
        case EL4:
            plusX = shapeToFit == EL3 ? 3 : 0;
            map[x+plusX][y+1] = newValue;     /*  ####  EL3 */
            map[x][y]         = newValue;     /*     #      */
            map[x+1][y]       = newValue;
            map[x+2][y]       = newValue;     /*  ####  EL4 */
            map[x+3][y]       = newValue;     /*  #         */
            break;
        case EL5:
        case EL6:
            plusX = shapeToFit == EL5 ? 1 : 0;
            map[x+plusX][y]   = newValue;     /*  # EL5  #  EL6 */
            map[x+plusX][y+1] = newValue;     /*  #      #      */
            map[x+plusX][y+2] = newValue;     /*  #      #      */
            map[x][y+3]       = newValue;     /* ##      ##     */
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
            map[x+1][y]   = newValue;         /*  ## */
            map[x+2][y]   = newValue;         /* ##  */
            map[x][y+1]   = newValue;
            map[x+1][y+1] = newValue;
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
            map[x+1][y]     = newValue;       /*  # */
            map[x+1][y+1]   = newValue;       /* ## */
            map[x][y+1]     = newValue;       /* #  */
            map[x][y+2]     = newValue;
            break;
        case TRIANGLE1:
        case TRIANGLE2:
            plusY = shapeToFit == TRIANGLE1 ? 1 : 0;
            map[x+1][y]       = newValue;     /*  # TRIANGLE1   */
            map[x+1][y+1]     = newValue;     /* ###            */
            map[x][y+plusY]   = newValue;     /*            ### */
            map[x+2][y+plusY] = newValue;     /*   TRIANGLE2 #  */
            break;
        case TRIANGLE3:
        case TRIANGLE4:
            plusX = shapeToFit == TRIANGLE3 ? 0 : 1;
            map[x][y+1]       = newValue;     /* # TRIANGLE3     */
            map[x+1][y+1]     = newValue;     /* ##            # */
            map[x+plusX][y]   = newValue;     /* #            ## */
            map[x+plusX][y+2] = newValue;     /*     TRIANGLE4 # */
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
            fprintf(stderr, "INVALID SHAPE TO FIT! shapeToFit=%d, index=%d, newValue=%d",
                    shapeToFit, index, newValue);
            break;
    }
    if (DEBUG) {
        printf("New shape fitted\n");
        printMap();
    }
}
void printMap() {
    int x,y;
    for (x = 0; x < WIDTH*4; x++) {
        printf("=");
    }
    printf("\n");
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            printf("|%.2d|", map[x][y]);
        }
        printf("\n");
    }
    for (x = 0; x < WIDTH*4; x++) {
        printf("=");
    }
    printf("\n");
}