#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "b_tree.h"
#include "err_msg.h"

inline int left_child (int a) { return a; }
inline int right_child (int a) { return a+1; }

// Chave de um nó da árvore B*
struct KEY {
	// Chave do registro;
	int key;
	// RRN onde está o registro com tal chave no arquivo de base de dados;
	short rrn;
};

// Cada nó ocupa uma página de disco de 64 bytes;
//  1   2   3   4   5   6   7
// |*| |*| |*| |*| |*| |*| |*|
//    1   2   3   4   5   6
struct NODE {
	// RRN do nó no arquivo de índice;
	short rrn;
	// RRNs dos filhos deste nó;
	short sons[N_KEYS + 1];
	// Chaves 
	KEY keys[N_KEYS];
};

// Estrutura da árvore B*
struct B_TREE {
	NODE *n;
	char *filename;
	FILE *header;
	FILE *index;
	// Número de nós que há no arquivo de índice;
	int n_nodes;
};

//TODO: erros.
/**
 * Cria uma árvore B*, criando um arquivo própria para ela.
 *
 * Parâmetros: name - nome do arquivo de índice.
 *
 * Retorno: árvore B* alocada.
 */
B_TREE *create_tree (char *name) {
	if (name != NULL) {
		char *filename = (char *) malloc (strlen (name)*sizeof (char));
		FILE* header_file;
		header_file = fopen("header", "w+");
		fwrite (0, sizeof(int), 1, header_file);

		strcpy (filename, name);

		B_TREE *b = (B_TREE *) malloc (sizeof (B_TREE));

		b->filename = filename;
		b->n_nodes = 0;
		b->index = NULL;
		b->header = header_file;
		b->n = NULL;
		fclose(header_file);
		return b;
	}

	return NULL;
}

/**
 * Compara duas structs KEY.
 *
 * Parâmetros: a - primeira KEY;
 * 			   b - segunda KEY.
 *
 * Retorno: -2 - a = NULL ou b = NULL;
 * 			-1 - a < b;
 * 			 0 - a = b;
 * 			+1 - a > b.
 */
int compare_key (KEY *a, KEY *b) {
	if (a != NULL && b != NULL) {
		if (a->key < b->key) return -1;
		if (a->key == b->key) return 0;
		if (a->key > b->key) return 1;
	}

	return 0;
}

/**
 * Recupera um nó com dado RRN do arquivo de índice.
 *
 * Parâmetros: b - árvore B* onde será guardado o nó requerido.
 *
 * Retorno: TODO
 */
int catch_node (B_TREE *b, int rrn) {
	if (b != NULL && rrn >= 0) {
		// Abrindo o arquivo de índice;
		b->index = fopen (b->filename, "+w");

		// Acha a posição;
		fseek (b->index, SEEK_SET, rrn*sizeof (NODE));
		// Lê o registro inteiro, guardando seu valor na variável n em b;
		fread (b->n, sizeof (NODE), 1, b->index);

		// Fechando o arquivo;
		fclose (b->index);
	}

	return INVALID_B_TREE;
}

/**
 *
 *
 *
 */
void search_item (B_TREE *b, int key) {
	if (b == NULL || key < 0) return;

	// Etapas a serem feitas:
	// - Pegar o RRN da raiz
	// - Fazer busca sequencial no vetor de chaves
	// - Ir descendo na árvore conforme há necessidade
	if (b->index == NULL) return; //a arvore nao possui registros

	fread()


}

int print_index (B_TREE *b) {
	if (b == NULL) return INVALID_B_TREE;
}

int read_root (B_TREE *b) {

	rewind(b->header_file);
	int rrn = 0;
	fread (&rrn, sizeof(int), 1, b->header_file);
	
	return rrn;
}