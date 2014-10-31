#include <stdio.h>
#include <stdlib.h>
#include "internet.h"


#if 0
AS_node * AS_node_new(int asn, int neighbor, int rel){
  AS_node *new = (AS_node *) malloc(sizeof(AS_node));

  AS_node->AS_num = asn;


  return new;
}
#endif

#if 0
void new_link(int asn, int neighborn, int rel, AS_node **hash_table){
	int index;
	AS_node *aux;
	AS_neighbor *neighbor;

	/* criar vizinho */
	neighbor = AS_neighbor_new(neighborn);


	/* verificar se a AS existe na hash_table*/
	index = asn % 127;

	for(aux = hash_table[index]; aux != NULL ; aux = aux->next){
		if(aux->AS_num == asn){
			if(rel == 1){
				neighbor->next = aux->customers;
				aux->customers = neighbor;
			}
			else if(rel == 2){
				neighbor->next = aux->peers;
				aux->peers = neighbor;
			}
			else{
				neighbor->next = aux->providers;
				aux->providers = neighbor;
			}
		}

	}


}
#endif

void print_AS_table(AS_node ** AS_table){
  
  AS_node * aux;
  AS_neighbor * n;
  int i;
  
  for(i = 0; i < 127; i++){
    for(aux = AS_table[i]; aux != NULL; aux = aux->next){
	printf("ASN = %d: \n\tcustomers: ", aux->AS_num);
	for(n = aux->customers; n != NULL; n = n->next) printf("%d ", n->num);
	printf("\n\tpeers: ");
	for(n = aux->peers; n != NULL; n = n->next) printf("%d ", n->num);
	printf("\n\tproviders: ");
	for(n = aux->providers; n != NULL; n = n->next) printf("%d ", n->num);
	printf("\n");
    }
      
  }
  
  return;
}


void new_link(int asn, int neighborn, int rel, AS_node ** AS_table){
	int index;
	AS_node *as;
	AS_neighbor *neighbor;

	/* criar vizinho */
	neighbor = AS_neighbor_new(neighborn);

	/* descobrir se a AS existe na AS_table*/
	index = asn % 127;

	for(as = AS_table[index]; (as != NULL) && (as->AS_num != asn) ; as = as->next);
	  
	/*se a AS não existe ainda na AS_table, então vamos criá-la e inseri-la*/
	
	if(as == NULL){
	  AS_table[index] = AS_node_new();
	  AS_table[index]->AS_num = asn;
	  as = AS_table[index];
	}
	
	/* inserir a informação sobre o novo link na AS_table*/
	
	switch (rel) {
	case 1:
	  neighbor->next = as->customers;
	  as->customers = neighbor;
	  break;
	  
	case 2:
	  neighbor->next = as->peers;
	  as->peers = neighbor;
	  break;
	  
	case 3:
	  neighbor->next = as->providers;
	  as->providers = neighbor;
	  break;
	  
	default:
	  /* ERRO*/
	  break;
	}
	
	
	return;

}


AS_neighbor * AS_neighbor_new(int num){

	AS_neighbor *new = (AS_neighbor *) malloc(sizeof(AS_neighbor));
	new->num = num;
	new->next = NULL;

	return new;
}
	

AS_node * AS_node_new(void){
  AS_node *new = (AS_node *) malloc(sizeof(AS_node));

  new->AS_num = 0;
  new->next = NULL;
  new->peers = NULL;
  new->customers = NULL;
  new->providers = NULL;

  return new;
}


int main(int argc, char *argv[]){
	
	
	AS_node ** AS_table = (AS_node **) malloc (127 * sizeof(AS_neighbor *));
	
	new_link(4323, 12122, 1, AS_table);
	new_link(12122, 4323, 3, AS_table);
	new_link(4323, 12124, 1, AS_table);
	new_link(12124, 4323, 3, AS_table);
	new_link(29017, 34309, 2, AS_table);
	new_link(34309, 29017, 2, AS_table);

	print_AS_table(AS_table);
	
	return 0;
}