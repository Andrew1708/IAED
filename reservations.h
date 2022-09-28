#ifndef RESERVATIONS_H
#define RESERVATIONS_H

/* Estruta para reservas */
typedef struct reservation {
    char *flight_code;      /* Ponteiro para o código do voo da reserva */
    Date *res_date;         /* Data da reserva */
    char *res_code;         /* Codigo da reserva */
    int *passengers;        /* Nr de passageiros */
} Reservation;

/* Estrutura que representas os nós da hashtable de reservas */
typedef struct lnode{ 
   Reservation *r;      /* Ponteiro para a reserva */
   struct lnode *next;  /* Ponteiro para o nó seguinte */
} lnode;

/* Estrutura que representa os nós da hashtable de ponteiros */
typedef struct ptr_node{
    lnode *node;            /* Ponteiro para o nó da hashtable das reservas */
    struct ptr_node *next;  /* Ponteiro para o nó seguinte */
} ptr_node;

/* Função ser usada quando a memória se esgotar */
void check_memory(void *ptr);

/*
 * Função idêntica ao strdup que recebe uma string e devolve um ponteiro que
 * cuja a memoória foi dinâmicamente alocada par, aponta para essa string
 */
char* strdup_s(char *s);

/* Função de hashing, copiada dos slides */
int hashU(char *v, int M);

/* Inicializa hashtable, copiada dos slides */
lnode **Init(int m);

/* Inicializa hashtable, copiada dos slides */
ptr_node **ptr_Init(int m);

/* Insere novo elemento lnode na hashtable das reservas */
void insert(lnode **head_ref, int i, lnode *new);

/* Insere novo elemento ptr_node na hastable de ponteiros para reservas */
void insert_ptr(ptr_node **ptr_head_ref, int i, ptr_node *new_ptr);

/* 
 * "Destrói" a hashtable, libertando toda a memória previamente alocada
 * error = 1, nos casos em que acabou a memória e error = 0 no caso do
 * comando q
 */
void destroy_hashtables(lnode **head_ref, ptr_node **ptr_head_ref, int M);

/* 
 * Recebe os dados de reservation e aloca espaço 
 * na memória para um ponteiro Reservations devolvendo o seu endereço 
 */
Reservation *new_reservation(char *f_c, Date *r_d, char *r_c, int *p);

/* 
 * Liberta a memória de um dado ponteiro reservations 
 * (e todos os subsequentes ponteiros associados a esta estrutura 
 */
void free_reservation(Reservation *r);

/* Adiciona nó a lista ligada no inicio */
void push(lnode **head_ref, Reservation *new);

/* Insere novo nó a seguir a prev_node */
void insertAfter(lnode *prev_node, Reservation *new);

/* Adiciona novo nó no fim da lista ligada */
void append(lnode **head_ref, Reservation *new);

/*
 * Função auxiliar que adiciona uma nova reserva a uma lista ligada de 
 * reservas. Esta adição é feita de modo a que o a lista fique sempre
 * ordenada lexicograficamente.
 * 
 *  Nota: strcmp(s1, s2) devolve um valor
 * < 0, se s1<s2 (lexicograficamente); >0, se s1>s2; e =0, se s1=s2
 */ 
void add_sorted(lnode **head_ref, Reservation *new);

/*
 * Procura as reservas com o flight code indicado na hash table e adiciona-as,
 * de forma a que estejam ordenadas lexicograficamente, a uma lista ligada 
 * auxiliar. Por fim imprimi 1 a 1 cada elemento dessa lista
 * 
 */ 
void search_and_sort(char *f_code, Date* d, lnode **head_ref, int M);

/* 
 * Imprime todos os elementos de uma lista ligada, 
 * libertando a memória a medida que imprime 
 */
void print_list(lnode *list);

/*
 * Função que verifica se um dado codigo de reserva é válido, devolve 0 se
 * não (imprimindo a mensagem de erro), e 1 caso seja.
 */
int is_reservation_code_valid(char *r_code);

/* 
 * Dado o indice em flights e os novos passageiros a adicionar, determina se o
 * voo excede a capacidade, devolvendo um int > 0 se sim (imprimindo a mensagem de erro) 
 * e 0 caso contrário 
 */
int exceeds_capacity(int flight_index, int new_passengers);

/* 
 * Verifica se o valor de passageiros fornecido é válido. 
 * Deovlve 1 se sim e 0 caso contrário (imprimindo a mensagem de erro) 
 */
int is_valid_passenger_number(int p);

/* 
 * Procura na hashtable de ponteiros se existe algum ponteiro para uma reserva
 * com codigo igual a r_code. Esta função altera o valor do endereço de 
 * previous,correspondente a, caso existe, o endereço do nó anterior 
 * (isto simplifica o processo de remoção, caso seja necessário).
 * Devolve o endereço para o nó da hashtable
 */
ptr_node *search_for_node(ptr_node **ptr_head_ref, int i, 
                            char *r_code, ptr_node **previous);

/* 
 * Verifica se já existe alguma reserva com res_code. Devolvendo 1 e a mensagem
 * de erro caso sim, 0 caso não 
 */
int is_res_code_in_use(ptr_node **ptr_node_ref, int i, char *r_code);

/* Adiciona reserva à hashtable de reservas */
void add_reservation(char *f_code, Date d, char *r_code, int *p, 
                        lnode **head_ref, ptr_node **ptr_head_ref, int M);

/* Lê o input, devolve 1 se é uma nova reserva, 0 se for para consultar */
int read_input(char *f_code, Date *d, char *r_code, int *p);

/* 
 * Comando r, dependendo do input recebido, adiciona uma nova reserva à 
 * hashtable de reservas ou devolve a lista de reservas para um dado voo 
 */
void cmd_r(lnode **head_ref, ptr_node **ptr_head_ref, int M);

#endif
