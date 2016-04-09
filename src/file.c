#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file.h"

struct SERIES_DATABASE {
	// Arquivo com a base de dados de séries;
	FILE *file;
	// Nome do arquivo de base de dados;
	char *name;

	/* Dados dentro do arquivo de base de dados */
	// Identificador único de cada série;
	int idSerie;
	// Título;
	char *tituloSerie;
	// Descrição;
	char *descSerie;
	// País de origem;
	char *producao;
	// Ano de lançamento;
	short int anoLancamento;
	// Número de temporadas;
	char temporada;
	// Gênero;
	char *generoSerie;
};

// TODO: definir as mensagens de erro;
// TODO: verificação name == NULL;
// TODO: verificar malloc;
int create_file (const char *name, SERIES_DATABASE **db) {
	*db = (SERIES_DATABASE *) malloc (sizeof (SERIES_DATABASE));

	// Para não permitir o usuário modificar o nome do arquivo, faço uma cópia
	// do nome para dentro da struct;
	(*db)->name = (char *) malloc (strlen (name)*sizeof (char));
	strcpy ((*db)->name, name);

	return 0;
}

int destroy_file (SERIES_DATABASE **db) {
	if (db != NULL && *db != NULL) {

		// Liberando memória que tenha sido alocada;
		if ((*db)->name != NULL) free ((*db)->name);
		if ((*db)->tituloSerie != NULL) free ((*db)->tituloSerie);
		if ((*db)->descSerie != NULL) free ((*db)->descSerie);
		if ((*db)->producao != NULL) free ((*db)->producao);
		if ((*db)->generoSerie != NULL) free ((*db)->generoSerie);
		if ((*db)->file != NULL) fclose ((*db)->file);

		free (*db);
		*db = NULL;

	}
}
