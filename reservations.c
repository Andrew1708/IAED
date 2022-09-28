#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "airports.h"
#include "commandT.h"
#include "flights.h"
#include "reservations.h"
#include "elimination.h"

void check_memory(void *ptr){                                                  
    if(ptr == NULL){
        printf("No memory");
        exit(0);
    }
    return;
}

char* strdup_s(char *s){
    char * new = malloc((strlen(s) + 1)*sizeof(char));
    if (new == NULL){
        return NULL;
    }
    strcpy(new,s);
    return new;
}

int hashU(char *v, int M){
  int h, a = 31415, b = 27183;
  for (h = 0; *v != '\0'; v++, a = a*b % (M-1))
    h = (a*h + *v) % M;
return h; 
}

lnode **Init(int m) {
  int i;
    lnode **heads = malloc(m * sizeof(lnode));
    for (i = 0; i < m; i++){ 
        heads[i] = NULL;
    }
    return heads;
}

ptr_node **ptr_Init(int m) {
  int i;
    ptr_node **heads = malloc(m * sizeof(ptr_node));
    for (i = 0; i < m; i++){ 
        heads[i] = NULL;
    }
    return heads;
}

void insert(lnode **head_ref, int i, lnode *new){
    /* Se a cabeça não estiver vazia */
    if (head_ref[i] != NULL){
        /* Adiciona new à cabeça, sendo next a antiga cabeça */
        new->next = head_ref[i];
        head_ref[i] = new;
    }
    /* Cabeça está vazia, é so adicionar */
    else{
        head_ref[i] = new;
    }
}

void insert_ptr(ptr_node **ptr_head_ref, int i, ptr_node *new_ptr){
    if (ptr_head_ref[i] != NULL){
        new_ptr->next = ptr_head_ref[i];
        ptr_head_ref[i] = new_ptr;
    }
    else{
        ptr_head_ref[i] = new_ptr;
    }
}

void destroy_hashtables(lnode **head_ref, ptr_node **ptr_head_ref, int M){     
    int i;
    /* Liberta todos os nodes */
    for(i = 0; i < M; i++){
        free(head_ref[i]);
        free(ptr_head_ref[i]);
    }
    /* Liberta os ponteiros */
    free(head_ref);
    free(ptr_head_ref);
}

Reservation *new_reservation(char *f_c, Date *r_d, char *r_c, int *p){
    Reservation *new = malloc(sizeof(Reservation));
    check_memory(new);

    new->flight_code = strdup_s(f_c);
    check_memory(new->flight_code);

    new->res_date = malloc(sizeof(Date));
    check_memory(new->res_date);
    new->res_date->day = r_d->day;
    new->res_date->month = r_d->month;
    new->res_date->year = r_d->year;

    new->res_code = strdup_s(r_c);
    check_memory(new->flight_code);

    new->passengers = malloc(sizeof(int));
    check_memory(new->passengers);
    new->passengers = p;
    return new;
}

void free_reservation(Reservation *r){
    free(r->flight_code);
    free(r->res_date);
    free(r->res_code);
    free(r->passengers);
    free(r);
}

void push(lnode **head_ref, Reservation *new){
    /* Cria novo nó */
    lnode *new_node = malloc(sizeof(lnode));
    /* Verifica se o malloc funcionou */
    check_memory(new_node);
    new_node->r = new;             /* Atribiu reserva do novo nó */
    new_node->next = (*head_ref);  /* Next do novo nó é a cabeça antiga */
    (*head_ref) = new_node;        /* Nova cabeça é o novo nó */
}

void insertAfter(lnode *prev_node, Reservation *new){
    lnode *new_node = malloc(sizeof(lnode));
    /*Verifica se malloc funcionou */
    check_memory(new_node);
    new_node->r = new;
    /* Adiciona novo nó entre prev_node e prev_node->next */
    new_node->next = prev_node->next; 
    prev_node->next = new_node;       
}

void append(lnode **head_ref, Reservation *new){
    lnode *new_node = malloc(sizeof(lnode));
    /* Varível que vai guardar o ultimo nó da lista */
    lnode *last = *head_ref;
    check_memory(new_node);
    new_node->r = new;
    /* Como pretendemos adicionar o novo nó fim o next deste tem de ser NULL */
    new_node->next = NULL;
    /* Se lista está vazia */
    if (*head_ref == NULL){
        /* O endereço da lista é apenas o novo elemento */
        *head_ref = new_node;
        return;
    }
    /* Se a lista não for vazia procuramos o ultimo elemento */
    while(last->next != NULL){
        last = last->next;
    }
    /* Ao encontrar colocamos o ponteiro deste nó a apontar para o new_node */
    last->next = new_node;
    return;
}

void add_sorted(lnode **head_ref, Reservation *new){
    lnode *current_node = *head_ref; /* Iterador da lista */
    /* Se a lista estiver vazia */
    if(*head_ref == NULL){
        /* Adicionamos o elemento */
        append(head_ref, new);
        return;
    }
    /* Se o elemento a adicionar for menor que o elemento da caveça */
    else if(strcmp(new->res_code, current_node->r->res_code) < 0){
        /* Adicionamos o novo elemento à cabeça da lista */
        push(head_ref, new);
        return;
    }
    /* Caso contrário vamos procurar o sitio onde adicionar o novo elemento */
    else{
        /* Iteramos a lista até chegarmos ao fim (proximo elemento ser NULL) */
        while ((current_node->next) != NULL){
            /* Corresponde a previous < new < next */
            if (strcmp(new->res_code, current_node->r->res_code) > 0 &&
                strcmp(new->res_code, current_node->next->r->res_code) < 0){
                /* Adiconamos depois de current */
                insertAfter(current_node, new);
                return;
            }
            current_node = current_node->next;
        }
    }
    /* Se chegar aqui é pq estamos no fim da lista (adicionamos o no nó)*/
    append(head_ref, new);
    return;
}

void search_and_sort(char *f_code, Date* d, lnode **head_ref, int M){
    /* Key da hash table */
    int i = hashU(f_code, M);
    lnode *curr = head_ref[i]; /* Iterador */
    /* Ponteiro para a lista ligada auxiliar */
    lnode **head_ref_aux = malloc(sizeof(lnode)); 
    /* Verifica malloc */
    check_memory(head_ref_aux);
    *head_ref_aux = NULL;
    /* Verifica se codigo e data são validos */
    if ((flight_exists(f_code, *d, 1) != -1) && is_valid(*d)){
        /* Percorre a lista ligada da hashtable */
        while(curr != NULL){
            /* Se corresponde ao elemento que queremos imprimir */
            if(!strcmp(curr->r->flight_code, f_code) &&
                is_same_date(*curr->r->res_date, *d)){
                    /* Adicionamo-lo ordeiramente na lista auxiliar */
                    add_sorted(head_ref_aux, curr->r);
                }
            curr = curr->next;
        }
        /* Chegamos ao fim da entrada da hashtable, imprimios a lista aux */
        print_list(*head_ref_aux);
    }
    free(head_ref_aux);
}

void print_list(lnode *list){
    lnode *aux; /* Auxilia o a libertção de memória */

    while(list != NULL){
        printf("%s %d\n", list->r->res_code, *list->r->passengers);
        aux = list;
        list = list->next;
        /* Liberta elemento da lista que foi já impresso */
        free(aux);
    }
}

int is_reservation_code_valid(char *r_code){
    int len, i;
    if ((len = strlen(r_code)) >= 10){
        /* Percorre a string */
        for (i = 0; i < len; i++){
            /* Se encontrar um carater inválido */
            if (!((r_code[i] >= 'A' && r_code[i] <= 'Z') || 
                (r_code[i] >= '0' && r_code[i] <= '9'))){
                printf("invalid reservation code\n");   
                return 0;
            }
        }
        /* 
        * Fazemos o realloc com o tamanho correto da string, de modo a não 
        * termos memoria alocada desnecessária 
        */
        r_code = realloc(r_code, (len +1) * sizeof(char));
        return 1;   
    }
    printf("invalid reservation code\n");
    return 0;
}

int exceeds_capacity(int flight_index, int new_passengers){
    int passengers_in_flight = flights[flight_index].passengers;
    return (passengers_in_flight + new_passengers) > 
        flights[flight_index].capacity ? 
        printf("too many reservartions\n"): 
        0;
}

int is_valid_passenger_number(int p){
    return p > 0 ? 1: printf("invalid passenger number\n") - 25;
}

ptr_node *search_for_node(ptr_node **ptr_head_ref, int i, 
                            char *r_code, ptr_node **previous){
    /* Iterador da lista */
    ptr_node *curr = ptr_head_ref[i];
    /* Só adicionei esta instruição pra que o programa compile, não faz nada */
    (void) previous;
    /* Se não houver nada nessa posição do dicionário */
    if (curr == NULL){
        return NULL;
    }
    /* Percorremos a lista */
    while(curr != NULL){
        /* Paramos ao encontrar o nó que procuramos */
        if (!strcmp(curr->node->r->res_code, r_code)){
            break;
        }
        /* Guardamos o endereço do "interador" e incrementamos */
        previous = &curr;
        curr = curr->next;
    }
    /* 
     * Devolvemos o endereço do nó, verificamos de novo a condição 
     * para os casos do inicio e quando chegamos ao fim da lista 
     */
    return !strcmp(curr->node->r->res_code, r_code) ? curr: NULL;
}

int is_res_code_in_use(ptr_node **ptr_node_ref, int i, char *r_code){
    /* Procuramos nó na hashtable de ponteiros */
    ptr_node *aux = search_for_node(ptr_node_ref, i, r_code, NULL);
    /* Se não existir é porque res_code não está em uso */
    if(aux != NULL){
        printf("%s: flight reservation already used\n", r_code);
        return 1;
    }
    return 0;
}

void add_reservation(char *f_code, Date d, char *r_code, int *p, 
                        lnode **head_ref, ptr_node **ptr_head_ref, int M){
    /* 
     * i - key da hashtable de reservas; 
     * j - indice do voo com f_code em flights; 
     * k - key da hashtable de ponteiros c
     */
    int i, j, k = hashU(r_code, M);
    lnode *new;
    ptr_node *new_ptr;

    /* Verifica todas as condições antes de adicionar a reserva */
    if(is_reservation_code_valid(r_code) && 
    ((j = flight_exists(f_code, d, 1)) != -1) &&
    !is_res_code_in_use(ptr_head_ref, k, r_code) && 
    !exceeds_capacity(j, *p) && is_valid(d) &&
    is_valid_passenger_number(*p)){
        /* Novo nó da hashtable das reservas */
        i = hashU(f_code, M);
        new =  malloc(sizeof(lnode));
        check_memory(new);
        /* Cria a nova reserva e guardamo-la no nó */
        new->r = new_reservation(f_code, &d, r_code, p);;
        new->next = NULL;
        /* Insere na hashtable a nova reserva */
        insert(head_ref, i, new);
        /* Incrementa o valor de passageiros no voo */
        flights[j].passengers += *p;

        /* Novo nó da hashtable de ponteiros */
        new_ptr = malloc(sizeof(ptr_node));
        check_memory(new_ptr);
        /* Guarda o ponteiro */
        new_ptr->node = new;
        new_ptr->next = NULL;
        /* Insere o nó na hashtable*/
        insert_ptr(ptr_head_ref, k, new_ptr);
    }
}

int read_input(char *f_code, Date *d, char *r_code, int *p){
    scanf("%s %d-%d-%d", f_code, &d->day, &d->month, &d->year);
    f_code = realloc(f_code, (strlen(f_code) + 1) * sizeof(char));
    if (getchar() == ' '){
        scanf("%s %d", r_code, p);
        return 1;
    }
    else{
        return 0;
    }
}

void cmd_r(lnode **head_ref, ptr_node **ptr_head_ref, int M){
    char *f_code = malloc(7*sizeof(char)), 
         *r_code = malloc(65536*sizeof(char));
    Date *d = malloc(sizeof(Date));
    int *p = malloc(sizeof(int));
    /* Verifica se houve uma alocação correta das variáveis */
    check_memory(f_code);
    check_memory(r_code);
    check_memory(d);
    check_memory(p);
    /* Lê input, e consoante o que for lido, realiza a tarefa pretendida */
    read_input(f_code, d, r_code, p) ? 
    add_reservation(f_code, *d, r_code, p, head_ref, ptr_head_ref, M):
    search_and_sort(f_code, d, head_ref, M);
    /* Liberta as variáveis */
}



