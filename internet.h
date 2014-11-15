#include <stdio.h>
#include <stdlib.h>

typedef struct neighbor{
	int num;
	struct neighbor *next;

} AS_neighbor;

/*
 *	Estrutura que guarda o ASN e os vizinhos
 */
typedef struct asnode{
  AS_neighbor *customers; 
  AS_neighbor *peers;
  AS_neighbor *providers;

} AS_node;

typedef struct astreenode{
	int asn;
	struct astreenode *next;
	struct astreenode *left;
	struct astreenode *right;
} AS_tree_node;

void new_link(int asn, int neighborn, int rel, AS_node ** hash_table);

AS_neighbor * AS_neighbor_new(int num);

AS_node * AS_node_new(void);

void print_AS_table(AS_node ** AS_table);

AS_neighbor * remove_node(int selected_node, AS_neighbor * set_to_visit);
int elected_route(int origin, int dest, AS_node ** AS_table);
void list_to_tree(AS_tree_node * root, AS_tree_node * first, int n);
void AS_tree_insert(AS_tree_node **root, int asn);
void AS_tree_remove(AS_tree_node **root, int asn);
AS_tree_node *search_node(AS_tree_node *root, AS_tree_node **parent, int asn);
int set_to_visit_empty(AS_tree_node **set_to_visit);
int best_est_found(AS_tree_node **set_to_visit, int est);
int AS_tree_rand_remove(AS_tree_node **root);


