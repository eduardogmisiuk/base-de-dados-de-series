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
 * Retorno: TODO.
 */
int create_tree (char *name, B_TREE **b) {
	if (name != NULL) {
		*b = (B_TREE *) malloc (sizeof (B_TREE));
		if (b == NULL) {
			fprintf (stderr, ERROR_ALLOCATION);
			return ALLOCATION;
		}

		char *filename = (char *) malloc (strlen (name)*sizeof (char));
		FILE* header_file;

		// Para o usuário não modificar o nome do arquivo, copio este para
		// outro
		strcpy (filename, name);

		(*b)->filename = filename;
		(*b)->n_nodes = 0;
		(*b)->index = NULL;
		(*b)->n = NULL;

		header_file = fopen("header", "w+");
		fwrite (0, sizeof(int), 1, header_file);
		(*b)->header = header_file;

		fclose(header_file);
	}
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
}

/**
 * 
 *
 * Parâmetros: 
 */
void in_order_print (B_TREE *b, NODE *node, int n) {
	if (node != NULL) {
		int i, j;
		for (i = 0; i < N_KEYS && node->keys[i].rrn != -1; i++) {
			// Indo no filho à esquerda desta chave;
			catch_node (b, left_child (node->keys[i].rrn));
			in_order_print (b, b->n, n+1);

			// Processando a chave;
			// Printando primeiro os espaços, para dar o visual de uma árvore;
			for (j = 0; j < n; j++) printf ("\t");
			// Imprimindo a chave;
			printf ("%d\n", node->keys[i].key);
		}

		// Como provocaria uma repetição na impressão, somente a última
		// chave irá entrar recursivamente no seu filho direito;
		// Indo no filho à direita desta chave;
		catch_node (b, right_child (node->keys[i-1].rrn));
		in_order_print (b, b->n, n+1);
	}
}

/**
 * 
 *
 * Parâmetros: 
 *
 * Retorno:
 */
int print_index (B_TREE *b) {
	if (b == NULL) return INVALID_B_TREE;

	int root = read_root (b);

	catch_node (b, root);

	in_order_print (b, b->n, 0);
}

/**
 * 
 *
 * Parâmetros: 
 *
 * Retorno: 
 */
int read_root (B_TREE *b) {
	int rrn = 0;

	rewind(b->header);
	fread (&rrn, sizeof(int), 1, b->header);
	
	return rrn;
}

// Resolvi deixar isso aqui para possíveis testes futuros;
/*int main (int argc, char *argv[]) {
	printf ("int - %d\n", sizeof (int));
	printf ("short int - %d\n", sizeof (short int));
	printf ("NODE - %d\n", sizeof (NODE));
	printf ("KEY - %d\n", sizeof (KEY));

	return 0;
}*/
