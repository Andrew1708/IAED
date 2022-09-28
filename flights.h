#ifndef FLIGHTS_H
#define FLIGHTS_H

#define LEN_FLIGHTS 30000 /* Número maximo de voos agendados */

/* Estrutura que representa os voos */
typedef struct flight{
    char code[7];                   /* Codigo de voo */
    char departure[LEN_ID + 1];     /* ID do aeroporto de partida */
    char arrival[LEN_ID +1];        /* ID do aeroporto de chegada */
    Date date;                      /* Data de partida */
    Time time;                      /* Hora de partida do voo */
    Time duration;                  /* Duração da viagem */
    int capacity;                   /* Capacidade do voo */
    int passengers;                 /* Numero de passageiros reegistados */
} Flight;

/* Variável global que guarda os voos */
extern Flight flights[LEN_FLIGHTS];

/* Variável global que indica o numero de voos registados */
extern int scheduled_flights; 

/* 
 * Função que verfica se o codigo de voo introduzido é válido. Devolve 1 se for
 * 0, caso contrário (imprimindo as mensagens de erro)
 */
int is_code_valid(char code[]);

/* 
 * Função que veriffica se o o voo já se econtra em flights. Devolve o indice
 * da lista flights onde esse voo se encontra. Caso o voo não exista devolve -1.
 * Imprime mensagens do terminal consoante o valor de mute. 
 */
int flight_exists(char code[], Date date, int mute);

/* 
 * Verifica se ainda há espaço em flights para agendar novos voos. Devolve 
 * 1 se flights estiver cheio (imprimindo a mensagem de erro) 
 * e 0 caso ainda tenha espaço
 */
int is_full();

/* 
 * Verifica se ainda se a duração do novo voo é valiada. Devovle 
 * 1 se for, e 0 caso contrário (imprimindo o erro)
 */
int is_duration_valid(Time duration);

/* 
 * Verifica se ainda se a capacidade do novo voo é valiada. Devolve 
 * 1 se for, e 0 caso contrário (imprimindo o erro)
 */
int is_capacity_valid(int capacity);

/*
 * Verifica se o voo que se pretende adicionar tem todos os parâmetros corretos
 * e se é possivel adicioná-lo a flights. Devolve 1 se sim, 0 caso contrário 
 * (imprimindo 1º erro que impossibilitou a adição deste voo)
 */
int can_add_flight(Flight new_f);

/* Imprime todos os voos em flights */
void print_all_flights();

/* 
 * Função que dependente do input adiciona um novo voo ou mostra lista de
 * voos adicionados até ao momento
 */
void add_or_see_flights();

/*
 * Recebe dois tempos e verifica se time1 é antes de time2. Se for devolve
 * 1, se não devolve 0
 */
int is_before_time(Time time1, Time time2);

/*
 * Função similar ao BubbleSort que recebe uma lista de voos, list[], e 
 * um número de voos dessa lista. Devolve list[] ordenada cronologicamente
 * dos voos a ocorrer (pela ordem que estes ocorrem no tempo)
 */
void sort_flights(Flight list[], int n_flights);

/*
 * Recebe um inteiro e verifca se este corresponde ao um mês de de 30 dias
 * Se for devolve 1, caso contrário devolve 0
 */
int is_30_day_month(int month);

/*
 * Função que recebe uma lista de voos, flight_list[], e um número de 
 * voos dessa lista, n_flights. Devolve a lista onde foi feito o update 
 * dos parâmetros date e time, que passam ser a data e tempo de chegada
 */
void update(Flight flight_list[], int n_flights);

/* 
 * Função que recebe um carateres correspondente ao comando ('p' ou 'c') e 
 * lê o id. No caso de cmd = 'p' vai devolver a lista de voos com partida
 * no aeroporto com o id lido odernados cronologicamente. No caso de cmd = 'c'
 * vai devolver a lista de voos com chegada no aeroporto com id lido ordenados
 * cronologicamente
 */
void flight_list(char cmd);


#endif
