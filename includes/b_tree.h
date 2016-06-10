#ifndef _B_TREE_H_
#define _B_TREE_H_

#define N_KEYS 7
/* Tamanho de uma página de disco; */
#define CLUSTER 64

typedef struct NODE NODE;
typedef struct B_TREE B_TREE;
typedef struct KEY KEY;

/**
 * Cria uma árvore B*, criando um arquivo própria para ela.
 *
 * Parâmetros: name - nome do arquivo de índice.
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int create_tree (char *name, B_TREE **b);

/**
 * TODO
 *
 * Parâmetros: b - ;
 * 			   k - .
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int insert_in_header (B_TREE *b, KEY k);

/**
 * TODO
 *
 * Parâmetros: 
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int print_index (B_TREE *b);

/**
 * TODO
 *
 * Parâmetros: 
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int read_root (B_TREE *b);

/**
 * TODO
 *
 * Parâmetros: 
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int insert_item (B_TREE *b, NODE *n);

/**
 * Busca por um item de ID "id" e o retorna por meio do parâmetro item.
 *
 * Parâmetros: b - árvore onde será feita a busca;
 * 			   item - endereço onde será colocado o nó encontrado;
 * 			   id - ID que será buscado na árvore B*.
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int search_item (B_TREE *b, int16_t * rrn, int key);

/**
 * Desaloca a memória utilizada pela árvore B*.
 *
 * Parâmetros: b - endereço da árvore a ser desalocada.
 */
void delete_tree (B_TREE **b);

#endif
