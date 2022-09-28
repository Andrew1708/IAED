#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "airports.h"
#include "commandT.h"
#include "flights.h"
#include "reservations.h"
#include "elimination.h"

int scheduled_flights = 0; 
Flight flights[LEN_FLIGHTS];

/*---------------------------------------------------------------------------
                             COMANDO V                   
---------------------------------------------------------------------------*/

int is_code_valid(char code[]){
    int len = strlen(code), i;

    if (3 <= len && len <= 6 && isupper(code[0])
            && isupper(code[1]) && isdigit(code[2]) && code[2] != 0) {
        /* Se code for maior que 3 verificamos se os 3 carateres são digitos */
        for(i = 3; i < len; i++){
            /* Se encontra um que não seja imprime o erro e retorna 0 */
            if (!isdigit(code[i])){
                printf("invalid flight code\n");
                return 0;
            }
        }
        /* Se chegou aqui é porque está tudo bem, retorna 1 */
        return 1;

    /* Não verificou o primeiro if, logo code é inválido */
    } else {   
        printf("invalid flight code\n");
        return 0;
    }
}

int flight_exists(char code[], Date date, int mute){
    int i;
    /* Percorremos o array flights */
    for(i = 0; i < scheduled_flights; i++){
        /* 
         * Se encontrarmos um voo em flights com o mesmo codigo e 
         * data de partida 
         */
        if(strcmp(flights[i].code, code) == 0 && 
        is_same_date(flights[i].date, date)) {
                /* Imprimimos a mensagem de erro e retonamos 0 */
                if (!mute){
                printf("flight already exists\n");
                }
                return i;
            }
    }
    /* Não encontrou nenhum voo igual*/
    if (mute){
        printf("%s: flight does not exist\n", code);
    }
    return -1;
}

int is_full(){
    return (scheduled_flights >= LEN_FLIGHTS) ? 
    (printf("too many flights\n") - 16) : 0;
}

int is_duration_valid(Time duration){
    return (duration.hour > 12 || (duration.hour == 12 && duration.minutes > 0)) ?
            (printf("invalid duration\n") - 17) : 1;
}

int is_capacity_valid(int capacity){
    return (10 <= capacity) ? 
            1 : (printf("invalid capacity\n") - 17);


}

int can_add_flight(Flight new_f){
    return (is_code_valid(new_f.code) && 
        (flight_exists(new_f.code, new_f.date, 0) == -1) &&
        !is_full() && is_valid(new_f.date) && 
        is_duration_valid(new_f.duration) &&
        is_capacity_valid(new_f.capacity));
}

void print_all_flights(){
        int i;
        for (i = 0; i < scheduled_flights; i++){
            /* %.2d força o printf a imprimir o inteiro dado com 2 digitos */
            printf("%s %s %s %.2d-%.2d-%d %.2d:%.2d\n", flights[i].code, 
            flights[i].departure, flights[i].arrival, flights[i].date.day,
            flights[i].date.month, flights[i].date.year,
            flights[i].time.hour, flights[i].time.minutes);
        }
}


void add_or_see_flights(){
    int depart, arriv;
    /* Se o carater lido for \n então não existe mais input */
    if (getchar() == '\n'){
        print_all_flights();
    }
    else{                   /* Há mais input a seguir a v */
        Flight new;         /* Cria-se o novo voo que vai ser adicionado */

        /* Lê o input e associa cada valor ao respetivo parâmetro de new */
        scanf("%s %s %s %d-%d-%d %d:%d %d:%d %d", new.code, new.departure, 
        new.arrival, &new.date.day, &new.date.month, &new.date.year, 
        &new.time.hour, &new.time.minutes, &new.duration.hour, 
        &new.duration.minutes, &new.capacity);
        /* Adiciona o voo se este verificar todos os requisitos */
        if (can_add_flight(new) && 
            ((depart = indx_in_sys(new.departure, 1, 0)) != -1) && 
            ((arriv = indx_in_sys(new.arrival, 1, 0)) != -1)){
            new.passengers = 0;
            /* Neste caso adicionamos o novo voo, new, ao array flights */
            flights[scheduled_flights] = new;
    /* Atuliza numero de voos em flights */
            scheduled_flights++;
    /* Atuliza o numero de voos do aeroporto de origem deste novo voo */
            sys[depart].flights++;
    /* Atuliza o numero de voos de chegada do aeroporto de chegada deste novo voo */
            sys[arriv].arrival_flights++;
        }
    }
}
/*---------------------------------------------------------------------------
                            COMANDO P E C               
---------------------------------------------------------------------------*/

int is_before_time(Time time1, Time time2){
    return(time1.hour < time2.hour || (time1.hour == time2.hour && 
            time1.minutes < time2.minutes));
}

void sort_flights(Flight list[], int n_flights){
    int i,j, done;
    Flight t;

    for(i = 0; i < n_flights ; i++){
        for(j = 0; j < n_flights - 1 - i; j++){
            /* 
             * Se os termos j e j+1 de list[] não estão ordenados
             * cronologicamente:
             */
            if (is_past_date(list[j+1].date, list[j].date) ||
                (is_same_date(list[j].date, list[j+1].date) && 
                is_before_time(list[j+1].time, list[j].time))){
                /* Troca os elementos da lista */
                t = list[j];
                list[j] = list[j+1];
                list[j+1] = t;
                done = 0;
            }  
        }
        if (done){
            break;
        }
    }
}

int is_30_day_month(int month){
    return (month == 4 || month == 6 || month == 9 || month == 11);
}

void update(Flight flight_list[], int n_flights){
    int i;

    for(i = 0; i < n_flights; i++){
        /* Soma-se os minutos da partida com os minutos da duração */
        if ((flight_list[i].time.minutes += flight_list[i].duration.minutes) 
            >= 60){
            /* Se a soma exceder os 60 somamos +1 às horas de chegada */
            flight_list[i].time.hour++;
            flight_list[i].time.minutes -= 60;
        }
        /* Soma-se a hora de partida às horas da duração */
        if ((flight_list[i].time.hour += flight_list[i].duration.hour) >= 24){
            /* Se exceder os 24 somamos +1 ao dia de chegada */
            flight_list[i].date.day++; 
            flight_list[i].time.hour -= 24; 
        }
        /* Se o dia "exceder o mês" */
        if ((flight_list[i].date.day == 31 && 
            is_30_day_month(flight_list[i].date.month)) || 
            (flight_list[i].date.day == 32 &&
            !is_30_day_month(flight_list[i].date.month)) ||
            (flight_list[i].date.month == 2 && flight_list[i].date.day == 29)){
            /* Avançamos a data para o dia 1 do próximo mês */
            flight_list[i].date.day = 1;
            flight_list[i].date.month++;
        }
        /* Se o mês exceder o ano */
        if(flight_list[i].date.month == 13){
            /* Avançamos para o ano seguinte */
            flight_list[i].date.month = 1;
            flight_list[i].date.year++;
        }
    }
}

void flight_list(char cmd){
    char id[LEN_ID + 1];
    /* n_flights é o numero de voos  */
    int i, j = 0, n_flights;
    /* Criamos o array que consiste na lista de voos */
    Flight list[LEN_FLIGHTS];

    scanf("%s", id);
    /* Verifica se id corresponde a um aeroporto em sys e guarda o indice */
    if ((i = indx_in_sys(id, 1, 0)) == -1) {
        /* Se i = -1 signfica que o id fornecido não consta em sys */
        return;
    }
    /* 
     * De modo a evitar iterações desnecessárias definimos o número de voos
     * que terá list que corresponde no caso do comando ser 'p' ao número de
     * voos com origem no aeroporto e for c e número de voos que chegam a esse
     * aeroporto
     */
    n_flights = ((cmd == 'p') ? sys[i].flights : sys[i].arrival_flights);
    /* Constrói a lista flight com n_flights elementos */
    for (i=0; j < n_flights; i++){
        /* 
         * Encontra os voos cujo id do aeroporto de origem, no caso de cmd = 'p'
         * (aeroporto de chegada, se cmd = 'c') é igual ao id e adicioana-os a
         * list
         */
        if ((cmd == 'p') ? !strcmp(flights[i].departure, id) : 
            !strcmp(flights[i].arrival, id)){
                /* i itera sob flights e j sob list */
                list[j] = flights[i];
                j++;
        }
    }
    /* 
     * Se cmd == c, precisamos de dar "update" ao tempo e às datas dos voos,
     * uma vez que passamos a querer o dia e a hora que os voos chegam. Depois
     * organizamos a lista por ordem cronologica de voos 
     */
    (cmd == 'c') ? update(list, n_flights), sort_flights(list, n_flights) :
    sort_flights(list, n_flights);
    /* Imprimimos todos os voos de list */
    for (j = 0; j < n_flights; j++){
        printf("%s %s %.2d-%.2d-%d %.2d:%.2d\n", list[j].code, (cmd == 'p') ? 
        list[j].arrival : list[j].departure, list[j].date.day, 
        list[j].date.month, list[j].date.year, list[j].time.hour, 
        list[j].time.minutes);
    }
}
