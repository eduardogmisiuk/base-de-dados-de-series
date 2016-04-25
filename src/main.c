#include <stdlib.h>
#include <stdio.h>
#include "db.h"
#include "utils.h"

int main (int argc, char *argv[]) {
	SERIES_DATABASE *db = NULL;
	char *file = str_read (stdin, '\n', '\0', -1, -1);

	create_file (file, &db);

	printf ("Gerando arquivo aleatório...\n");
	generate_random_file (db);

	destroy_file (&db);
	free (file);

	return EXIT_SUCCESS;
}
