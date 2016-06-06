/*
 * Mensagens e números de erros.
 *
 * Autores:
 * Allan Silva Domingues 	 9293290
 * Eduardo Garcia Misiuk 	 9293230
 * Raul Wagner Martins Costa 9293032
 */

#ifndef _ERR_MSG_H_
#define _ERR_MSG_H_

// Mensagens de erro:
#define ERROR_OPENING_FILE "Erro: falha ao abrir o arquivo.\n"
#define ERROR_ALLOCATION "Erro: não há memória suficiente para realizar esta operação!\n"
#define ERROR_INVALID_NAME "Erro: nome inválido!\n"
#define ERROR_ID_NOT_FOUND "Erro: ID não encontrado!\n"
#define ERROR_INVALID_B_TREE "Erro: índice B* nulo!\n"
#define ERROR_NODE_NOT_FOUND "Erro: falha ao procurar o RRN especificado no Buffer Pool!\n"
#define ERROR_INVALID_ARGUMENT "Erro: argumento inválido.\n"

#define SUCCESS 0
// Números dos erros:
#define OPENING_FILE 1
#define ALLOCATION 2
#define INVALID_NAME 3
#define ID_NOT_FOUND 4
#define INVALID_B_TREE 5
#define NODE_NOT_FOUND 6
#define INVALID_ARGUMENT 7

#endif
