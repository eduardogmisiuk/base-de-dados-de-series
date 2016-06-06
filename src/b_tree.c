/*
 * Implementação da árvore B* com buffer pool de leitura do índice.
 *
 * Autores:
 * Allan Silva Domingues 	 9293290
 * Eduardo Garcia Misiuk 	 9293230
 * Raul Wagner Martins Costa 9293032
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include "b_tree.h"
#include "err_msg.h"
#include "utils.h"

#define LEFT_CHILD(a) a
#define RIGHT_CHILD(a) a+1

typedef struct HEADER HEADER;
typedef struct BUFFER_POOL BUFFER_POOL;

/* Chave de um nó da árvore B* */
struct KEY {
	/* Chave do registro; */
	int32_t id;
	/* RRN onde está o registro com tal chave no arquivo de base de dados; */
	int16_t rrn;
};

/* Cada nó ocupa uma página de disco de 64 bytes; */
/*  1   2   3   4   5   6   7 */
/* |*| |*| |*| |*| |*| |*| |*|*/
/*    1   2   3   4   5   6   */
struct NODE {
	/* RRN do nó no arquivo de índice; */
	int16_t rrn;
	/* RRNs dos filhos deste nó; */
	int16_t sons[N_KEYS + 1];
	/* Chaves; */
	KEY keys[N_KEYS];
};

/* Buffer no qual qualquer página é colocada ao ser lida do arquivo ou
 * para ser escrita no mesmo.
 */
struct BUFFER_POOL {
	int used;
	int recently[BUFFER_POOL_SIZE];
	NODE page[BUFFER_POOL_SIZE];
};

/* Estrutura que mantém as informações do cabeçalho enquanto ele é modificado
 * em memória. Ao criar uma árvore, essa estrutura é inicializada com os dados
 * de um cabeçalho salvo anteriormente ou com os dados iniciais, caso não haja
 * um arquivo ainda. Ao deletar uma árvore as novas informações do cabeçalho
 * são gravadas.
 */
struct HEADER {
	char *filename;
	int16_t root;
	int height;
	int16_t last_rrn;
	int order;
};

/* Estrutura da árvore B* */
struct B_TREE {
	NODE *n;
	BUFFER_POOL *buffer;
	HEADER *header;
	char *filename;
	/* Número de nós que há no arquivo de índice; */
	int n_nodes;
};

/* Protótipos; */
void delete_header (B_TREE *b);

/**
 * Imprime a mensagem de código do erro passado.
 *
 * Parâmetros: err_code - código de erro.
 */
void print_error_by_code (int err_code) {
	switch (err_code) {
		case OPENING_FILE:
			fprintf (stderr, ERROR_OPENING_FILE);
			break;

		case ALLOCATION:
			fprintf (stderr, ERROR_ALLOCATION);
			break;

		case INVALID_NAME:
			fprintf (stderr, ERROR_INVALID_NAME);
			break;

		case ID_NOT_FOUND:
			fprintf (stderr, ERROR_ID_NOT_FOUND);
			break;

		case INVALID_B_TREE:
			fprintf (stderr, ERROR_INVALID_B_TREE);
			break;

		case NODE_NOT_FOUND:
			fprintf (stderr, ERROR_NODE_NOT_FOUND);
			break;

		case INVALID_ARGUMENT:
			fprintf (stderr, ERROR_INVALID_ARGUMENT);
			break;

		default:
			break;
	}
}

/**
 * TODO
 *
 * Parâmetros: 
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int create_header (B_TREE *b, char *name) {
	int filename_len;
	FILE *new_header;

	b->header = (HEADER *) malloc (sizeof (HEADER));
	if (b->header == NULL)
		return ALLOCATION;

	filename_len = strlen (name) + strlen (H_EXT) + 1;
	b->header->filename = (char *) malloc (filename_len * sizeof (char));
	if (b->header == NULL) {
		delete_header (b);

		return ALLOCATION;
	}

	strcpy (b->header->filename, name);
	strcat (b->header->filename, H_EXT);

	return SUCCESS;
}

/**
 * TODO
 *
 * Parâmetros: 
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int create_buffer_pool (B_TREE *b) {
	b->buffer = (BUFFER_POOL *) malloc (sizeof (BUFFER_POOL));
	if (b->buffer == NULL)
		return ALLOCATION;

	return SUCCESS;
}

/**
 * Verifica se há um arquivo de header já criado.
 *
 * Parâmetros: b - árvore com o header que será verificado.
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int isset_header (B_TREE *b) {
	/* Tentando abrir o arquivo de header para saber se ele existe; */
	FILE *header = fopen (b->header->filename, "r");
	/* Caso retorne NULL, não há um arquivo de header criado; */
	if (header == NULL) {
		return FALSE;
	}
	/* Caso contrário, há uma rquivo de header criado; */
	else {
		fclose (header);
		return TRUE;
	}
}

/**
 * Carrega os dados do arquivo de header na memória.
 *
 * Parâmetros: b - árvore com o header que será lido.
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int load_header (B_TREE *b) {
	FILE *header = fopen (b->header->filename, "r");
	if (header == NULL) {
		return OPENING_FILE;
	}

	fread (&(b->header->root), sizeof (int16_t), 1, header); 
	fread (&(b->header->height), sizeof (int), 1, header); 
	fread (&(b->header->last_rrn), sizeof (int16_t), 1, header); 
	fread (&(b->header->order), sizeof (int), 1, header);

	return SUCCESS;
}

/**
 * TODO
 *
 * Parâmetros: 
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int update_header (B_TREE *b) {
	FILE* header;

	header = fopen (b->header->filename, "w+");
	if (header == NULL) {
		return OPENING_FILE;
	}

	fwrite (&(b->header->root), sizeof (int16_t), 1, header); 
	fwrite (&(b->header->height), sizeof (int), 1, header); 
	fwrite (&(b->header->last_rrn), sizeof (int16_t), 1, header); 
	fwrite (&(b->header->order), sizeof (int), 1, header);

	fclose (header);

	return SUCCESS;
}

/**
 * TODO
 *
 * Parâmetros: 
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int start_tree (B_TREE *b) {
	b->n = NULL;
	b->n_nodes = 0;
	b->buffer->used = 0;
	if (isset_header (b)) {
		b->header->root = -1;
		b->header->height = 0;
		b->header->last_rrn = -1;
		b->header->order = N_KEYS + 1;

		return update_header (b);
	}
	else {
		return load_header (b);
	}
}

int create_tree (char *name, B_TREE **b) {
	int status;
	FILE *new_index;
	int filename_len;

	if (name != NULL) {
		*b = (B_TREE *) malloc (sizeof (B_TREE));
		if (b == NULL) {
			print_error_by_code (ALLOCATION);
			return ALLOCATION;
		}

		filename_len = strlen (name) + strlen (I_EXT) + 1;
		(*b)->filename = (char *) malloc (filename_len * sizeof (char));
		if ((*b)->filename == NULL) {
			delete_tree (b);
			print_error_by_code (ALLOCATION);
			return ALLOCATION;
		}

		/* Para o usuário não modificar o nome do arquivo, copio este para outro; */
		strcpy ((*b)->filename, name);
		strcat ((*b)->filename, I_EXT);
		new_index = fopen ((*b)->filename, "w+");
		if (new_index == NULL) {
			delete_tree (b);
			print_error_by_code (OPENING_FILE);
			return OPENING_FILE;
		}
		fclose (new_index);

		status = create_header (*b, name);
		if (status != SUCCESS) {
			delete_tree (b);
			print_error_by_code (status);
			return status;
		}

		status = create_buffer_pool (*b);
		if (status != SUCCESS) {
			delete_tree (b);
			print_error_by_code (status);
			return status;
		}

		status = start_tree (*b);
		if (status != SUCCESS) {
			delete_tree (b);
			print_error_by_code (status);
			return status;
		}

		return SUCCESS;
	}

	print_error_by_code (INVALID_NAME);
	return INVALID_NAME;
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
		if (a->id < b->id)
			return -1;
		if (a->id == b->id)
			return 0;
		else
			return 1;
	}

	return 0;
}

/**
 * TODO
 *
 * Parâmetros: 
 *
 * Retorno: 
 */
int at_buffer (B_TREE *b, int rrn) {
	int i;

	for (i = 0; i < b->buffer->used; i++)
		if (b->buffer->page[i].rrn == rrn)
			return TRUE;

	return FALSE;
}

/**
 * TODO 
 *
 * Parâmetros: 
 *
 * Retorno: 
 */
void write_buffered_page (B_TREE *b, int position) {
	FILE *index;

	index = fopen (b->filename, "r+");
	if (index != NULL) {
		/* TODO: serialize */
		fseek (index, SEEK_SET, b->buffer->page[position].rrn * sizeof (NODE));
		fwrite (&(b->buffer->page[position]), sizeof (NODE), 1, index);
		fclose (index);
	}
	else {
		print_error_by_code (OPENING_FILE);
	}
}

/**
 * TODO
 *
 * Parâmetros: 
 *
 * Retorno: 
 */
void update_buffer (B_TREE *b, int rrn) {
	int aux, i, j;
	int last_position = BUFFER_POOL_SIZE - 1;
	int* recent_pos = b->buffer->recently;
	NODE* buffered = b->buffer->page;
	FILE *index;

	/* Vai à posição em que está o registro procurado ou à próxima
	 * posição livre. O tamanho do array de posições é atingido pelo
	 * contador se não houver posições livres e o registro não estiver
	 * armazenado no buffer.
	 */
	for (i = 0; i < b->buffer->used && buffered[recent_pos[i]].rrn != rrn; i++);

	/* Se o registro não estiver no buffer, */
	if (i == b->buffer->used) {
		/* se não houver posição livre, */
		if (b->buffer->used == last_position + 1) {
			/* a última será substituída e precisa ser
			 * gravada no arquivo novamente*/
			i--;
			write_buffered_page (b, recent_pos[i]);
		}
		/* se houver, */
		else {
			/* é necessário indicar qual posição é essa e aumentar
			 * a quantidade de posições utilizadas */
			recent_pos[i] = b->buffer->used++;
		}

		/* Em ambos os casos, há uma leitura do arquivo. */
		index = fopen (b->filename, "r+");
		if (index != NULL) {
			fseek (index, SEEK_SET, rrn * sizeof (NODE));
			fread (&(buffered[recent_pos[i]]), sizeof (NODE), 1, index);
			fclose (index);
		}
		else {
			print_error_by_code (OPENING_FILE);
		}
	}

	/* E então, há a rotação dos mais recentemente usados. */
	aux = recent_pos[i];
	for (j = 0; j < i; j++)
		recent_pos[i - j] = recent_pos[i - j - 1];
	recent_pos[0] = aux;
}

/**
 * TODO
 *
 * Parâmetros: 
 *
 * Retorno: 
 */
int get_buffer_index (B_TREE *b, int rrn) {
	int i;

	for (i = 0; i < b->buffer->used; i++)
		if (b->buffer->page[b->buffer->recently[i]].rrn == rrn)
			return i;

	return -1;
}

/**
 * Recupera um nó com dado RRN do arquivo de índice.
 *
 * Parâmetros: b - árvore B* onde será guardado o nó requerido.
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int catch_node (B_TREE *b, int rrn) {
	int position;

	if (b != NULL && rrn >= 0) {
		update_buffer (b, rrn);

		position = get_buffer_index (b, rrn);
		if (position != -1) {
			b->n = &(b->buffer->page[position]);
			return SUCCESS;
		}

		return NODE_NOT_FOUND;
	}

	return INVALID_B_TREE;
}

/**
 * Cria uma página de disco com nada dentro.
 *
 * Parâmetros: b - árvore B*;
 * 			   page - endereço da variável que guardará a página criada.
 *
 * Retorno: o retorno é somente para erros. Os erros possíveis para todas as funções
 * se encontram descritos no arquivo err_msg.h na pasta includes.
 */
int create_page (B_TREE *b, NODE **page) {
	if (page == NULL) return INVALID_ARGUMENT;

	else {
		int i;

		*page = (NODE *) malloc (sizeof (NODE));
		if (*page== NULL) return ALLOCATION;

		(*page)->rrn = ++(b->header->last_rrn);
		memset ((*page)->sons, -1, sizeof (int16_t) * (N_KEYS + 1));
		memset ((*page)->keys, -1, sizeof (KEY) * N_KEYS);

		return SUCCESS;
	}
}

int insert_in_header (B_TREE *b, KEY k) {
	/*if (b == NULL)
		return INSERTION;*/

	if (b->header->root == -1) {
	}
}

int search_item (B_TREE *b, NODE **n, int key) {
	if (b == NULL || key < 0) return 1;

	/* Etapas a serem feitas: */
	/* - Pegar o RRN da raiz */
	/* - Fazer busca sequencial no vetor de chaves */
	/* - Ir descendo na árvore conforme há necessidade */
	if (b->filename == NULL) return 0; /* a arvore nao possui registros; */
}

/**
 * Função recursiva de impressão da árvore B*.
 * Nesta impressão, é usada a ideia de impressão em ordem das árvores
 * binárias. A raiz ficará à esquerda, com a árvore descendo à direita.
 *
 * Parâmetros: b - árvore B* a ser impressa;
 * 			   node - nó que será impresso;
 * 			   n - nível do nó passado.
 */
void in_order_print (B_TREE *b, NODE *node, int n) {
	if (node != NULL) {
		int i, j;
		for (i = 0; i < N_KEYS && node->keys[i].rrn != -1; i++) {
			/* Indo no filho à esquerda desta chave; */
			catch_node (b, node->sons[LEFT_CHILD(node->keys[i].rrn)]);
			in_order_print (b, b->n, n+1);
			free (b->n);

			/* Processando a chave; */
			/* Printando primeiro os espaços, para dar o visual de uma árvore; */
			for (j = 0; j < n; j++) printf ("\t");
			/* Imprimindo a chave; */
			/* Por questão de compatibilidade, usaremos as macros definidas na biblioteca */
			/* inttypes.h. A macro abaixo é definida como "d", porém em outros sistemas pode */
			/* mudar, por isso há necessidade de se utilizá-la; */
			printf ("%" PRId32 "\n", node->keys[i].id);
		}

		/* Indo no filho à direita desta chave; */
		/* Como provocaria uma repetição na impressão, somente a última */
		/* chave irá entrar recursivamente no seu filho direito; */
		catch_node (b, node->sons[RIGHT_CHILD(node->keys[i-1].rrn)]);
		in_order_print (b, b->n, n+1);

		free (b->n);
	}
}

int print_index (B_TREE *b) {
	if (b == NULL) return INVALID_B_TREE;

	int root = read_root (b);
	NODE *n = NULL;

	catch_node (b, root);
	n = b->n;

	in_order_print (b, n, 0);

	free (b->n);

	return SUCCESS;
}

/**
 * TODO
 *
 * Parâmetros: 
 */
void delete_header (B_TREE *b) {
	if (b != NULL && b->header != NULL) {
		if (b->header->filename != NULL)
			free (b->header->filename);

		free (b->header);
		b->header = NULL;
	}
}

void delete_tree (B_TREE **b) {
	if (b == NULL || *b == NULL) return;

	/* Caso não seja nulo nem o endereço da variável nem a árvore em si; */
	delete_header (*b);

	if ((*b)->buffer != NULL) free ((*b)->buffer);
	if ((*b)->filename != NULL) free ((*b)->filename);

	free (*b);
	*b = NULL;
}

int insert_item (B_TREE *b, NODE *n) {
	if (b == NULL || n == NULL) return INVALID_ARGUMENT;

	return SUCCESS;
}
