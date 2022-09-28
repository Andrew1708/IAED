#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "airports.h"
#include "commandT.h"
#include "flights.h"
#include "reservations.h"
#include "elimination.h"

int airports_in_sys = 0;
Airport sys[LEN_SYSTEM];

/*---------------------------------------------------------------------------
                             COMANDO A                   
---------------------------------------------------------------------------*/

int is_ID_valid(Airport airport){
    /* Verifica se todos os carateres são maíusculos */
    return (isupper(airport.id[0]) && isupper(airport.id[1]) && 
            isupper(airport.id[2])) ? 1 : printf("invalid airport ID\n") - 19;
}

int is_sys_full(){
    return (airports_in_sys >= LEN_SYSTEM) ? 
            printf("too many airports\n") - 17 : 0; 
}

int indx_in_sys(char id[], int mute1, int mute2){
    int i;
    /* Percorre todos os aeroportos em sys */
    for (i = 0; i < airports_in_sys; i++){
        /* Se encontra um com mesmo id: */
        if (!strcmp(sys[i].id, id)){
            /* Imprime a mensagem de erro se mute1 = 0 */
            if (!mute1){
                printf("duplicate airport\n");
            }
            /* Retorna o indice de airport em sys */
            return i;
        }
    }
    /* Caso não encontre airport em sys sai do ciclo */

    /* Imprime a mensagem de erro se mute2 = 0 */
    if (!mute2){
        printf("%s: no such airport ID\n", id);
    }
    return -1;
}

int can_add_airport(Airport new_airport){
    return (is_ID_valid(new_airport) && !is_sys_full() 
            && indx_in_sys(new_airport.id, 0, 1) == -1);
}

void add_airport(){
    Airport new;

    /* Lê o input e guarda as strings lidas em new */
    scanf("%s %s %[^\n]s", new.id, new.country, new.city);

    if (can_add_airport(new)){
        /* Adiciona novo voo a sys */
        new.flights = 0;
        new.arrival_flights = 0;
        sys[airports_in_sys] = new;
        airports_in_sys++;

        /* Imprime a mensagem de sucesso */
        printf("airport %s\n", new.id);
    } 
} 

/*---------------------------------------------------------------------------
                             COMANDO L                   
---------------------------------------------------------------------------*/

void sort_sys(){
    int i,j, done;
    Airport t;
    for (i = 0; i < airports_in_sys; i++){
        for (j = 0; j < airports_in_sys - 1 - i; j++){
            /* Se o j-esimo e o j+1-esimo elemento de sys tiverem desordanados
            alfabeticamente: */
            if ((strcmp(sys[j].id, sys[j+1].id)) > 0){
                /* Trocamos estes termos */
                t = sys[j];
                sys[j] = sys[j+1];
                sys[j+1] = t;
                done = 0;
            }  
        }
        if (done){
            break;
        }
    }
}

void print_airport(Airport airport){
    printf("%s %s %s %d\n", airport.id, airport.city, airport.country, 
            airport.flights);
}

void airport_list(){
    char id[LEN_ID + 1];
    int i;

    /* Se não forem dados ID's o proximo careter lido é o '\n' (terminação) */
    if (getchar() == '\n'){
        sort_sys();              /* Ordenamos alfabeticamente sys */
        /* Iteramos sobre sys imprimindo todos os seus aeroportos */
        for (i = 0; i < airports_in_sys; i++){
            print_airport(sys[i]);
        }
    }
    else do{
        /* O ID seguinte é lido */
        scanf("%s", id);

        /* 
         * Se i = -1 é porque o ID lido não existe no sys, pelo que se devolve
         * apenas a mensagem de erro
         */
        if ((i = indx_in_sys(id, 1, 0)) != -1){
            print_airport(sys[i]);
        }
    } while(getchar() != '\n'); /* Repete-se o ciclo até não haver mais input */
}
