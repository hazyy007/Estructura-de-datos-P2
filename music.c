/**
 * @file  music.c
 * @author Profesores EDAT
 * @date February 2026
 * @version 1.0
 * @brief Library to manage ADT Music
 *
 * @details
 *
 * @see
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "music.h"

/* START [STR_LENGTH] */
#define STR_LENGTH 64
/* END [STR_LENGTH] */

/* START [Music] */
struct _Music
{
  long id;
  char title[STR_LENGTH];
  char artist[STR_LENGTH];
  unsigned short duration;
  State state;
  int index;  
};
/* END [Music] */

Music *music_init()
{
  Music *m = (Music *)malloc(sizeof(Music));
  if (!m)
    return NULL;

  m->id = 0;
  strcpy(m->title, "");
  strcpy(m->artist, "");
  m->duration = 0;
  m->state = NOT_LISTENED;
  m->index = -1;

  return m;
}

Status music_setField(Music *m, char *key, char *value)
{
  if (!key || !value)
    return ERROR;

  if (strcmp(key, "id") == 0)
  {
    return music_setId(m, atol(value));
  }
  else if (strcmp(key, "title") == 0)
  {
    return music_setTitle(m, value);
  }
  else if (strcmp(key, "artist") == 0)
  {
    return music_setArtist(m, value);
  }
  else if (strcmp(key, "duration") == 0)
  {
    return music_setDuration(m, atol(value));
  }
  else if (strcmp(key, "state") == 0)
  {
    return music_setState(m, (State)atoi(value));
  }

  return ERROR;
}

Music *music_initFromString(char *descr) {
  Music *m;
  char *p, *key_start, *value_start, *buffer;

  if (!descr) return NULL;

  /* Creamos una copia para no destrozar el string original con strtok/punteros */
  buffer = (char*)malloc(strlen(descr) + 1);
  if (!buffer) return NULL;
  strcpy(buffer, descr);

  m = music_init();
  if (!m) {
    free(buffer);
    return NULL;
  }

  p = buffer;
  while (*p) {
    /* 1. Saltar espacios en blanco iniciales */
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++; 
    if (!*p) break;

    /* 2. Identificar la clave (ej: "id", "title") */
    key_start = p;
    while (*p && *p != ':') p++; 
    if (!*p) break;
    *p++ = '\0'; /* Terminamos la clave con un nulo */

    /* 3. Saltar posibles espacios después de los ':' */
    while (*p == ' ' || *p == '\t') p++; 

    /* 4. Manejo de valores (con o sin comillas) */
    if (*p == '"' || *p == '\'') {
      char quote = *p++; /* Guardamos si es " o ' */
      value_start = p;   /* El valor empieza DESPUÉS de la comilla */
      while (*p && *p != quote) p++;
      if (*p) *p++ = '\0'; /* Quitamos la comilla de cierre y terminamos el string */
    } else {
      /* Si no hay comillas, leemos hasta el siguiente espacio o fin */
      value_start = p;
      while (*p && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r') p++;
      if (*p) *p++ = '\0';
    }

    /* 5. Guardar el campo en la estructura Music */
    /* Ahora value_start contiene "317" en vez de "\"317\"" */
    music_setField(m, key_start, value_start);
  }

  free(buffer);
  return m;
}
void music_free(void *m)
{
  if (m)
  {
    free(m);
  }
}

long music_getId(const Music *m)
{
  if (!m)
  {
    return -1;
  }
  return m->id;
}

const char *music_getTitle(const Music *m)
{
  if (!m)
  {
    return NULL;
  }
  return m->title;
}

const char *music_getArtist(const Music *m)
{
  if (!m)
  {
    return NULL;
  }
  return m->artist;
}

unsigned short music_getDuration(const Music *m)
{
  if (!m)
  {
    return -1;
  }
  return m->duration;
}

/**----------------------------------------------------------------------------------- */
State music_getState(const Music *m)
{
  if (!m)
  {
    return ERROR_MUSIC;
  }
  return m->state;
}

Status music_setId(Music *m, const long id)
{ /**a partir de aqui sergio */
  if (!m || id < 0)
  {
    return ERROR;
  }
  m->id = id;
  return OK;
}

Status music_setTitle(Music *m, const char *title)
{
  if (!m || !title)
  {
    return ERROR;
  }
  if (strlen(title) >= STR_LENGTH)
  {
    return ERROR;
  }
  strcpy(m->title, title);
  return OK;
}

Status music_setArtist(Music *m, const char *artist)
{
  if (!m || !artist)
  {
    return ERROR;
  }
  if (strlen(artist) >= STR_LENGTH)
  {
    return ERROR;
  }
  strcpy(m->artist, artist);
  return OK;
}

Status music_setDuration(Music *m, const unsigned short duration)
{
  if (!m)
  {
    return ERROR;
  }
  m->duration = duration;
  return OK;
}

Status music_setState(Music *m, const State state)
{
  if (!m)
  {
    return ERROR;
  }
  m->state = state;
  return OK;
}

int music_cmp(const void *m1, const void *m2)
{
  const Music *a;
  const Music *b;
  int title_cmp;

  if (!m1 || !m2) {
    return 0;
  }

  a = (const Music *)m1;
  b = (const Music *)m2;

  if (a->id != b->id) {
    return (a->id < b->id) ? -1 : 1;
  }

  title_cmp = strcmp(a->title, b->title);
  if (title_cmp != 0) {
    return title_cmp;
  }

  return strcmp(a->artist, b->artist);
}

void *music_copy(const void *src)
{
  Music *original = (Music *)src;
  Music *copy;

  if (!original)
  {
    return NULL;
  }

  copy = (Music *)malloc(sizeof(Music));
  if (!copy)
  {
    return NULL;
  }

  copy->id = original->id;
  strcpy(copy->title, original->title);
  strcpy(copy->artist, original->artist);
  copy->duration = original->duration;
  copy->state = original->state;

  return copy;
}


int music_plain_print(FILE *pf, const void *m)
{
  Music *aux;

  if (!pf || !m)
    return -1;

  aux = (Music *)m;

  return fprintf(pf, "[%ld, %s, %s, %u, %d]", aux->id, aux->title, aux->artist, aux->duration, aux->state);
}

/**  Remaining functions of music.h to be implemented here **/

int music_formatted_print(FILE *pf, const void *m)
{
  Music *aux;
  int counter = 0, minutes, sec;
  if (!pf || !m)
    return -1;

  aux = (Music *)m;

  if (!aux->duration || aux->duration <= 0)
    return -1;
  minutes = aux->duration / 60;
  sec = aux->duration % 60;

  counter = fprintf(pf, "\t ɴᴏᴡ ᴘʟᴀʏɪɴɢ: %s\n", aux->title);
  counter += fprintf(pf, "\t • Artist %s •\n", aux->artist);
  counter += fprintf(pf, "\t──────────⚪──────────\n");
  counter += fprintf(pf, "\t\t◄◄⠀▐▐ ⠀►►\n");
  counter += fprintf(pf, "\t 0:00 / %02d:%02d ───○ 🔊⠀\n\n", minutes, sec);

  return counter;
}
