#ifndef _FILE_H_
#define _FILE_H_

typedef struct SERIES_DATABASE SERIES_DATABASE;

int create_file (const char *name, SERIES_DATABASE **db);
int destroy_file (SERIES_DATABASE **db);

#endif
