
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "radio.h"
#include "stack.h"
#include "music.h"

int compare_artist(const void *e1, const void *e2){

    const Music *m1 = NULL;
    const Music *m2 = NULL;

    m1 = (const Music *)e1;
    m2 = (const Music *)e2;

    return strcmp(music_getArtist(m1), music_getArtist(m2));
}

Status mergeStacks(Stack *sin1, Stack *sin2, Stack *sout, int (*cmp)(const void *, const void *)){
    void *e1, *e2;
    Stack *ps = NULL;
    if (!sin1 || !sin2 || !sout) return ERROR;

    while (!stack_isEmpty(sin1) && !stack_isEmpty(sin2)) {

        e1 = stack_top(sin1);
        e2 = stack_top(sin2);

        /*Ordeno alfabeticamente por artista*/
        if(cmp(e1, e2) > 0) {
            stack_push(sout, stack_pop(sin1));
        }
        else{
            stack_push(sout, stack_pop(sin2));
        }

    }

    /* Vaciar la pila que haya quedado con elementos [cite: 120, 128] */
    ps = stack_isEmpty(sin1) ? sin2 : sin1;
    while (!stack_isEmpty(ps)) {
        stack_push(sout, stack_pop(ps));
    }

    return OK;
}
int main(int argc, char **argv) {
    Radio *r1 = NULL, *r2 = NULL;
    Stack *s1 = NULL, *s2 = NULL, *sout = NULL;
    FILE *f1 = NULL, *f2 = NULL;
    int i;

    if (argc < 3) {
        return EXIT_FAILURE;
    }

    r1 = radio_init();
    r2 = radio_init();
    s1 = stack_init();
    s2 = stack_init();
    sout = stack_init();

    if (!r1 || !r2 || !s1 || !s2 || !sout) {
        radio_free(r1);
        radio_free(r2);
        stack_free(s1);
        stack_free(s2);
        stack_free(sout);
        return EXIT_FAILURE;
    }

    f1 = fopen(argv[1], "r");
    f2 = fopen(argv[2], "r");

    if (!f1 || !f2) {
        fclose(f1);
        fclose(f2);
        radio_free(r1);
        radio_free(r2);
        stack_free(s1);
        stack_free(s2);
        stack_free(sout);
        return EXIT_FAILURE;
    }

    radio_readFromFile(f1, r1);
    radio_readFromFile(f2, r2);

    fclose(f1);
    fclose(f2);

    for (i = 0; i < radio_getNumberOfMusic(r1); i++) {
        stack_push(s1, radio_getMusicIndex(r1, i));
    }

    for (i = 0; i < radio_getNumberOfMusic(r2); i++) {
        stack_push(s2, radio_getMusicIndex(r2, i));
    }

    if (mergeStacks(s1, s2, sout, compare_artist) == ERROR) {
        radio_free(r1);
        radio_free(r2);
        stack_free(s1);
        stack_free(s2);
        stack_free(sout);
        return EXIT_FAILURE;
    }

    printf("Playlist combined:\n");
    stack_print(stdout, sout, music_plain_print);

    radio_free(r1);
    radio_free(r2);
    stack_free(s1);
    stack_free(s2);
    stack_free(sout);

    return EXIT_SUCCESS;
}