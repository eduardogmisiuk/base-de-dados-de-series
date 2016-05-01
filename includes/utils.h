/*
 * Cabeçalho do código fonte de funções úteis. contém, ainda, define's 
 * muito utilizados no código.
 *
 * Autores:
 * Allan Silva Domingues 	 9293290
 * Eduardo Garcia Misiuk 	 9293230
 * Raul Wagner Martins Costa 9293032
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#define FIELD_SEPARATOR '|'
#define REGISTER_SEPARATOR (signed char) 186

#define ID_SIZE sizeof (int)
#define PRODUCAO_SIZE 60*sizeof (char)
#define ANO_SIZE sizeof (short int)
#define TEMPORADA_SIZE 1*sizeof (char)

#define RANDOM_SERIES "series/series.txt"
#define SIZE_RANDOM_SERIES 100
#define RANDOM_LIMIT 1000

#define DB_FILE_NAME "series/series.db"

char *str_read (FILE *f, char d1, char d2, char d3, char d4);

#endif
