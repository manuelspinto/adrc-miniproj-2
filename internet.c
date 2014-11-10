#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "internet.h"

#define NEW 1
#define FINAL 0


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


#if 0
char * append_AS(char * AS_path, int asn){
  
  /*aloca uma nova string com espaço para a string AS_path + ' -> ' + asn */
  char * new_path = (char*) malloc ((strlen(AS_path) + 10) * sizeof(char));
  
  /*cria a nova string com o novo AS_path */
  sprintf(new_path, "%s -> %d", AS_path, asn);
  
  /*liberta a string que guarda o AS_path antigo*/
  free(AS_path);
  
  return new_path;
  
}

#endif


/* Insere um AS no AS_path. A inserção é feita no início de uma lista */ 
void append_AS(AS_neighbor **AS_path, int asn){
	AS_neighbor *new_root;
	new_root = AS_neighbor_new(asn);

	if(AS_path[NEW] != NULL)
	  new_root->next = AS_path[NEW];
	AS_path[NEW] = new_root;
}


/* Remove o AS do AS_PATH 
 A estratégia de inserção e remoção na lista é LIFO por isso basta retirar o primeiro elemento */

void remove_last_AS(AS_neighbor **AS_path){
	AS_neighbor *new_root;
	new_root = AS_path[NEW]->next;

	free(AS_path[NEW]);
	AS_path[NEW] = new_root;
}

int len_AS_path(AS_neighbor *AS_path){
	int i = 0;
	AS_neighbor *p;

	for(p = AS_path; p != NULL ; p = p->next)
		i++;

	return i;
}

void free_AS_path(AS_neighbor *AS_path){
	AS_neighbor *p;
	AS_neighbor *pnext;

	if(AS_path == NULL)
		return;
	for(p = AS_path, pnext = AS_path->next; pnext != NULL; pnext = pnext->next){
		free(p);
		p = pnext;
	}
	free(p);

	return;
}

void cpy_AS_path(AS_neighbor **AS_path){
	AS_neighbor *new_root = NULL;
	AS_neighbor *p;
	AS_neighbor *p_new;

	for(p = AS_path[NEW]; p != NULL; p = p->next){
		p_new = AS_neighbor_new(p->num);
		p_new->next = new_root;
		new_root = p_new;
	}
	AS_path[FINAL] = new_root;
}

/*
 * Imprime uma lista de ASes com as seguintes informaçoes: AS_num, lista de customers, lista de peers, lista de providers
 */

void print_AS_table(AS_node ** AS_table){
  
  AS_node * aux;
  AS_neighbor * n;
  int i;
  
  for(i = 0; i < 65536; i++){	/*percorre a AS_table*/
    if(AS_table[i] != NULL){
    	aux = AS_table[i];
			printf("ASN = %d: \n\tcustomers: ", i);	/*imprime AS_num da AS*/
			for(n = aux->customers; n != NULL; n = n->next) printf("%d ", n->num);	/*imprime a lista de customers da AS*/
			printf("\n\tpeers: ");
			for(n = aux->peers; n != NULL; n = n->next) printf("%d ", n->num);	/*imprime a lista de peers da AS*/
			printf("\n\tproviders: ");
			for(n = aux->providers; n != NULL; n = n->next) printf("%d ", n->num);	/*imprime a lista de providers da AS*/
			printf("\n");
		}
  }
  
  return;
}

/*
 * Insere na AS_table a informação de uma linha do ficheiro de entrada
 */

void new_link(int asn, int neighborn, int rel, AS_node ** AS_table){
	AS_node *as;
	AS_neighbor *neighbor;

	/* cria o vizinho (head do link) */
	neighbor = AS_neighbor_new(neighborn);

	/* percorre a lista de ASes na posiçao index da AS_table para descobrir se a AS (tail do link) existe na AS_table*/
	/*for(as = AS_table[index]; (as != NULL) && (as->AS_num != asn) ; as = as->next);*/
	  
	/*se a AS não existe ainda na AS_table, então vamos criá-la e inseri-la*/
	if(AS_table[asn] == NULL)
	  AS_table[asn] = AS_node_new();
	
	as = AS_table[asn];
	
	/* dependendo da relaçao rel, insere um customer/peer/provider na lista correspondente da AS*/
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

  new->peers = NULL;
  new->customers = NULL;
  new->providers = NULL;

  return new;
}

void print_AS_path(AS_neighbor *path, AS_node **AS_table){
	AS_neighbor *p;
	int rel;

	rel = path->num; /* primeiro elemento da lista indica o tipo de rota */

	printf("AS_path: ");
	if(rel == 0)
		printf("NO ROUTE\n");
	else{
		if(rel == 3)
			printf("(Provider Route) ");
		else if(rel == 2)
			printf("(P2P Route) ");
		else
			printf("(Customer Route) ");

		for(p = path->next; p != NULL; p = p->next)
			printf("%d -> ",p->num);
		printf("\n");
	}
}

void cmp_AS_path2(AS_neighbor **AS_path, AS_node **AS_table){

	if(AS_path[FINAL] == NULL) cpy_AS_path(AS_path);
	
	else if(len_AS_path(AS_path[NEW]) < len_AS_path(AS_path[FINAL])){
	  free_AS_path(AS_path[FINAL]);
	  cpy_AS_path(AS_path);
	}
	return;
}


int find_alg(int from, int this, int dest, int rel, AS_node **AS_table, AS_neighbor **AS_path){
	AS_node *AS;
	AS_neighbor *n;

	AS = AS_table[this];

	/* For each customer */
	for(n = AS->customers; n != NULL; n = n->next){
		if(n->num != from){
			append_AS(AS_path, n->num);
			if(n->num == dest){
				cmp_AS_path2(AS_path, AS_table);
				remove_last_AS(AS_path);
			}
			else
				find_alg(this, n->num, dest, 1, AS_table, AS_path);
		}
	}

	if(AS_path[FINAL] != NULL){
	  remove_last_AS(AS_path);
	  return 1;
	}
	
	if(rel == 3){
		
		/* For each peer */
		for(n = AS->peers; n != NULL; n = n->next){
			if(n->num != from){
				append_AS(AS_path, n->num);
				if(n->num == dest){
					cmp_AS_path2(AS_path, AS_table);
					remove_last_AS(AS_path);
				}
				else
					find_alg(this, n->num, dest, 2, AS_table, AS_path);
			}
		}

		if(AS_path[FINAL] != NULL){
		  remove_last_AS(AS_path);
		  return 2;
		}
	  
		/* For each provider */
		for(n = AS->providers; n != NULL; n = n->next){
			if(n->num != from){
				append_AS(AS_path, n->num);
				if(n->num == dest){
					cmp_AS_path2(AS_path, AS_table);
					remove_last_AS(AS_path);
				}
				else
					find_alg(this, n->num, dest, 3, AS_table, AS_path);
			}
		}
		
		if(AS_path[FINAL] != NULL){
		  remove_last_AS(AS_path);
		  return 3;
		}
	}
	
	remove_last_AS(AS_path);
	return 0;
}

AS_neighbor *AS_find_path(int asn_s, int asn_d, AS_node **AS_table){
	AS_neighbor **AS_path = (AS_neighbor **) malloc (2 * sizeof(AS_neighbor *));
	AS_neighbor *new_root;
	
	AS_path[FINAL] = NULL;
	AS_path[NEW] = NULL;

	append_AS(AS_path, asn_s);
	
	new_root = AS_neighbor_new(find_alg(-1, asn_s, asn_d, 3, AS_table, AS_path));
	new_root->next = AS_path[FINAL];
	AS_path[FINAL] = new_root;
	
	free_AS_path(AS_path[NEW]);

	return AS_path[FINAL];
}

int main(int argc, char *argv[]){
	int i, s, d;
	int asn1, asn2, rel;
	AS_neighbor *AS_path;
	FILE *fp;
	char buff[128];

	if(argc != 2){
		printf("./internet network_file.txt\n");
		exit(1);
	}

	AS_node ** AS_table = (AS_node **) malloc (65536 * sizeof(AS_node *));
	for(i = 0; i<65536;i++) /* O MAC não mete por defeito os ponteiros alocados a NULL e sem isto dá-me seg_fault na verificação de NULL da print_AS_table, depois pode-se apagar */
		AS_table[i] = NULL;

	fp = fopen(argv[1],"r");
  if(fp == NULL){
    printf("ERROR: Impossivel abrir o ficheiro '%s'\n",argv[1]);
    exit(1);
  }
  
  while (fgets(buff, sizeof(buff), fp) != NULL ){
    sscanf(buff,"%d\t%d\t%d",&asn1,&asn2,&rel);
    new_link(asn1,asn2,rel,AS_table);
  }

	/*new_link(4323, 12122, 1, AS_table);
	new_link(12122, 4323, 3, AS_table);
	new_link(4323, 12249, 1, AS_table);
	new_link(12249, 4323, 3, AS_table);
	new_link(29017, 34309, 2, AS_table);
	new_link(34309, 29017, 2, AS_table);*/

	/*new_link(1, 2, 1, AS_table);
	new_link(2, 1, 3, AS_table);
	new_link(7, 1, 1, AS_table);
	new_link(1, 7, 3, AS_table);
	new_link(1, 3, 1, AS_table);
	new_link(3, 1, 3, AS_table);
	new_link(5, 3, 1, AS_table);
	new_link(3, 5, 3, AS_table);
	new_link(6, 5, 1, AS_table);
	new_link(5, 6, 3, AS_table);
	new_link(4, 2, 1, AS_table);
	new_link(2, 4, 3, AS_table);
	new_link(4, 1, 2, AS_table);
	new_link(1, 4, 2, AS_table);
	new_link(7, 6, 2, AS_table);
	new_link(6, 7, 2, AS_table);
	print_AS_table(AS_table);*/


	while(1){
		printf("[Source] [Dest]: ");
		scanf("%d %d",&s,&d);
		AS_path = AS_find_path(s , d, AS_table);
		print_AS_path(AS_path, AS_table);
	} 
	
	return 0;
}
