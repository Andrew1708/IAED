#ifndef AIRPORTS_H
#define AIRPORTS_H
#define LEN_ID 3 /* Número de carateres do ID permitido*/
#define LEN_COUNTRY 30 /* Número de carateres do nome do país permitido*/
#define LEN_CITY 50 /* Número de carateres do nome da cidade premitido*/
#define LEN_SYSTEM 40 /* Número máximo de aeroportos suportado no sistema*/

/* Estrutura que representa um aeroporto */
typedef struct airport{
    char id[LEN_ID + 1];              /* ID do aeroporto*/
    char country[LEN_COUNTRY + 1];    /* Pais */
    char city[LEN_CITY + 1];          /* Cidade */
    int flights;                      /* Nº de voos com origem no aeroporto */
    int arrival_flights;              /* Nº de voos com chegada no aeroporto */
} Airport;

/* Variável global que representa o sistema de aeroportos */
extern Airport sys[LEN_SYSTEM]; 

/* Variável global que representa o numero de aeroportos registados no sys */
extern int airports_in_sys; 

/* 
 * Recebe uma cadeia de carateres, o id, e  verfica se este é válido
 * Devolve 1 se for válido, 0 caso contrário
 */
int is_ID_valid(Airport airport);

/* 
 * Devolve 1 se a variável sys tiver excedido o numero maximo de aeroportos
 * possiveis de guardar; devolve 0, caso contrário.
 */
int is_sys_full();

/*
 * Recebe um id e dois inteiros (0 ou 1) se o aeroporto existir devolve o
 * indice de onde este se encontra em sys, caso contrário devolve -1
 * Adicionalmente imprime as mensagens de erros se estas não estiverem
 * mute, i.e., se o valor para mute for 0
 */
int indx_in_sys(char id[], int mute1, int mute2);

/* 
 * Verifica se o novo aeroporto pode ser adicionado ao sys. Devolve 1 se poder
 * ser adicionado e 0, caso contrário (imprimindo 1ª mensagem de erro que 
 * invalida a condição)
 */
int can_add_airport(Airport new_airport);

/* 
 * Função que lê o input introduzido, e cria um novo aeroporto. Se este 
 * tiver um id válido (string de 3 carateres com apenas maísculas e não, 
 * existe um aeroporto com o mesmo id em sys) e ainda há espaço em sys,
 * adiciona-o ao sys, se não apenas imprime uma mensagem de erro
*/
void add_airport();

/* 
 * Similar ao BubbleSort, organiza os aeroportos no sys por ordem alfabética 
 * de id
 */
void sort_sys();

/* Recebe um aeroporto e imprimi-o */
void print_airport(Airport airport);

/* 
 * Devolve a lista de aeroportos ordenada alfabeticamente se não forem
 * dados argumentos ao comando, ou lista de aeroportos cujos ids foram 
 * fornecidos ao comando
 */
void airport_list();

#endif
