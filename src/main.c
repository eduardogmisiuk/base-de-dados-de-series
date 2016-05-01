#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "utils.h"

int main (int argc, char *argv[]) {
	SERIES_DATABASE *db = NULL;
	int opt = 1;
	int i, aux; //contador , auxiliar na leitura do retorno da busca de uma serie
	
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
				// TODO: criar a função de busca. Programmer: Raul.
				// Instruções: passar por parâmetro uma struct SERIE e imprimir seus dados aqui na main, depois
				// de feita a busca.
				// Utilizar o retorno da função para retornar erros somente. Trate-os aqui na main.
				// Organize a impressão do jeito que achar melhor.
				aux = searchSeries(db);
				if(aux == -1)
				{
					fprintf(stderr, "A serie nao esta no banco de dados!");
				}
				else if(aux == -2)
				{
					fprintf(stderr,"O registro de series esta vazio!");
				}
				else
				{
					/*printf("Nome: ");
					for(i = 0; i < db->s[aux].titulo_size; i++)
						printf("%c",db->s[aux].tituloSerie);
					printf("\n");
					
					printf("Descricao: ");
					for(i = 0; i < db->s[aux].desc_size; i++)
						printf("%c", db->s[aux].descSerie);
					printf("\n");
					
					printf("Genero: ");
					for(i = 0; i < db->s[aux].genero_size; i++)
						printf("%c", db->s[i].generoSerie);
					printf("\n");
					
					printf("Producao: ");
						printf("%s\n", db->s[i].producao);
						
					printf("Ano de Lancamento: %d", db->s[aux].anoLancamento);
					
					printf("Numero de Temporadas: %c", db->s[aux].temporada);*/
							
				}
				break;

			case 3:
				printf ("Séries contidas no sistema:\n");
				// TODO: criar a função de buscar todas as séries. Programmer: Allan.
				break;

		}

	}

	// TODO: erros.
	// Destruindo o arquivo;
	destroy_file (&db);

	return EXIT_SUCCESS;
}
