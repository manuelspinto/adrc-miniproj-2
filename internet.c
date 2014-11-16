#include <stdio.h>
#include <stdlib.h>
#include "internet.h"

#define AS_card 65536
#define zero 0
#define C 1
#define R 2
#define P 3
#define inf 4

/*Função que remove do conjunto set_to_visit o nó selected_node*/
AS_neighbor * remove_node(AS_neighbor * remove, AS_neighbor * parent, AS_neighbor * set_to_visit){
	
	/*Se o nó a remover for o primeiro elemento:*/
	if(remove == set_to_visit) set_to_visit = remove->next;
	
	/*Caso contrário:*/
	else parent->next = remove->next;
	
	free(remove);

	return set_to_visit;
}

/*Função que determina a melhor estimativa possível para chegar a dest dadas as contagens do vector counts*/
int best_est_possible(int * counts){

	if(counts[C] != 0) return C;
	if(counts[R] != 0) return R;
	if(counts[P] != 0) return P;
	return inf;
}


/*Função que determina o tipo de rota entre o nó origin e o nó dest*/
void all_elected_route(int dest, AS_node ** AS_table, float *stat){
	

	int est[AS_card]; /*estimativa do tipo de caminho com que a AS[i] chega a AS dest*/
	int i, selected_node, best_est;
	int * counts; /*vector em que se guarda quantas AS têm estimativa C, R, P ou inf para chegar ao destino*/
	AS_neighbor * set_to_visit, * aux, *parent;

	/*Criar o vector counts*/
	counts = (int *) malloc(5 * sizeof(int));
	for(i = 0; i < 5; i++) counts[i] = 0;
	
	/*Inicializar o vector de estimativas a inifinito para todos os nós, excepto o destino que fica com estimativa zero*/
	/*Criar o set_to_visit com todos os nós excepto o destino*/
	/*Incrementar counts[inf] por cada AS, excepto a AS dest */
	set_to_visit = NULL;
	for(i = 0; i < AS_card; i++){
		if(i != dest){	
			est[i] = inf;	
			if(AS_table[i] != NULL){
				aux = AS_neighbor_new(i);
				aux->next = set_to_visit;
				set_to_visit = aux;
				counts[inf]++;
			}
		}
		else{
			est[i] = zero;
			counts[zero]++;
		}
	}

	/*Para cada vizinho de dest, actualizar a est e counts*/
	
	for(aux = AS_table[dest]->customers; aux != NULL; aux = aux->next){
		est[aux->num] = P;
		counts[inf]--;
		counts[P]++;
	}
	for(aux = AS_table[dest]->peers; aux != NULL; aux = aux->next){
		est[aux->num] = R;
		counts[inf]--;
		counts[R]++;
	}
	for(aux = AS_table[dest]->providers; aux != NULL; aux = aux->next){
		est[aux->num] = C;
		counts[inf]--;
		counts[C]++;
	}

	/*Iterar a actualizaçao de estimativas para todos os nós até todos os nós terem sido visitados
	ou ate já sabermos que se chega da origin até dest pelo melhor caminho possivel*/

	best_est = best_est_possible(counts);
	while(set_to_visit != NULL && best_est < inf){
	
		/*Encontrar o/um nó que chega a dest com o mínimo custo possível (best_est)*/
		selected_node = 0;
		
		/*ver se vamos tirar logo o primeiro nó*/
		if(est[set_to_visit->num] == best_est){
			selected_node = set_to_visit->num;
			aux = set_to_visit;
			parent = NULL;
		}
		
		/*se não vamos procurar um nó que chegue a dest com best_est*/
		else{
			for(aux = set_to_visit->next, parent = set_to_visit; aux != NULL && est[aux->num] > best_est; aux = aux->next, parent = parent->next);
			selected_node = aux->num;
		}
			
		/*Retirar o nó selected_node do set_to_visit*/
		set_to_visit = remove_node(aux, parent, set_to_visit);
		counts[best_est]--;
		
		/*Percorrer os vizinhos e actualizar as suas est*/
		/*Os customers do nó seleccionado podem chegar a dest por um caminho P*/
		for(aux = AS_table[selected_node]->customers; aux != NULL; aux = aux->next)
			if(est[aux->num] > P){
				est[aux->num] = P;
				counts[inf]--;
				counts[P]++;
			}
		
		/*Se o nó seleccionado chegar a dest por um caminho C, propaga essa info para os seus peers e providers*/
		if(best_est == C){

			/*Os peers do nó seleccionado podem chegar a dest por um caminho R*/
			for(aux = AS_table[selected_node]->peers; aux != NULL; aux = aux->next){
				if(est[aux->num] > R){
					if(est[aux->num] == inf) counts[inf]--;
					else counts[P]--;
					est[aux->num] = R;
					counts[R]++;
				}
			}
			/*Os providers do nó seleccionado podem chegar a dest por um caminho C*/
			for(aux = AS_table[selected_node]->providers; aux != NULL; aux = aux->next)
				if(est[aux->num] > C){
					if(est[aux->num] == inf) counts[inf]--;
					else if(est[aux->num] == P) counts[P]--;
					else counts[R]--;
					est[aux->num] = C;
					counts[C]++;
				}
		}
		
		best_est = best_est_possible(counts);
	}  
  

	/*Se nenhum nó chegar a dest com menos de inf, sabemos que já não mais nenhuma estimativa a ser econtrada
	 e o vector est já tem os seus valores finais*/
	/*Libertar o set_to_visit*/
	while(set_to_visit != NULL) set_to_visit = remove_node(set_to_visit, NULL, set_to_visit);
	
	/*Libertar o vector counts*/
	free(counts);

	/*Contar quantos caminhos de cada tipo foram encontrados*/
	for(i = 0; i < AS_card; i++)
		if(AS_table[i] != NULL)
			stat[est[i]]++;

	return;
}




/*Função que determina o tipo de rota entre o nó origin e o nó dest*/
int elected_route(int origin, int dest, AS_node ** AS_table){
	
	int est[AS_card]; /*estimativa do tipo de caminho com que a AS[i] chega a AS dest*/
	int i, selected_node, best_est;
	int * counts; /*vector em que se guarda quantas AS têm estimativa C, R, P ou inf para chegar ao destino*/
	AS_neighbor * set_to_visit, * aux, *parent;

	/*Criar o vector counts*/
	counts = (int *) malloc(5 * sizeof(int));
	for(i = 0; i < 5; i++) counts[i] = 0;
	
	/*Inicializar o vector de estimativas a inifinito para todos os nós, excepto o destino que fica com estimativa zero*/
	/*Criar o set_to_visit com todos os nós excepto o destino*/
	/*Incrementar counts[inf] por cada AS, excepto a AS dest */
	set_to_visit = NULL;
	for(i = 0; i < AS_card; i++){
		if(i != dest){	
			est[i] = inf;	
			if(AS_table[i] != NULL){
				aux = AS_neighbor_new(i);
				aux->next = set_to_visit;
				set_to_visit = aux;
				counts[inf]++;
			}
		}
		else{
			est[i] = zero;
			counts[zero]++;
		}
	}

	/*Para cada vizinho de dest, actualizar a est e counts*/
	
	for(aux = AS_table[dest]->customers; aux != NULL; aux = aux->next){
		est[aux->num] = P;
		counts[inf]--;
		counts[P]++;
	}
	for(aux = AS_table[dest]->peers; aux != NULL; aux = aux->next){
		est[aux->num] = R;
		counts[inf]--;
		counts[R]++;
	}
	for(aux = AS_table[dest]->providers; aux != NULL; aux = aux->next){
		est[aux->num] = C;
		counts[inf]--;
		counts[C]++;
	}

	/*Iterar a actualizaçao de estimativas para todos os nós até todos os nós terem sido visitados
	ou ate já sabermos que se chega da origin até dest pelo melhor caminho possivel*/

	best_est = best_est_possible(counts);
	while(set_to_visit != NULL && est[origin] > best_est){
	
		/*Encontrar o/um nó que chega a dest com o mínimo custo possível (best_est)*/
		selected_node = 0;
		
		/*ver se vamos tirar logo o primeiro nó*/
		if(est[set_to_visit->num] == best_est){
			selected_node = set_to_visit->num;
			aux = set_to_visit;
			parent = NULL;
		}
		
		/*se não vamos procurar um nó que chegue a dest com best_est*/
		else{
			for(aux = set_to_visit->next, parent = set_to_visit; aux != NULL && est[aux->num] > best_est; aux = aux->next, parent = parent->next);
			selected_node = aux->num;
		}
		/*Se encontrarmos um nó no passo anterior e a sua est for menor que inf vamos actualizar as est dos vizinhos desse nó*/
		if(best_est < inf){
			
			/*Retirar o nó do set_to_visit*/
			set_to_visit = remove_node(aux, parent, set_to_visit);
			counts[best_est]--;
			
			/*Percorrer os vizinhos e actualizar as suas est*/
			/*Os customers do nó seleccionado podem chegar a dest por um caminho P*/
			for(aux = AS_table[selected_node]->customers; aux != NULL; aux = aux->next)
				if(est[aux->num] > P){
					est[aux->num] = P;
					counts[inf]--;
					counts[P]++;
				}
			
			/*Se o nó seleccionado chegar a dest por um caminho C, propaga essa info para os seus peers e providers*/
			if(best_est == C){

				/*Os peers do nó seleccionado podem chegar a dest por um caminho R*/
				for(aux = AS_table[selected_node]->peers; aux != NULL; aux = aux->next){
					if(est[aux->num] > R){
						if(est[aux->num] == inf) counts[inf]--;
						else counts[P]--;
						est[aux->num] = R;
						counts[R]++;
					}
				}
				/*Os providers do nó seleccionado podem chegar a dest por um caminho C*/
				for(aux = AS_table[selected_node]->providers; aux != NULL; aux = aux->next)
					if(est[aux->num] > C){
						if(est[aux->num] == inf) counts[inf]--;
						else if(est[aux->num] == P) counts[P]--;
						else counts[R]--;
						est[aux->num] = C;
						counts[C]++;
					}
			}
		}

		/*Se nenhum nó chegar a dest com menos de inf, então não há caminho entre origin e dest
		e já não queremos visitar mais nós*/		
		else while(set_to_visit != NULL) set_to_visit = remove_node(set_to_visit, NULL, set_to_visit);
		
		best_est = best_est_possible(counts);
	}
	
	/*Libertar o vector counts*/
	free(counts);

	return est[origin];
}

/*
 * Imprime uma lista de ASes com as seguintes informaçoes: AS_num, lista de customers, lista de peers, lista de providers
 */

void print_AS_table(AS_node ** AS_table){
  
  AS_node * aux;
  AS_neighbor * n;
  int i;
  
  for(i = 0; i < AS_card; i++){	/*percorre a AS_table*/
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
	  printf("A relação entre o nó %d e o nó %d não está bem definida!", asn, neighborn);
	  exit(1);
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

AS_topnode * AS_topnode_new(int asn, AS_neighbor *start, AS_topnode *next){
  AS_topnode *new = (AS_topnode *) malloc(sizeof(AS_topnode));

  new->asn = asn;
  new->start = start;
  new->next = next;

  return new;
}


/* 
	Verifica se uma dada rede é policy connected
 */
int IsConnected(AS_node **AS_table){
	AS_topnode *top_head = NULL;
	AS_topnode *pt, *ptaux;
	AS_neighbor *naux;
	int i = 0;
	int found = 0;

	for(i = 0; i<AS_card ; i++)
		if(AS_table[i] != NULL)
			if(AS_table[i]->providers == NULL){ /* Procura ASes do topo (sem providers) */
				/* Ordena a lista dos peers de forma decrescente (Pode ser feito fora do algoritmo) */
				AS_table[i]->peers = Sort_List(AS_table[i]->peers,i,&naux);
				/* Insere um novo AS 'top' numa lista que fica ordenada de forma decrescente de ASes */
				top_head = AS_topnode_new(i,naux,top_head);
			}


	for(pt = top_head ; pt != NULL ; pt = pt->next){ /* Ponteiro que percorre a lista dos top ASes */
		naux = pt->start;
		/* Procura os ASes que fazem parte do grupo top e compara com os seus peers */
		for(ptaux = pt->next; ptaux != NULL; ptaux = ptaux->next){ 
			for(found =  0; naux != NULL ; naux = naux->next){ 
				if(naux->num == ptaux->asn){
					found = 1;
					naux = naux->next;
					break;
				}
			}
			/* Terminou de visitar a lista de peers e não encontrou o AS top, não é policy connected, sair.*/
			if(found == 0)
				return 0;
		}
	}
	return 1;
}


/* 

	Utiliza o algoritmo 'Insertion Sort' para ordenar de forma decrescente uma lista de ASes

	O algoritmo foi adaptado da versão em código diponível em http://en.wikipedia.org/wiki/Insertion_sort

*/
AS_neighbor * Sort_List(AS_neighbor *pList, int asn, AS_neighbor **start) {
	AS_neighbor *head, *current, *p;
	int diff = 65536;
	*start = NULL;

    /* zero or one element in list*/
    if(pList == NULL || pList->next == NULL)
        return pList;
    /* head is the first element of resulting sorted list*/
    head = NULL;
    while(pList != NULL) {
        current = pList;
        pList = pList->next;

        if( ((asn - current->num) > 0 ) && ((asn - current->num) < diff) ){
        	diff = asn - current->num;
        	*start = current;
        }

        if(head == NULL || current->num > head->num) {
            /* insert into the head of the sorted list*/
            /* or as the first element into an empty sorted list*/
            current->next = head;
            head = current;
        } else {
            /* insert current element into proper position in non-empty sorted list*/
            p = head;
            while(p != NULL) {
                if(p->next == NULL || /* last element of the sorted list*/
                   current->num > p->next->num) /* middle of the list*/
                {
                    /* insert into middle of the sorted list or as the last element*/
                    current->next = p->next;
                    p->next = current;
                    break; /* done*/
                }
                p = p->next;
            }
        }
    }
    return head;
}



int main(int argc, char *argv[]){
	char c;
	int i;
	float ascount = 0.0;
	float stat[5] = {0,0,0,0,0};
	float j = 0.0;
	float total_paths = 0;
	int asn1, asn2, rel;
	FILE *fp;
	char buff[128];
	AS_node ** AS_table;

	if(argc != 2){
		printf("./internet network_file.txt\n");
		exit(1);
	}

	AS_table = (AS_node **) malloc (AS_card * sizeof(AS_node *));
	for(i = 0; i<AS_card;i++) /* O MAC não mete por defeito os ponteiros alocados a NULL e sem isto dá-me seg_fault na verificação de NULL da print_AS_table, depois pode-se apagar */
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

	for(i = 0; i < AS_card; i++)
	   if(AS_table[i] != NULL)
	     ascount++;
	
	 printf("[a] Determinar tipos de rotas entre duas ASes\n");
	 printf("[b] Determinar estatísticas sobre as rotas da rede\n");
	 printf("[c] Determinar se a rede é policy connected\n");
	 printf("[q] Sair\n");
	 printf("Selecione uma das opcões:\n");   
	   
	while(1){
	 scanf("%c", &c);
	 if(c == 'a'){
	   while(1){
	      printf("[Source] [Dest] :");
	      scanf("%d %d",&asn1, &asn2);
	      if(AS_table[asn1] == NULL){
		printf("A AS %d não existe!\n", asn1);
		exit(1);
	      }
	      if(AS_table[asn2] == NULL){
		printf("A AS %d não existe!\n", asn2);
		exit(1);
	      }
	      printf("De %d para %d: elected_route = %d\n", asn1, asn2, elected_route(asn1, asn2, AS_table));
	   }
	 }
	 else if(c == 'b'){
	   for(i = 1; i < AS_card; i++)
		if(AS_table[i] != NULL){
			all_elected_route(i , AS_table, &stat[0]);
			j++;
			printf("%f%% complete\n", 100*j/ascount);
		}

	    total_paths = stat[1] + stat[2] + stat[3] + stat[4];
	    printf("Customer: %.2f%%\nProvider: %.2f%%\nPeer: %.2f%%\nUnreachable: %.2f%%\n", 
		    100*(stat[1]/total_paths), 100*(stat[3]/total_paths), 100*(stat[2]/total_paths), 100*(stat[4]/total_paths)); 
	 }
	 else if(c == 'c'){
	   if(IsConnected(AS_table) == 1) printf("\nA rede é policy connected.\n\n");
	   else printf("\nA rede não é policy connected.\n\n");
	   
	 } 
	 else if(c == 'q'){
	   break;
	 } 
	}

	return 0;
}
