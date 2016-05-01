#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "utils.h"

int main (int argc, char *argv[]) {
	SERIES_DATABASE *db = NULL;
	int opt = 1;
	
	// Inicializando o arquivo;
	create_file (DB_FILE_NAME, &db);

	while (opt != 0) {

		printf ("==================== SERIES ====================\n");
		printf ("Bem-vindo! Selecione uma das opções abaixo:\n");
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
				generate_random_file (db);
				// TODO: verificação de erros da função generate_random_file ();
				printf ("Arquivos gerados com sucesso!\n");
				break;

			case 2:
				printf ("Digite o ID da série: ");
				searchSeries(db);
				break;

			case 3:
				printf ("Séries contidas no sistema:\n");
				// TODO: criar a função de buscar todas as séries. Programmer: Allan.
				break;

		}

	}

	// Destruindo o arquivo;
	destroy_file (&db);

	return EXIT_SUCCESS;
}
