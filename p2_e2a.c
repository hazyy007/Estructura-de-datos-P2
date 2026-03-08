#include <stdio.h>
#include <stdlib.h>

#include "radio.h"
#include "stack.h"
#include "music.h"

Status mergeStacks(Stack *sin1, Stack *sin2, Stack *sout) {
    void *e1, *e2;
    Stack *ps = NULL;
    if (!sin1 || !sin2 || !sout) return ERROR;

    while (!stack_isEmpty(sin1) && !stack_isEmpty(sin2)) {
        e1 = stack_top(sin1);
        e2 = stack_top(sin2);

        /* Comparamos duraciones para decidir cuál va a la pila combinada [cite: 106] */
        if (music_getDuration((Music*)e1) > music_getDuration((Music*)e2)) {
            stack_push(sout, stack_pop(sin1));
        } else {
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

int main(int argc, char **argv)
{
    Radio *r1 = NULL, *r2 = NULL;
    Stack *s1 = NULL, *s2 = NULL, *sout = NULL;
    FILE *f1 = NULL, *f2 = NULL;
    int i;
    r1 = radio_init();
    r2 = radio_init();
    s1 = stack_init();
    s2 = stack_init();
    sout = stack_init();
    if (!r1 || !r2 || !s1 || !s2 || !sout) 
    {
        /* Limpieza básica en caso de error de memoria */
        radio_free(r1); 
        radio_free(r2);
        stack_free(s1); 
        stack_free(s2); 
        stack_free(sout);
        return EXIT_FAILURE;
    }
    /*Compruebo que los archivos se inician*/
    if(argc < 3){
        fprintf(stderr, "Error al iniciar");
        return EXIT_FAILURE;
    }
    f1 = fopen(argv[1], "r");
    f2 = fopen(argv[2], "r");
    if (!f1 || !f2)
    {
        radio_free(r1); 
        radio_free(r2);
        stack_free(s1); 
        stack_free(s2); 
        stack_free(sout);
        return EXIT_FAILURE;
    }

    radio_readFromFile(f1, r1);
    radio_readFromFile(f2, r2);

    /*Insertamos las canciones de las radios en las pilas*/
    for (i = 0; i < radio_getNumberOfMusic(r1); i++) 
    {
        stack_push(s1, radio_getMusicIndex(r1, i));
    }
    for (i = 0; i < radio_getNumberOfMusic(r2); i++) 
    {
        stack_push(s2, radio_getMusicIndex(r2, i));
    }

    /* Mostrar pilas iniciales antes de combinar*/
    printf("Playlist 0:\n");
    stack_print(stdout, s1, music_plain_print);
    
    printf("\nPlaylist 1:\n");
    stack_print(stdout, s2, music_plain_print);
    /*Combinamos las pilas*/
    if (mergeStacks(s1, s2, sout) == OK)
    {
        printf("\nPlaylist combined:\n");
        stack_print(stdout, sout, music_plain_print);
    }
    else
    {
        fprintf(stderr, "Error al combinar las pilas.\n");
    }
    /*Liberamos memoria*/
    radio_free(r1); 
    radio_free(r2);
    stack_free(s1); 
    stack_free(s2); 
    stack_free(sout);

    return EXIT_SUCCESS;

    
}