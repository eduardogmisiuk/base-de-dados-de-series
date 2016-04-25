/*------------------------------------------------------------------------/
\       Eduardo Garcia Misiuk | 9293230 | USP - ICMC | 08.11.2015         \
/------------------------------------------------------------------------*/
#ifndef _UTILS_H_
#define _UTILS_H_

#define FIELD_SEPARATOR '|'
#define REGISTER_SEPARATOR 186

#define ID_SIZE sizeof (unsigned int)
#define PRODUCAO_SIZE 60*sizeof (char)
#define ANO_SIZE sizeof (short int)
#define TEMPORADA_SIZE 1*sizeof (char)

#define RANDOM_SERIES "series/series.txt"
#define SIZE_RANDOM_SERIES 3
#define RANDOM_LIMIT 1000

char *str_read (FILE *f, char d1, char d2, char d3, char d4);

#endif
