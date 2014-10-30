#include <stdio.h>
#include <stdlib.h>


/*AS_node * AS_node_new(int asn, int neighbor, int rel){
  AS_node *new = (AS_node *) malloc(sizeof(AS_node));

  AS_node->AS_num = asn;


  return new;
}
*/

void new_link(int asn, int neighborn, int rel, AS_node **hash_table){
	int index;
	AS_node *aux;
	AS_neighbor *neighbor;

	/* criar vizinho */
	neighbor = AS_neighbor_new(neighborn);


	/*verificar se a AS existe na hash_table */
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
	AS_node *hash_table[127];




	return 0;
}