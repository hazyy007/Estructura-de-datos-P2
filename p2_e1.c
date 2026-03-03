#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "radio.h"

int show_player_menu(Stack *history);
int show_player_menu(Stack *history) {
    int option;
    Music *m = NULL;
	
	/* Obtenemos canción actual del top de la pila */
	m = (Music *)stack_top (history);
    if (m != NULL) {
		music_formatted_print(stdout, m);
    } else {
        printf("\nNo song currently playing.\n");
    }
	
	/* Imprimimos historial (pila) */
	printf("\nRecently Played:\n");
	stack_print(stdout, history, music_plain_print);
	
	/* Mostramos menu y esperamos selección */
    printf("\n1. Back to previous song\n");
    printf("2. Exit\n");
    printf("Choose an option: ");

    scanf("%d", &option);

    return option;
}

/* TODO MAIN FUNCTION */
int main (int argc, char **argv) {
    Radio *r = NULL;
    Stack *history = NULL;
    FILE *f = NULL;
    Music *m = NULL;
    int i, option = 0;

    /*Inicializamos una radio y pila*/
    r = radio_init();
    history = stack_init();
    if (!r || !history)
    {
        radio_free(r);
        stack_free(history);
        return EXIT_FAILURE;
    }

    /*Leer radio desde fichero */
    f = fopen(argv[1], "r");
    if (!f)
    {
        radio_free(r);
        stack_free(history);
        return EXIT_FAILURE;  
    }

    radio_readFromFile(f, r);
    fclose(f);

    /*Insertar canciones de la radio a la pila*/
    for (i = 0; i < radio_getNumberOfMusic(r); i++)
    {
        m = (Music *)radio_getMusicIndex(r, i);
        if (m)
        {
            stack_push(history, m);
        }
    }

    /*Menu interactivo*/
    while (option != 2)
    {
        option = show_player_menu(history);

        if (option == 1)
        {
            /*volver atras*/
            if (!stack_isEmpty(history))
            {
                stack_pop(history);
            }
        }
    }

    /*liberar*/
    radio_free(r);
    stack_free(history);
    return EXIT_SUCCESS; 

}


