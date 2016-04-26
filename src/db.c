#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "db.h"
#include "utils.h"

typedef struct INDEX {
	// Nome do arquivo de índice;
	char *name;
	// Arquivo de índice;
	FILE *index;
	// Número de índices no arquivo;
	int n_index;
} INDEX;

struct SERIES_DATABASE {
	// Nome do arquivo de base de dados;
	char *name;
	// Arquivo com a base de dados de séries;
	FILE *file;
	struct SERIE *s;
	// Número de séries no arquivo;
	int n_series;
};

/* Dados dentro do arquivo de base de dados */
struct SERIE {
	// Identificador único de cada série;
	int idSerie;

	// Título;
	char *tituloSerie;
	int titulo_size;

	// Descrição;
	char *descSerie;
	int desc_size;

	// País de origem;
	char *producao;

	// Ano de lançamento;
	short int anoLancamento;

	// Número de temporadas;
	char temporada;

	// Gênero;
	char *generoSerie;
	int genero_size;
};

// TODO: definir as mensagens de erro;
int create_file (const char *name, SERIES_DATABASE **db) {
	// TODO
	if (name == NULL);

	*db = (SERIES_DATABASE *) malloc (sizeof (SERIES_DATABASE));

	// TODO
	// Caso falhe a alocação de memória para a variável db;
	if (*db == NULL);

	(*db)->name = (char *) malloc ((strlen (name) + 1)*sizeof (char));

	// Para não permitir o usuário modificar o nome do arquivo, copio a string;
	strcpy ((*db)->name, name);

	(*db)->s = NULL;
	(*db)->file = NULL;

	(*db)->n_series = 0;

	return 0;
}

int destroy_file (SERIES_DATABASE **db) {
	if (db != NULL && *db != NULL) {

		// Liberando memória que tenha sido alocada;
		if ((*db)->name != NULL) free ((*db)->name);
		if ((*db)->s != NULL) {

			if ((*db)->s->tituloSerie != NULL) free ((*db)->s->tituloSerie);
			if ((*db)->s->descSerie != NULL) free ((*db)->s->descSerie);
			if ((*db)->s->producao != NULL) free ((*db)->s->producao);
			if ((*db)->s->generoSerie != NULL) free ((*db)->s->generoSerie);

		}

		if ((*db)->file != NULL) fclose ((*db)->file);

		free (*db);
		*db = NULL;

	}

	// TODO
	else {


	}

	return 0;
}

int generate_random_file (SERIES_DATABASE *db) {
	if (db == NULL) return 1;

	// Arquivo .txt com várias séries a serem adicionadas;
	FILE *random_series = NULL;
	// Vetor utilizado para a "randomização" dos IDs das séries;
	char *used = NULL;
	SERIE **series = NULL;
	int i;
	int random;
	char field_delimiter = FIELD_SEPARATOR;
	char record_delimiter = REGISTER_SEPARATOR;
	// Ponteiro para auxiliar no swap entre os registros;
	SERIE *aux = NULL;
	char *prod_aux = NULL;

	// Alocando memória para o vetor utilizado para aleatorizar os IDs;
	used = (char *) calloc (RANDOM_LIMIT, sizeof (char));
	if (used == NULL) return 2;

	// Abrindo o arquivo em que há dados das séries;
	random_series = fopen (RANDOM_SERIES, "r");
	if (random_series == NULL) return 3;

	// Alocando memória para o vetor de informações de séries;
	series = (SERIE **) malloc (SIZE_RANDOM_SERIES*sizeof (SERIE *));
	if (series == NULL) return 4;

	// "Setando" uma seed para números aleatórios;
	srand (time (NULL));

	// Arquivo que armazenará as séries aleatoriamente ordenadas do arquivo .txt acima;
	db->file = fopen (db->name, "w+");
	if (db->file == NULL) return 3;

	for (i = 0; i < SIZE_RANDOM_SERIES; i++) {

		// Alocando memória para a série que será lida;
		series[i] = (SERIE *) malloc (SIZE_RANDOM_SERIES*sizeof (SERIE));

		// Lendo o país de origem da série;
		series[i]->producao = str_read (random_series, '\0', '\n', -1, -1);
		prod_aux = (char *) calloc (60, sizeof (char));
		strcpy (prod_aux, series[i]->producao);
		free (series[i]->producao);
		series[i]->producao = prod_aux;
		prod_aux = NULL;

		// Lendo o ano de lançamento e o número de temporadas da série;
		fscanf (random_series, "%hd\n%c\n", &(series[i]->anoLancamento), &(series[i]->temporada));

		// Lendo o nome da série;
		series[i]->tituloSerie = str_read (random_series, '\0', '\n', -1, -1);
		series[i]->titulo_size = strlen (series[i]->tituloSerie) + 1;

		// lendo a descrição da série;
		series[i]->descSerie = str_read (random_series, '\0', '\n', -1, -1);
		series[i]->desc_size = strlen (series[i]->descSerie) + 1;

		// Lendo o gênero da série;
		series[i]->generoSerie = str_read (random_series, '\0', '\n', -1, -1);
		series[i]->genero_size = strlen (series[i]->generoSerie) + 1;

		// Pegando um valor aleatório para op ID da série lida;
		do random = rand () % RANDOM_LIMIT;
		while (used[random] == 1);
		used[random] = 1;

		// Armazenando o ID aleatório da série;
		series[i]->idSerie = random;

	}

	for (i = 0; i < SIZE_RANDOM_SERIES; i++) {

		// Pegando uma posição aleatória no vetor;
		random = rand () % SIZE_RANDOM_SERIES;

		// Trocando de posições os registros;
		aux = series[random];
		series[random] = series[i];
		series[i] = aux;

	}

	for (i = 0; i < SIZE_RANDOM_SERIES; i++) {

		fwrite (&(series[i]->idSerie), ID_SIZE, 1, db->file);

		fwrite (series[i]->producao, PRODUCAO_SIZE, 1, db->file);

		fwrite (&(series[i]->anoLancamento), ANO_SIZE, 1, db->file);

		fwrite (&(series[i]->temporada), TEMPORADA_SIZE, 1, db->file);

		fwrite (series[i]->tituloSerie, series[i]->titulo_size, 1, db->file);
		fwrite (&field_delimiter, sizeof (char), 1, db->file);

		fwrite (series[i]->descSerie, series[i]->desc_size, 1, db->file);
		fwrite (&field_delimiter, sizeof (char), 1, db->file);

		fwrite (series[i]->generoSerie, series[i]->genero_size, 1, db->file);
		fwrite (&field_delimiter, sizeof (char), 1, db->file);

		fwrite (&record_delimiter, sizeof (char), 1, db->file);

		free (series[i]->producao);
		free (series[i]->tituloSerie);
		free (series[i]->descSerie);
		free (series[i]->generoSerie);
		free (series[i]);

		// Atualizando o número de séries dentro do arquivo;
		db->n_series++;

	}

	free (series);
	free (used);

	fclose (random_series);

	fclose (db->file);
	db->file = NULL;

	return 0;
}
