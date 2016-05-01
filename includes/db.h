/*
 * Cabeçalho do código fonte da implementação da base de dados.
 *
 * Autores:
 * Allan Silva Domingues 	 9293290
 * Eduardo Garcia Misiuk 	 9293230
 * Raul Wagner Martins Costa 9293032
 */

#ifndef _DB_H_
#define _DB_H_

typedef struct SERIES_DATABASE SERIES_DATABASE;
typedef struct SERIE SERIE;

int create_file (const char *name, SERIES_DATABASE **db);
void destroy_file (SERIES_DATABASE **db);
int generate_random_file (SERIES_DATABASE *db);
int search_series(SERIES_DATABASE* db);
void print_serie (SERIE *s);
int create_serie (SERIE **s);
void remove_serie (SERIE **s);
int all_series (SERIES_DATABASE *db);

#endif
