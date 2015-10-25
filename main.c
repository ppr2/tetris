

/*
 * #
 * ####  EL1
 *
 * ##
 * #     EL2
 * #
 * #
 *
 * ####  EL3
 *    #
 *
 *  #
 *  #
 *  #    EL4
 * ##
 * */

typedef enum {
    SQUARE, EL1, EL2, EL3, EL4 // TODO zbytek
} shape;

const int WIDTH = 5;
const int HEIGHT = 5;



/*
 * index je modulo sirka mapy
 *
 *
 * */
state * fit(struct current, shape brickType) {
    // for all bricks
    switch(brickType) {
        case SQUARE:
            if () {
                new = fitSquare
                new.map = ;// COPY of curren.
                return new;// nafituj vsechny konfigurace
            }
            break;
        case EL:
            break;
        case STAIRS:
            break;
        case TRIANGLE:
            break;
        case STICK:
            break;
    }
    return NULL;
}

state * fitSquare(struct state current, shape brickType) {
    if (1) {
        state new;
        new.index = current.index + 1;
        map
        new.brickCounts = //COpy of current.brickCounts
        new.brickCounts[0] += 1;
        return &new;
    }
    // pro vsechny konfigurace
    // hod na zasobnik novou mapu, pokud tam jde nafitovat
    jdeNafitovat?
        jo: naser to do new; return 1;
        ne: return NULL;
    stack.push(zkopirovany/pole);


}

/*
 * fit1()
 *  if possible
 *   hod novy na stack
 *   fn1()
 * */

struct state {
    char index;
    char map[][];
    char brickCounts[];
};
typedef struct node {
    struct state;
    struct node * next;
} node_t;

node_t * head = malloc(sizeof(node_t));

int main() {
    short map[WIDTH][HEIGHT] = {0};
    int index, i;
    int x = index / WIDTH - 1;
    int y = index % WIDTH - 1;

    state init;
    init.map = map;
    init.index = 0;
    init.brickCounts = {[0 ... 4] = 0};
    push(head, init);

    state new;
    while (!stackEmpty()) {
        state current = stackPop();
        for (i = 0; i < 5; i++) {
            if (new = fit(current, i)) {
                stackPush(&new);
            }
        }
    }
    /*
     * stack s
     * s.push(prazdna mapa)
     * while stack.notEmpty?
     *  x = stack.pop()
     *  for all bricks // state je nafitovana brick
     *    if fitable?
     *      state = fit(brick)
     *      stack.push(state)
     *
     *
     *
     *
     *
     *
     *
     * s = stack
     * s.push(map)
     * while s not empty
     *  for index=1..n
     *      stav = s.pop
     *      for brick = brick1..brick-n     *
     *          funkce(stav, index, brick)

     * fn1
     *  while neco na zasobniku
     *   mapZeStacku = peek ze stacku
     *
     *   fit1()
     *   fit2()
     *   fit3()
     *   ....
     *   odeber ze stacku
     * */


    for (i = 0; i < WIDTH * HEIGHT; i++) {

        fitSquare();
        fitEl();
        //...
    }

    return 0;
}