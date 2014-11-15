#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "internet.h"

#define AS_card 65536
#define zero 0
#define C 1
#define R 2
#define P 3
#define inf 4

#if 0
/*Não usada para já*/
/*Função que remove do conjunto set_to_visit o nó selected_node*/
AS_neighbor * remove_node(int selected_node, AS_neighbor * set_to_visit){

	AS_neighbor * aux, * remove;
	
	printf("vou remover: %d\n", selected_node);
	/*Encontrar na lista o nó a remover*/
	for(remove = set_to_visit; remove->num != selected_node; remove = remove->next);
	
	/*Se o nó a remover for o primeiro elemento:*/
	if(remove == set_to_visit){
		set_to_visit = remove->next;
	}
	
	/*Caso contrário:*/
	else{
		/*Encontrar o nó anterior aquele que vamos remover*/
		for(aux = set_to_visit; aux->next != remove; aux = aux->next);
		aux->next = remove->next;
	}
	free(remove);

	return set_to_visit;
}
#endif

#if 0
/*Não usada para já*/
/*Função que determina o tipo de rota entre o nó origin e o nó dest*/
int elected_route(int origin, int dest, AS_node ** AS_table){
	
	int est[AS_card]; /*estimativa do tipo de caminho com que a AS[i] chega a AS dest*/
	int i, selected_node, selected_est;
	AS_neighbor * set_to_visit, * aux;

	/*Inicializar o vector de estimativas a inifinito para todos os nós, excepto o destino que fica com estimativa zero*/
	/*Criar o set_to_visit com todos os nós excepto o destino*/
	set_to_visit = NULL;
	for(i = 0; i < AS_card; i++){
		if(i != dest){
			est[i] = inf;
			if(AS_table[i] != NULL){
				aux = AS_neighbor_new(i);
				aux->next = set_to_visit;
				set_to_visit = aux;
			}
		}
		else est[i] = zero;
	}

	/*Para cada vizinho de dest, actualizar a est*/
	
	for(aux = AS_table[dest]->customers; aux != NULL; aux = aux->next) est[aux->num] = P;
	for(aux = AS_table[dest]->peers; aux != NULL; aux = aux->next) est[aux->num] = R;
	for(aux = AS_table[dest]->providers; aux != NULL; aux = aux->next) est[aux->num] = C;

	/*Iterar a actualizaçao de estimativas para todos os nós até todos os nos terem sido visitados
	ou ate sabermos que se chega da origin até dest por um customer path (C)*/

	while(set_to_visit != NULL && est[origin] > C){
	
		/*Encontrar o/um nó que chega a dest com o mínimo custo possível*/
		selected_node = 0;
		selected_est = inf;
		for(aux = set_to_visit; aux != NULL; aux = aux->next){
			if(est[aux->num] < selected_est){
				selected_node = aux->num;
				selected_est = est[aux->num];
			}
		}

		/*Se encontrarmos um nó no passo anterior e a sua est for menor que inf vamos actualizar as est dos vizinhos desse nó*/
		if(selected_est < inf){
			
			/*Retirar o nó do set_to_visit*/
			set_to_visit = remove_node(selected_node, set_to_visit);
			
			/*Percorrer os vizinhos e actualizar as suas est*/
			/*Os customers do nó seleccionado podem chegar a dest por um caminho P*/
			for(aux = AS_table[selected_node]->customers; aux != NULL; aux = aux->next)
				if(est[aux->num] > P) est[aux->num] = P;
			
			/*Se o nó seleccionado chegar a dest por um caminho C, propaga essa info para os seus peers e providers*/
			if(selected_est == C){

				/*Os peers do nó seleccionado podem chegar a dest por um caminho R*/
				for(aux = AS_table[selected_node]->peers; aux != NULL; aux = aux->next)
					if(est[aux->num] > R) est[aux->num] = R;

				/*Os providers do nó seleccionado podem chegar a dest por um caminho C*/
				for(aux = AS_table[selected_node]->providers; aux != NULL; aux = aux->next)
					if(est[aux->num] > C) est[aux->num] = C;

			}
		}

		/*Se nenhum nó chegar a dest com menos de inf, então não há caminho entre origin e dest
		e já não queremos visitar mais nós*/		
		else while(set_to_visit != NULL) remove_node(set_to_visit->num, set_to_visit);
	}

	return est[origin];
}
#endif

/*NOVO*/
/*Cria um nó do tipo AS_tree_node*/
AS_tree_node * AS_tree_node_new(int asn){

	AS_tree_node * new = (AS_tree_node *) malloc (sizeof (AS_tree_node));
	
	new->asn = asn;
	new->next = NULL;
	new->left = NULL;
	new->right = NULL;
	
	return new;
}

/*NOVO*/
/*Imprime uma árvore binária. Usada para verificar a árvore*/
void preorder_print(AS_tree_node * root){
	
	if(root != NULL){
		printf("asn: %d\n", root->asn);
		preorder_print(root->left);
		preorder_print(root->right);
	}
	
	return;
}

#if 0
void insert_to_BST(AS_tree_node *root, int asn){
	AS_tree_node *aux;
	if(root == NULL)
		root = AS_tree_node_new(asn);
	else{
		aux = root;
		while(1){
			if(aux->asn < asn){
				if(aux->left == NULL){
					aux->right = AS_tree_node_new(asn);
					break;
				}
				else
					aux = aux->left;
			}
			else {
				if(aux->right == NULL){
					aux->right = AS_tree_node_new(asn);
					break;
				}
				else
					aux = aux->right;
			}
		}
	}
	return;
}
#endif

void AS_tree_insert(AS_tree_node **root, int asn){
	AS_tree_node *node;

	node = search_node(*root,NULL,asn);

	if(node == NULL){
		*root = AS_tree_node_new(asn);
		printf("Insert: %d\n",asn);
		preorder_print(*root);
		return;
	}

	if(asn < node->asn)
		node->left = AS_tree_node_new(asn);
	else
		node->right = AS_tree_node_new(asn);

	printf("Insert: %d\n",asn);
	preorder_print(*root);

	return;
}

void AS_tree_remove(AS_tree_node **root, int asn){
	AS_tree_node *node, *parent, *successor;

	node = search_node(*root,&parent,asn);

	if(parent == NULL){
		if(node->left == NULL){
			if(node->right == NULL)
				*root = NULL;
			else
				*root = node->right;

			printf("Remove_1: %d\n",asn);
			preorder_print(*root);

			free(node);
			return;
		}
		else if(node->right == NULL){
			*root = node->left;
			printf("Remove_1: %d\n",asn);
			preorder_print(*root);

			free(node);
			return;
		}
	}




	if((node->left != NULL) && (node->right != NULL)){
		successor = node->right;
		parent = node;
		while(successor->left != NULL){
			parent = successor;
			successor = successor->left;
		}

		node->asn = successor->asn;
		node = successor;
	}

	if(parent->left == node){
		if(node->left == NULL)
			parent->left = node->right;
		else
			parent->left = node->left;
	}else{
		if(node->left == NULL)
			parent->right = node->right;
		else
			parent->right = node->left;
	}
	free(node);

	printf("Remove: %d\n",asn);
	preorder_print(*root);
}

AS_tree_node *search_node(AS_tree_node *root, AS_tree_node **parent, int asn){
	
	if(root == NULL)
		return NULL;
	if(root->asn == asn){
		*parent = NULL;
		return root;
	}

	while(1){
		if(asn < root->asn){
			if(root->left != NULL){
				if(root->left->asn == asn){
					*parent = root;
					return root->left;
				}else
					root = root->left;
			}else{
				return root;
			}
		}else{
			if(root->right != NULL){
				if(root->right->asn == asn){
					*parent = root;
					return root->right;
				}else
					root = root->right;
			}else{
				return root;
			}
		}
	}

	return NULL;
}




/*NOVO*/
/*Transforma uma lista que tem incício em first, numa árvore binária com raíz em root (valor do meio da lista)*/
void list_to_tree(AS_tree_node * root, AS_tree_node * first, int n){

	float nr, nl;
	int i;
	AS_tree_node * aux = first;
	
	nr = ceil(((float) n / 2.0) - 1);
	nl = floor ((float) n / 2.0);
	
	if(first == root) return;
	
	/*avançar até metade da lista esquerda e selccionar esse nó como filho esquerdo da raiz*/
	for(i = 0; i < (int) floor(nl / 2.0); i++) aux = aux->next;
	root->left = aux;
	
	/*avançar até metade da lista direita e selccionar esse nó como direito da raiz*/
	aux = root->next;
	for(i = 0; i < (int) floor(nr / 2.0); i++) aux = aux->next;
	root->right = aux;
	
	aux = root->next;
	root->next = NULL;
	
	list_to_tree(root->left, first, nl);
	list_to_tree(root->right, aux, nr);
	
	return;

}


/*Em manutenção*/
/*Calcula a rota entre o nó origem e dest*/
/*Por agora faz:
- cria o set to visit como uma lista a partir de set_to_visit[inf]
- transforma essa lista numa arvore. set_to_visit[inf] aponta para a raiz da arvore
- imprime a árvore
-(tenho o resto escrito num papel :p) */
int elected_route_new(int origin, int dest, AS_node ** AS_table){

	int est[AS_card];
	int i, as_count = 0;
	int selected_est = inf;
	int selected_node = 0;

	AS_tree_node ** set_to_visit, * aux, * root;
	AS_neighbor *aux_n;
	
	set_to_visit = (AS_tree_node **) malloc(5 * sizeof(AS_tree_node *)); 
	
	for(i = AS_card - 1; i >= 0; i--){
		if(i != dest){
			est[i] = inf;
			
			if(AS_table[i] != NULL){
				aux = AS_tree_node_new(i);
				aux->next = set_to_visit[inf];
				set_to_visit[inf] = aux;
				as_count++;
			}
		}
		else est[i] = zero;
	}
	
	root = set_to_visit[inf];
	for(i = 0; i < floor ((float) as_count / 2.0); i++) root = root->next;
	
	list_to_tree(root, set_to_visit[inf], as_count);
	set_to_visit[inf] = root;
	preorder_print(set_to_visit[inf]);

	/* Actualizar as estimativas dos clientes do nó 'dest' */
	for(aux_n = AS_table[dest]->customers; aux_n != NULL ; aux_n = aux_n->next){
		est[aux_n->num] = P;
		AS_tree_remove(&set_to_visit[inf], aux_n->num);
		AS_tree_insert(&set_to_visit[P], aux_n->num);
		selected_est = P;
	}

	/* Actualizar as estimativas dos peers do nó 'dest' */
	for(aux_n = AS_table[dest]->peers; aux_n != NULL ; aux_n = aux_n->next){
		est[aux_n->num] = R;
		AS_tree_remove(&set_to_visit[inf], aux_n->num);
		AS_tree_insert(&set_to_visit[R], aux_n->num);
		selected_est = R;
	}
	/* Actualizar as estimativas dos providers do nó 'dest' */
	for(aux_n = AS_table[dest]->providers; aux_n != NULL ; aux_n = aux_n->next){
		est[aux_n->num] = C;
		AS_tree_remove(&set_to_visit[inf], aux_n->num);
		AS_tree_insert(&set_to_visit[C], aux_n->num);
		selected_est = C;
	}

	while(set_to_visit_empty(set_to_visit) == 0 && best_est_found(set_to_visit, est[origin]) == 0){
		selected_est = inf;

		/* Encontrar um nó que chegue ao destino com a minima estimativa possivel */
		if(set_to_visit[C] != NULL){
			selected_node = AS_tree_rand_remove(&set_to_visit[C]);
			selected_est = C;
		} else if(set_to_visit[R] != NULL){
			selected_node = AS_tree_rand_remove(&set_to_visit[R]);
			selected_est = R;
		} else if(set_to_visit[P] != NULL){
			selected_node = AS_tree_rand_remove(&set_to_visit[P]);
			selected_est = P;
		}

		/* Percorrer os vizinhos do selected node e actualizar as estimativas */
		if(selected_est < inf){
			for(aux_n = AS_table[selected_node]->customers; aux_n != NULL ; aux_n = aux_n->next){
				est[aux_n->num] = P;
				AS_tree_remove(&set_to_visit[inf], aux_n->num);
				AS_tree_insert(&set_to_visit[P], aux_n->num);
			}

			if(selected_est == C){
				for(aux_n = AS_table[selected_node]->peers; aux_n != NULL ; aux_n = aux_n->next){
					if(est[aux_n->num] == inf) AS_tree_remove(&set_to_visit[inf], aux_n->num);
					else AS_tree_remove(&set_to_visit[P], aux_n->num);
					est[aux_n->num] = R;
					AS_tree_insert(&set_to_visit[R], aux_n->num);
				}
				for(aux_n = AS_table[selected_node]->providers; aux_n != NULL ; aux_n = aux_n->next){
					if(est[aux_n->num] == inf) AS_tree_remove(&set_to_visit[inf], aux_n->num);
					else if(est[aux_n->num] == P) AS_tree_remove(&set_to_visit[P], aux_n->num);
					else AS_tree_remove(&set_to_visit[R], aux_n->num);

					est[aux_n->num] = C;
					AS_tree_insert(&set_to_visit[C], aux_n->num);
				}
			}
		}
	}
	
	/*
	preorder_print(set_to_visit[inf]);
	AS_tree_remove(set_to_visit[inf],2);
	printf("\n");
	preorder_print(set_to_visit[inf]);
	AS_tree_insert(set_to_visit[inf],2);
	printf("\n");
	preorder_print(set_to_visit[inf]);
	AS_tree_remove(set_to_visit[inf],4);
	printf("\n");
	preorder_print(set_to_visit[inf]);*/

	
	return est[origin];
}

int set_to_visit_empty(AS_tree_node **set_to_visit){
	if(set_to_visit[C] == NULL && set_to_visit[R] == NULL 
		&& set_to_visit[P] == NULL && set_to_visit[inf] == NULL) return 1;
	return 0;
}

int best_est_found(AS_tree_node **set_to_visit, int est){
	if(est == C) return 1;
	if(set_to_visit[C] != NULL && est > C) return 0;
	if(set_to_visit[R] != NULL && est > R) return 0;
	if(set_to_visit[P] != NULL && est > P) return 0;

	return 1;
}

int AS_tree_rand_remove(AS_tree_node **root){
	AS_tree_node *node, *parent;
	int asn;

	node = *root;
	parent = node;
	if(node->left == NULL && node->right == NULL){
		asn = node->asn;
		free(node);
		root = NULL;
		printf("RandRemove: %d\n",asn);
		preorder_print(*root);
		return asn;
	}


	if(node->right == NULL)
		node = node->left;
	else if(node->left == NULL)
		node = node->right;
	else{
		if(rand()%2 == 0)
			node = node->left;
		else
			node = node->right;
	}


	while(node->left != NULL || node->right != NULL){
		parent = node;
		if(node->left == NULL)
			node = node->right;
		else if(node->right == NULL)
			node = node->left;
		else if(rand()%2 == 0)
			node = node->left;
		else
			node = node->right;
	}

	/* cheguei à folha que quero remover */
	asn = node->asn;
	if(parent->asn > asn) parent->left = NULL;
	else parent->right = NULL;
	free(node);

	printf("RandRemove: %d\n",asn);
	preorder_print(*root);

	return asn;
}



#if 0
/*ignorar*/
/*Função que determina o tipo de rota entre o nó origin e o nó dest*/
int elected_route_new(int origin, int dest, AS_node ** AS_table){
	
	int est[AS_card]; /*estimativa do tipo de caminho com que a AS[i] chega a AS dest*/
	int i, selected_node, selected_est;
	AS_neighbor * set_to_visit, * aux;

	/*Inicializar o vector de estimativas a inifinito para todos os nós, excepto o destino que fica com estimativa zero*/
	/*Criar o set_to_visit com todos os nós excepto o destino*/
	set_to_visit = NULL;
	for(i = 0; i < AS_card; i++){
		if(i != dest){
			est[i] = inf;
			if(AS_table[i] != NULL){
				aux = AS_neighbor_new(i);
				aux->next = set_to_visit;
				set_to_visit = aux;
			}
		}
		else est[i] = zero;
	}

	/*Para cada vizinho de dest, actualizar a est*/
	
	for(aux = AS_table[dest]->customers; aux != NULL; aux = aux->next) est[aux->num] = P;
	for(aux = AS_table[dest]->peers; aux != NULL; aux = aux->next) est[aux->num] = R;
	for(aux = AS_table[dest]->providers; aux != NULL; aux = aux->next) est[aux->num] = C;

	/*Iterar a actualizaçao de estimativas para todos os nós até todos os nos terem sido visitados
	ou ate sabermos que se chega da origin até dest por um customer path (C)*/

	while(set_to_visit != NULL && est[origin] > C){
	
		/*Encontrar o/um nó que chega a dest com o mínimo custo possível*/
		selected_node = 0;
		selected_est = inf;
		for(aux = set_to_visit; aux != NULL; aux = aux->next){
			if(est[aux->num] < selected_est){
				selected_node = aux->num;
				selected_est = est[aux->num];
			}
		}

		/*Se encontrarmos um nó no passo anterior e a sua est for menor que inf vamos actualizar as est dos vizinhos desse nó*/
		if(selected_est < inf){
			
			/*Retirar o nó do set_to_visit*/
			set_to_visit = remove_node(selected_node, set_to_visit);
			
			/*Percorrer os vizinhos e actualizar as suas est*/
			/*Os customers do nó seleccionado podem chegar a dest por um caminho P*/
			for(aux = AS_table[selected_node]->customers; aux != NULL; aux = aux->next)
				if(est[aux->num] > P) est[aux->num] = P;
			
			/*Se o nó seleccionado chegar a dest por um caminho C, propaga essa info para os seus peers e providers*/
			if(selected_est == C){

				/*Os peers do nó seleccionado podem chegar a dest por um caminho R*/
				for(aux = AS_table[selected_node]->peers; aux != NULL; aux = aux->next)
					if(est[aux->num] > R) est[aux->num] = R;

				/*Os providers do nó seleccionado podem chegar a dest por um caminho C*/
				for(aux = AS_table[selected_node]->providers; aux != NULL; aux = aux->next)
					if(est[aux->num] > C) est[aux->num] = C;

			}
		}

		/*Se nenhum nó chegar a dest com menos de inf, então não há caminho entre origin e dest
		e já não queremos visitar mais nós*/		
		else while(set_to_visit != NULL) remove_node(set_to_visit->num, set_to_visit);
	}

	return est[origin];
}

#endif
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



int main(int argc, char *argv[]){
	int i;
	int asn1, asn2, rel;
	FILE *fp;
	char buff[128];
	AS_node ** AS_table;

	srand(time(NULL));

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
/*
	print_AS_table(AS_table);
	for(i = 1; i < 8; i++)
		for(j = 1; j < 8; j++)
			printf("De %d para %d: elected_route = %d\n", i, j, elected_route(i, j, AS_table));
*/	

	while(1){
		printf("[Source] [Dest]: ");
		scanf("%d %d",&asn1,&asn2);
		printf("Elected route: %d\n",elected_route_new(asn1, asn2, AS_table));
	}
	
	return 0;
}
