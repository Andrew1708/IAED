/*
 * Projeto 1 e 2 de IAED
 * Autor : André Morgado (92737)
 * 
 * Descrição: O objetivo deste projeto foi desenvolver um sistema de gestão 
 * de voos entre aeroportos. Este sistema obedece a uma série de comandos
 * e todo o input é recebido através do terminal
 * 
 * Algumas notas:
 * - Tirei partido da função printf() retornar um inteiro correspondente ao
 * numero de carateres que imprimiu no terminal
 * 
 * - Utilizei a função isupper() que recebe um caratere e retorna um inteiro: 
 * = 0 se o caratere for uma letra maíuscula e >0 se não for
 * 
 * - Utilizei a função strcmp(str1, str2) que recebe duas strings e devolve 
 * um inteiro: = 0 se as strings forem iguais; >0 se str1 é "maior" que str2;
 * <0 se str1 é "menor" que str2
 * 
 * <<<<<<<<<<<<<<<<<<<<<<<<< REFERENTE AO PROJETO 2 >>>>>>>>>>>>>>>>>>>>>>>>>>>
 * 
 * Nesta secção utilizei 2 hashtables, uma que guarda as reservas cuja a chave
 * depede do codigo de voo. E outra que guarda os ponteiros para os 
 * nós/elementos da primeira hashtable, cujas chaves se obtem com base no
 * codigo de reserva (por outras palavras: uma hashtable que aponta para
 * outra hashtable, uniderecionalmente)
 * 
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "airports.h"
#include "commandT.h"
#include "flights.h"
#include "reservations.h"
#include "elimination.h"


int main(){
    int M = 30000; /* Tamanho das hashtables */

    /* Alocamos memória para os ponteiros da hashtable */
    lnode **head_ref;
    
    ptr_node **ptr_head_ref;
    
    /* Criamos a hashtable, obento a referencia para mesma */
    head_ref = Init(M);         /* Hashtable das reservas */
    check_memory(head_ref);     /* Verifica se há memória */
    ptr_head_ref = ptr_Init(M); /* Hashtable dos ponteiros */
    check_memory(ptr_head_ref);
    while(1){
        int c = getchar();

        switch (c){
        case 'q':
            destroy_hashtables(head_ref, ptr_head_ref, M);
            return 0;
        case 'a':
            add_airport();
            break;
        case 'l':
            airport_list();
            break;
        case 'v':
            add_or_see_flights();
            break;
        case 't':
            set_date();
            break;
        case 'p':
        case 'c':
            flight_list(c);
            break;
        case 'r':
            cmd_r(head_ref, ptr_head_ref, M);
            break;
        case 'e':
            elimination(head_ref, ptr_head_ref, M);
            break;
        default:
            break;
        }
    }
}

