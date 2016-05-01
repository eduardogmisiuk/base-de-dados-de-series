#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "db.h"
#include "utils.h"
#include "err_msg.h"

/**
 * Arquivo da base de dados de séries.
 */
struct SERIES_DATABASE {
	// Nome do arquivo de base de dados;
	char *name;
	// Arquivo com a base de dados de séries;
	FILE *file;
	struct SERIE *s;
	// Número de séries no arquivo;
	int n_series;
};

/**
 * Dados dentro do arquivo de base de dados.
 */
struct SERIE {
	// Identificador único de cada série;
	int idSerie;

	// Título;
	char *tituloSerie;
	// Tamanho;
	int titulo_size;

	// Descrição;
	char *descSerie;
	// Tamanho;
	int desc_size;

	// País de origem;
	char *producao;

	// Ano de lançamento;
	short int anoLancamento;

	// Número de temporadas;
	char temporada;

	// Gênero;
	char *generoSerie;
	// Tamanho;
	int genero_size;
};

/**
 * Inicializa tudo o que é necessário para criar a base de dados.
 *
 * name - nome do arquivo da base de dados;
 * db - endereço da base de dados.
 *
 * Retorno:
 * 0 - Sucesso ao criar a base de dados;
 * 2 - Falha de alocação.
 * 3 - Parâmetro inválido;
 */
int create_file (const char *name, SERIES_DATABASE **db) {
	if (name == NULL){
		fprintf (stderr, ERROR_INVALID_NAME);
		return INVALID_NAME;
	}

	*db = (SERIES_DATABASE *) malloc (sizeof (SERIES_DATABASE));

	// Caso falhe a alocação de memória para a variável db;
	if (*db == NULL){
		fprintf (stderr, ERROR_ALLOCATION);
		return ALLOCATION;
		//erro indicado pelo retorno do valor -1
	}

	(*db)->name = (char *) malloc ((strlen (name) + 1)*sizeof (char));
	if ((*db)->name == NULL) {
		free (*db);
		*db = NULL;
		fprintf (stderr, ERROR_ALLOCATION);
		return ALLOCATION;
	}

	// Para não permitir o usuário modificar o nome do arquivo, copio a string;
	strcpy ((*db)->name, name);

	(*db)->s = NULL;
	(*db)->file = NULL;
	(*db)->n_series = 0;

	return 0;
}

/**
 * Libera toda a memória utilizada na base de dados.
 *
 * db - endereço da base de dados.
 *
 * Autor: Eduardo Garcia Misiuk.
 */
void destroy_file (SERIES_DATABASE **db) {
	if (db != NULL && *db != NULL) {

		// Liberando memória que tenha sido alocada;
		if ((*db)->name != NULL) free ((*db)->name);
		// Caso tenha algum dado na struct SERIE, será desalocada aqui;
		if ((*db)->s != NULL) {

			if ((*db)->s->tituloSerie != NULL) free ((*db)->s->tituloSerie);
			if ((*db)->s->descSerie != NULL) free ((*db)->s->descSerie);
			if ((*db)->s->producao != NULL) free ((*db)->s->producao);
			if ((*db)->s->generoSerie != NULL) free ((*db)->s->generoSerie);

		}

		// Caso o arquivo esteja aberto, vamos fechá-lo;
		if ((*db)->file != NULL) fclose ((*db)->file);

		// Liberando toda a estrutura;
		free (*db);
		*db = NULL;

	}
}

/**
 * Busca um registro com o ID passado pelo usuário.
 *
 * db - arquivo da base de dados.
 *
 * Autor: Raul Wagner Martins Costa.
 *
 * Retorno:
 *  0 - Sucesso na busca
 *  1 - Erro na abertura de arquivo
 *  4 - O Id fornecido é invalido
 */
int searchSeries(SERIES_DATABASE* db)
{
	int id;
	char c;
	int aux;//auxiliar na leitura do id
	scanf("%d", &id);
    fgetc(stdin);
    if(db->file == NULL)
		db->file = fopen(db->name, "r");
	if(db->file == NULL)
	{
        fprintf(stderr, ERROR_OPENING_FILE);
        return OPENING_FILE;
	}
	while(!feof(db->file))
	{
		while((c = fgetc(db->file) != 186) && (!feof(db->file)))
		{
			fread(&(db->s->idSerie) , ID_SIZE, 1, db->file);
			if(db->s->idSerie == aux)
			{
				fread(&(db->s->producao), PRODUCAO_SIZE,1, db->file);
				fread(&(db->s->anoLancamento), ANO_SIZE,1, db->file);
				fread(&(db->s->temporada), TEMPORADA_SIZE, 1, db->file);
				
				db->s->tituloSerie = str_read(db->file, FIELD_SEPARATOR, -1, -1, -1);
				db->s->descSerie = str_read(db->file, FIELD_SEPARATOR, -1, -1, -1);
				db->s->generoSerie = str_read(db->file, FIELD_SEPARATOR, -1, -1, -1);
				
				db->s->titulo_size = strlen(db->s->tituloSerie) +1;
				db->s->desc_size = strlen(db->s->descSerie) +1;
				db->s->genero_size = strlen(db->s->generoSerie) +1;
				print_serie(db->s);
				fclose(db->file);
				return 0;//operacao funcionou!
			}
		}
	}
	fclose(db->file);
	fprintf(stderr, ERROR_ID_NOT_FOUND);
	return INVALID_ID;
}

/**
 * Cria um arquivo com 100 registros de séries totalmente aleatório.
 *
 * db - arquivo da base de dados.
 *
 * Retorno:
 * 0 - Sucesso na geração;
 * 1 - Erro na abertura de arquivo;
 * 2 - Erro de alocação de memória;
 *
 * Autores: Allan Silva Domingues e Eduardo Garcia Misiuk.
 */
int generate_random_file (SERIES_DATABASE *db) {
	// Arquivo .txt com várias séries a serem adicionadas;
	FILE *random_series = fopen (RANDOM_SERIES, "r");
	if (random_series == NULL) {
		fprintf (stderr, ERROR_OPENING_FILE);
		return OPENING_FILE;
	}
	// Vetor utilizado para a "randomização" dos IDs das séries.
	// Quando colocar em uma posição aleatória o registro, vou nesta posição neste vetor e marco como utilizada,
	// para não sobrescrever nenhum dado e manter realmente aleatorizado;;
	char *used = (char *) calloc (RANDOM_LIMIT, sizeof (char));
	if (used == NULL) {
		fclose (random_series);
		random_series = NULL;
		fprintf (stderr, ERROR_ALLOCATION);
		return ALLOCATION;
	}
	int i; // Contador;
	int random; // Número aleatório;
	char field_delimiter = FIELD_SEPARATOR; // Delimitador de campos;
	char record_delimiter = REGISTER_SEPARATOR; // Delimitador de registros;
	// Vetor com as séries;
	SERIE **series = (SERIE **) malloc (SIZE_RANDOM_SERIES*sizeof (SERIE *));
	if (series == NULL) {
		fclose (random_series);
		random_series = NULL;
		free (used);
		used = NULL;
		fprintf (stderr, ERROR_ALLOCATION);
		return ALLOCATION;
	}
	// Ponteiro para auxiliar no swap entre os registros;
	SERIE *aux = NULL;
	// Para evitar um warning durante a execução do programa (ele fala que está adicionando valores não
	// inicializados no arquivo), usaremos este vetor para já inicializar todas as 60 posições do vetor como 0;
	char *prod_aux = NULL;

	// Inicializando a seed dos números aleatórios;
	srand (time (NULL));

	// Arquivo que armazenará as séries aleatoriamente ordenadas do arquivo .txt acima;
	db->file = fopen (db->name, "w+");
	if (db->file == NULL) {
		fclose (random_series);
		random_series = NULL;
		free (used);
		used = NULL;
		free (series);
		series = NULL;
		fprintf (stderr, ERROR_OPENING_FILE);
		return OPENING_FILE;
	}

	// Leitura das séries que estão no arquivo .txt;
	for (i = 0; i < SIZE_RANDOM_SERIES; i++) {

		// Alocando memória para a série que será lida;
		series[i] = (SERIE *) malloc (SIZE_RANDOM_SERIES*sizeof (SERIE));

		// Lendo o país de origem da série;
		series[i]->producao = str_read (random_series, '\0', '\n', -1, -1);
		// Realizando a inicialização do vetor para suprimir o warning, como explicado acima;
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

	// Aleatorizando a posição das séries;
	for (i = 0; i < SIZE_RANDOM_SERIES; i++) {

		// Pegando uma posição aleatória no vetor;
		random = rand () % SIZE_RANDOM_SERIES;

		// Trocando de posições os registros;
		aux = series[random];
		series[random] = series[i];
		series[i] = aux;

	}

	// Adicionando ao arquivo as séries;
	for (i = 0; i < SIZE_RANDOM_SERIES; i++) {

		// ID;
		fwrite (&(series[i]->idSerie), ID_SIZE, 1, db->file);

		// País de origem;
		fwrite (series[i]->producao, PRODUCAO_SIZE, 1, db->file);

		// Ano de lançamento;
		fwrite (&(series[i]->anoLancamento), ANO_SIZE, 1, db->file);

		// Número de temporadas;
		fwrite (&(series[i]->temporada), TEMPORADA_SIZE, 1, db->file);

		// Título;
		fwrite (series[i]->tituloSerie, series[i]->titulo_size, 1, db->file);
		// Delimitador de campos;
		fwrite (&field_delimiter, sizeof (char), 1, db->file);

		// Descrição;
		fwrite (series[i]->descSerie, series[i]->desc_size, 1, db->file);
		// Delimitador de campos;
		fwrite (&field_delimiter, sizeof (char), 1, db->file);

		// Gênero;
		fwrite (series[i]->generoSerie, series[i]->genero_size, 1, db->file);
		// Delimitador de campos;
		fwrite (&field_delimiter, sizeof (char), 1, db->file);

		// Delimitador de registros;
		fwrite (&record_delimiter, sizeof (char), 1, db->file);

		// Incrementando o número de séries no arquivo;
		db->n_series++;

		free (series[i]->producao);
		free (series[i]->tituloSerie);
		free (series[i]->descSerie);
		free (series[i]->generoSerie);
		free (series[i]);

	}

	free (series);

	free (used);

	fclose (random_series);
	fclose (db->file);
	db->file = NULL;

	return 0;
}
/**
 * Imprime uma dada série.
 *
 * s - série a ser impressa.
 */
void print_serie (SERIE *s) {
	printf ("ID: %d\n", s->idSerie);
	printf ("Título: %s\n", s->tituloSerie);
	printf ("Descrição: %s\n", s->descSerie);
	printf ("País de produção: %s\n", s->producao);
	printf ("Ano de lançamento: %d\n", s->anoLancamento);
	printf ("Número de temporadas: %d\n", s->temporada);
	printf ("Gênero: %s\n", s->generoSerie);
}
