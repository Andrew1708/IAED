#ifndef COMMANDT_H
#define COMMANDT_H
/* Estrutura que representa a data */
typedef struct date{
    int day;
    int month;
    int year;
} Date;

/* Estrutura que representa o tempo */
typedef struct time{
    int hour;
    int minutes;
} Time;

/* Variável global que indica a Data "atual" */
extern Date date; 

/* 
 * Verifica se date1 é mais antiga que date2. Se for devolve 1, caso contrário
 * devolve 0
 */
int is_past_date(Date date1, Date date2);

/* 
 * Verifica se a data é valida. Devolve 1 se for, 0 caso contrário 
 * (imprimindo a mensagem de erro) 
 */
int is_valid(Date new_date);

/* 
 * Verifica se duas datas são identicas, devolvendo 1 caso sejam,
 * 0 caso contrário
 */
int is_same_date(Date d1, Date d2);

/* Lê nova data, se esta for válida atuliza a variável date */
void set_date();

#endif
