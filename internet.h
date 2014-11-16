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

typedef struct astopnode{
	int asn;
	AS_neighbor *start;
	struct astopnode *next;
} AS_topnode;

void new_link(int asn, int neighborn, int rel, AS_node ** hash_table);

AS_neighbor * AS_neighbor_new(int num);

AS_node * AS_node_new(void);

void print_AS_table(AS_node ** AS_table);

int best_est_possible(int * counts);
AS_neighbor * remove_node(AS_neighbor * remove, AS_neighbor * parent, AS_neighbor * set_to_visit);
int elected_route(int origin, int dest, AS_node ** AS_table);
AS_neighbor * Sort_List(AS_neighbor *pList, int asn, AS_neighbor **start);
int IsConnected(AS_node **AS_table);
AS_topnode * AS_topnode_new(int asn, AS_neighbor *start, AS_topnode *next);


