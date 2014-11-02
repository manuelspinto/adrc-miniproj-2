#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct neighbor{
	int num;
	struct neighbor *next;

} AS_neighbor;

/*
 *	Estrutura que guarda o ASN e os vizinhos
 */
typedef struct asnode{
  int AS_num;
  struct asnode *next;
  AS_neighbor *customers; 
  AS_neighbor *peers;
  AS_neighbor *providers;

} AS_node;

void new_link(int asn, int neighborn, int rel, AS_node ** hash_table);

AS_neighbor * AS_neighbor_new(int num);

AS_node * AS_node_new(void);

AS_node *get_AS(int asn, AS_node **AS_table);
void print_AS_path(AS_neighbor *path, AS_node **AS_table);
void append_AS(AS_neighbor **AS_path, int asn);
void remove_last_AS(AS_neighbor **AS_path);
void cmp_AS_path(AS_neighbor **AS_path, AS_node **AS_table);
int len_AS_path(AS_neighbor *AS_path);
void free_AS_path(AS_neighbor *AS_path);
void cpy_AS_path(AS_neighbor **AS_path);
AS_neighbor *AS_find_path(int asn_s, int asn_d, AS_node **AS_table);
void find_alg(int from, int this, int dest, int relation, AS_node **AS_table, AS_neighbor **AS_path);
int rel_AS_neighbor(int source, int neighbor, AS_node **AS_table);
