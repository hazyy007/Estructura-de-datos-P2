
#include <stdio.h>
#include <stdlib.h>

#include "radio.h"
#include "stack.h"

int compare_artist(const void *e1, const void *e2){

}

Status mergeStacks(Stack *sin1, Stack *sin2, Stack *sout) {
    void *e1, *e2;
    Stack *ps = NULL;
    if (!sin1 || !sin2 || !sout) return ERROR;

    while (!stack_isEmpty(sin1) && !stack_isEmpty(sin2)) {
        e1 = stack_top(sin1);
        e2 = stack_top(sin2);

        /*Ordeno alfabeticamente por artista*/

    }

    if(cmp())

    /* Vaciar la pila que haya quedado con elementos [cite: 120, 128] */
    ps = stack_isEmpty(sin1) ? sin2 : sin1;
    while (!stack_isEmpty(ps)) {
        stack_push(sout, stack_pop(ps));
    }

    return OK;
}