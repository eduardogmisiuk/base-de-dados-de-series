/*
 * Biblioteca com funções úteis para a implementação do trabalho.
 *
 * Autores:
 * Allan Silva Domingues 	 9293290
 * Eduardo Garcia Misiuk 	 9293230
 * Raul Wagner Martins Costa 9293032
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

/*-------------------------------------------------------------------------------->
Função str_read (): recebe um FILE * e armazena seu conteúdo, até chegar em um dos
delimitadores d1, d2, d3 ou d4. Caso não haja necessidade de usar um ou mais deli-
mitadores, atribuir a eles o valor -1.

Parâmetros: FILE *f - arquivo que será lido;
            char d1 - delimitador 1;
            char d2 - delimitador 2;
            char d3 - delimitador 3;
            char d4 - delimitador 4;

Retorno: string lida.
<--------------------------------------------------------------------------------*/

char *str_read (FILE *f, char d1, char d2, char d3, char d4){
    if (f != NULL){

        int i;            // Contador;
        char *str = NULL; // Nome do arquivo;
        char c = -1;      // Variável auxiliar;

        for (i = 0; (c = fgetc (f)) != d1 && c != d2 && c != d3 && c != d4; i++){

            // Alocando memória com um espaço extra para o '\0';
            str = (char *) realloc (str, (i + 2)*sizeof (char));
            str[i] = c;

        }

        if (str != NULL) str[i] = '\0';

        return str;

    }

    return NULL;
}
