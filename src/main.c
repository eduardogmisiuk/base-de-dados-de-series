/*
 * Interface para o programa de séries.
 *
 * Autores:
 * Allan Silva Domingues 	 9293290
 * Eduardo Garcia Misiuk 	 9293230
 * Raul Wagner Martins Costa 9293032
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "utils.h"

int main (int argc, char *argv[]) {
	SERIES_DATABASE *db = NULL;
	int opt = 1;
	int error;
	
	// Inicializando o arquivo;
	create_file (DB_FILE_NAME, &db);

	printf ("==================== SERIES ====================\n");
	printf ("Bem-vindo! Selecione uma das opções abaixo:\n");
	printf ("0 - Sair\n");
	printf ("1 - Gerar arquivo da base de dados aleatório\n");
	printf ("2 - Buscar por uma série\n");
	printf ("3 - Mostrar todas as séries\n");
	printf ("Opção escolhida: ");

	while (opt != 0) {

		printf ("==================== SERIES ====================\n");
		printf ("0 - Sair\n");
		printf ("1 - Gerar arquivo da base de dados aleatório\n");
		printf ("2 - Buscar por uma série\n");
		printf ("3 - Mostrar todas as séries\n");
		printf ("Opção escolhida: ");
		scanf ("%d", &opt);
		getc (stdin);

		switch (opt) {

			case 0: printf ("Finalizando o programa...\n");
				break;

			case 1:
				printf ("Gerando arquivos...\n");
				error = generate_random_file (db);
				// TODO: verificação de erros da função generate_random_file ();
				if (error == 0) printf ("Arquivos gerados com sucesso!\n");
				break;

			case 2:
				printf ("Digite o ID da série: ");
				search_series(db);
				break;

			case 3:
				printf ("Séries contidas no sistema:\n");
				all_series (db);
				break;

			default:
				printf ("Opção %d não é válida!\n", opt);
				break;

		}

	}

	// Destruindo o arquivo;
	destroy_file (&db);

	return EXIT_SUCCESS;
}
