#include <stdio.h>
char ** map;
int * arr;
int WIDTH  = 3;//= _WIDTH;
int HEIGHT = 3;//= _HEIGHT;

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
char ** newMap() {
    int i;
    char ** newMap;
    // Initialize map
    newMap = (char **) calloc(WIDTH, WIDTH * sizeof(char *));
    for (i = 0; i < WIDTH; i++) {
        newMap[i] = (char *) calloc(HEIGHT, HEIGHT * sizeof(char));
    }

    return newMap;
}

void freeMap(char ** someMap) {
    int i;

    if (someMap != NULL) {
        for (i = 0; i < WIDTH; i++){
            if (someMap[i] != NULL) {
                free(someMap[i]);
            }
        }
        free(someMap);
    }
}

void copyMap(char ** dest, char ** source) {
    int i;

    for (i = 0; i < WIDTH; i++) {
        memcpy(dest[i], source[i], HEIGHT);
    }
}

void copyMapToIntArray(int * dest, char ** source) {
    int i, j;
    for(i = 0; i < WIDTH; i++) {
        for(j = 0; j < HEIGHT; j++) {
            dest[i*WIDTH+j] = source[i][j];
        }
    }
}


int main(int argc, char *argv[]){
  arr = (int*)malloc(9*sizeof(int));
  /*
  char ** mapVal = newMap();
  mapVal[1][1] = 3;
  mapVal[2][2] = 4;
  map = newMap();
  int i;
  copyMap(map, mapVal);
  printMap(map);

  copyMapToIntArray(arr, map);
  for(int i=0;i<9;i++){
    printf("%d", arr[i]);
  }*/
  int index = 1, *index_p = (int*)&index;
  arr[(*index_p)++] = 4;
  arr[(*index_p)++] = 5;
  for(int i=0;i<9;i++){
    printf("%d", arr[i]);
  }
  return 0;
}
