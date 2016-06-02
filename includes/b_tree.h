#ifndef _B_TREE_H_
#define _B_TREE_H_

#define N_KEYS 6

typedef struct NODE NODE;
typedef struct B_TREE B_TREE;
typedef struct KEY KEY;

int create_tree (char *name, B_TREE **b);

int compare_key (KEY *a, KEY *b);

int catch_node (B_TREE *b, int rrn);

int print_index (B_TREE *b);

int read_root (B_TREE *b);

#endif
