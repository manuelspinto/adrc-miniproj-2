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

char * append_AS(char * AS_path, int asn);