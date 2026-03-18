/**
 * @file radio.c
 * @author Sergio Romera y Alejandro Dominguez
 * @date February 2026
 * @version 1.0
 * @brief Library to manage the radio ADT
 *
 * @see
 */

#define MAX_MSC 4096
#include "radio.h"
#include <string.h>
#include "stack.h"
struct _Radio
{
    Music *songs[MAX_MSC];            /*Array para canciones*/
    Bool relations[MAX_MSC][MAX_MSC]; /*Matriz para las relaciones*/
    int num_music;                    /*Numero de canciones*/
    int num_relations;                /*Numero de relaciones*/
};

Radio *radio_init()
{
    int i, j;
    Radio *r = (Radio *)malloc(sizeof(Radio));
    if (!r)
    {
        return NULL;
    }

    for (i = 0; i < MAX_MSC; i++)
    {
        r->songs[i] = NULL;
        for (j = 0; j < MAX_MSC; j++)
        {
            r->relations[i][j] = FALSE;
        }
    }
    r->num_music = 0;
    r->num_relations = 0;
    return r;
}

void radio_free(Radio *r)
{
    int i;
    if (r)
    {
        for (i = 0; i < r->num_music; i++)
        {
            music_free(r->songs[i]);
        }
        free(r);
    }
}

Status radio_newMusic(Radio *r, char *desc) {
    Music *new_music = NULL;
    int i;

    if (!r || !desc) return ERROR;

    new_music = music_initFromString(desc);
    if (!new_music) return ERROR;

    /* Comprobar si ya existe por ID */
    for (i = 0; i < r->num_music; i++) {
        if (music_getId(new_music) == music_getId(r->songs[i])) {
            music_free(new_music);
            return OK; /* Ya estaba, no es un error */
        }
    }

    /* Verificar que hay espacio */
    if (r->num_music >= MAX_MSC) {
        music_free(new_music);
        return ERROR;
    }

    music_setIndex(new_music, r->num_music); /* Asignar el índice actual */
    /* Insertar al final del contador actual */
    r->songs[r->num_music] = new_music;
    r->num_music++; 

    return OK;
}

Status radio_newRelation(Radio *r, long orig, long dest)
{
    int id_orig = -1, id_dest = -1, i;
    if (!r)
    {
        return ERROR;
    }
    for (i = 0; i < r->num_music; i++)
    {
        if (music_getId(r->songs[i]) == orig)
        {
            id_orig = i;
        }
        if (music_getId(r->songs[i]) == dest)
        {
            id_dest = i;
        }
        if (id_orig != -1 && id_dest != -1)
        {
            break;
        }
    }

    if (id_orig == -1 || id_dest == -1)
    {
        return ERROR;
    }

    if ((r->relations[id_orig][id_dest]) == FALSE)
    {
        r->relations[id_orig][id_dest] = TRUE;
        r->num_relations++;
    }

    return OK;
}

Bool radio_contains(const Radio *r, long id)
{
    int i;

    for (i = 0; i < r->num_music; i++)
    {
        if (music_getId(r->songs[i]) == id)
        {
            return TRUE;
        }
    }
    return FALSE;
}

int radio_getNumberOfMusic(const Radio *r)
{
    if (!r)
    {
        return -1;
    }

    return r->num_music;
}

int radio_getNumberOfRelations(const Radio *r)
{
    if (!r)
    {
        return -1;
    }

    return r->num_relations;
}

Bool radio_relationExists(const Radio *r, long orig, long dest)
{
    int i, id_orig = -1, id_dest = -1;
    if (!r)
        return FALSE;

    for (i = 0; i < r->num_music; i++)
    {
        if (music_getId(r->songs[i]) == orig)
            id_orig = i;
        if (music_getId(r->songs[i]) == dest)
            id_dest = i;
    }

    if (id_orig != -1 && id_dest != -1)
    {
        return r->relations[id_orig][id_dest];
    }
    return FALSE;
}

int radio_getNumberOfRelationsFromId(const Radio *r, long id)
{
    int i, j, idx = -1, contador = 0;
    if (!r)
        return -1;

    for (i = 0; i < r->num_music; i++)
    {
        if (music_getId(r->songs[i]) == id)
        {
            idx = i;
            break;
        }
    }

    if (idx == -1)
        return -1;

    for (j = 0; j < r->num_music; j++)
    {
        if (r->relations[id][j] == TRUE)
            contador++;
    }
    return contador;
}

long *radio_getRelationsFromId(const Radio *r, long id)
{
    int i, j, k = 0, contador = 0, idx = -1;
    long *array = NULL;

    if (!r)
        return NULL;

    for (i = 0; i < r->num_music; i++)
    {
        if (music_getId(r->songs[i]) == id)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
        return NULL;

    for (j = 0; j < r->num_music; j++)
    {
        if (r->relations[idx][j] == TRUE)
            contador++;
    }

    /* Reserva espacio para los IDs + un terminador -1 para saber cuándo parar */
    array = (long *)calloc(contador + 1, sizeof(long));
    if (!array)
        return NULL;

    for (j = 0; j < r->num_music; j++)
    {
        if (r->relations[idx][j] == TRUE)
        {
            array[k++] = music_getId(r->songs[j]);
        }
    }
    array[k] = -1; /* Marcador de final de array */
    return array;
}

int radio_print(FILE *pf, const Radio *r) {
    int i, j;
    if (!pf || !r) return -1;

    for (i = 0; i < r->num_music; i++) {
        music_plain_print(pf, r->songs[i]); /* Imprime [id, titulo...] */
        fprintf(pf, ":"); /* Los dos puntos pegados al corchete */

        for (j = 0; j < r->num_music; j++) {
            if (r->relations[i][j] == TRUE) {
                fprintf(pf, " "); /* Espacio separador */
                music_plain_print(pf, r->songs[j]);
            }
        }
        fprintf(pf, "\n");
    }
    return i;
}

Status radio_readFromFile(FILE *fin, Radio *r) {
    int i, total_a_leer;
    char desc[1024];
    long id_orig, id_dest;

    if (!fin || !r) return ERROR;

    /* 1. Leer cuántas canciones vienen (sin tocar r->num_music directamente) */
    if (fscanf(fin, "%d", &total_a_leer) != 1) return ERROR;
    
    /* LIMPIEZA: saltar el resto de la línea del número */
    fgets(desc, sizeof(desc), fin); 

    /* 2. Leer descripciones */
    for (i = 0; i < total_a_leer; i++) {
        if (fgets(desc, sizeof(desc), fin) == NULL) break;
        desc[strcspn(desc, "\r\n")] = '\0';
        
        if (desc[0] != '\0') 
        {
            radio_newMusic(r, desc);
        }
    }

    /* 3. Leer relaciones */
    /* fscanf es inteligente: se salta cualquier espacio o salto de línea */
    while (fscanf(fin, "%ld %ld", &id_orig, &id_dest) == 2) {
        radio_newRelation(r, id_orig, id_dest);
    }

    return OK;
}

Music *radio_getMusicIndex(const Radio *r, int index)
{
    if (!r || index < 0 ||index >= r->num_music)
    {
        return NULL;
    }

    return r->songs[index];
}

Status radio_depthSearch(Radio *r, long from_id, long to_id)
{
    int i, u;
    Music *m_origin = NULL, *current = NULL, *adj = NULL;
    Stack *s = NULL;
    Bool found = FALSE;

    if (!r)
    {
        return ERROR;
    }
    

    /*Recorremos las canciones de la radio*/
    for (i = 0; i < r->num_music; i++) {
        if (music_getId(r->songs[i]) == from_id) 
        {
            m_origin = r->songs[i];
        }
        /*Las ponemos not listen*/
        music_setState(r->songs[i], NOT_LISTENED);
    }

    if (!m_origin || radio_contains(r, to_id))
    {
        return ERROR;
    }

    /*Imprimir cabecera*/
    printf("From music with id: %ld", from_id);
    printf("To music with id: %ld", to_id);
    printf("Musci exploration path:\n");

    /*Inicializamos la pila*/
    s = stack_init();
    if (!s)
    {
        return ERROR;
    }

    music_setState(m_origin, LISTENED);
    stack_push(s, m_origin);

    while (!stack_isEmpty(s) && found == FALSE) {
        /* Extraemos el elemento en la cima de la pila */
        current = (Music *)stack_pop(s);
        
        /* Imprimimos la canción actual (cada paso del camino se imprime) */
        music_plain_print(stdout, current);
        printf("\n");

        /* Comprobamos si hemos llegado al destino */
        if (music_getId(current) == to_id) {
            found = TRUE;
        } else {
            u = music_getIndex(current); 
            
            /* Buscamos todos sus "vecinos" (relaciones) */
            for (i = 0; i < r->num_music; i++) {
                if (r->relations[u][i] == TRUE) {
                    adj = r->songs[i];
                    /* Si la canción vecina no ha sido visitada aún... */
                    if (music_getState(adj) == NOT_LISTENED) {
                        music_setState(adj, LISTENED); /* La marcamos */
                        stack_push(s, adj);            /* Y a la pila */
                    }
                }
            }
        }
    }

    /* 6. Limpieza final */
    stack_free(s);
    return OK;
}