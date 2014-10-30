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
  int AS_num;
  struct asnode *next;
  AS_neighbor *customers; 
  AS_neighbor *peers;
  AS_neighbor *providers;

} AS_node;

