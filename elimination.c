#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "airports.h"
#include "commandT.h"
#include "flights.h"
#include "reservations.h"
#include "elimination.h"

void pop(int i){
    /* Puxa todos o elementos à frente flights[i] uma posição para trás */
    while(i < scheduled_flights){
        flights[i] = flights[i+1];
        i++;  
    }
    /* Decrementa o valor de voos marcados */
    scheduled_flights--;
}

void remove_flight(char* flight_code){
    int i = 0, have_found = 0;
    /* Percorre a lista de flights */
    while (i <= scheduled_flights){
        /* Se encontra um voo com o mesmo flight_code */
        if (!strcmp(flights[i].code, flight_code)){
            /* Elimina-o */
            pop(i);
            have_found = 1;
        }
        /* 
         * A incrementação só deverá ser feita se não alterarmos o 
         * array de modo a não saltarmos elementos 
         */
        else{
            i++;
        }
    }
    /* Se não encontrar nenhum, imprime-se a mensagem de erro */
    if (!have_found){
        printf("not found\n");
    }
    return;
}

void delete(lnode **head_ref, ptr_node **ptr_head_ref, 
            char* code, int i, int M){
    /* temp corresponde ao iterador */
    lnode *temp = head_ref[i], *prev;
    int k;
    /* 
     * Se o elemento estiver à cabeça (repete-se enquanto houver 
     * elementos a remover à cabeça) 
     */
    while(temp != NULL && !strcmp(temp->r->flight_code, code)){
        /* Elimina o nó da hashtable de ponteiros */
        k = hashU(temp->r->res_code, M);
        delete_ptr(head_ref, ptr_head_ref, k, temp->r->res_code, M, 0);
        /* Elimina o nó da hashtable de reservas */
        head_ref[i] = temp->next;  /* Altera a cabeça */
        free_reservation(temp->r); /* Liberta a cabeça antiga */
        free(temp);
        temp = head_ref[i];        /* temp passa a ser a nova cabeça */
    }
    prev = temp;
    /* Elimina elementos fora da cabeça */
    while (temp != NULL){
        /* Procura o elemento a eliminar, guardando sempre o nó anterior */
        while (temp != NULL && strcmp(temp->r->flight_code, code)){
            prev = temp;
            temp = temp->next;
        }
        /* Se o elemento não estiver presente na lista */
        if (temp == NULL){
            return;
        }

        /* Remove elemento */
        k = hashU(temp->r->res_code, M);
        delete_ptr(head_ref, ptr_head_ref, k, temp->r->res_code, M, 0);
        prev->next = temp->next;
        free_reservation(temp->r);

        /* Próxima iteração */
        temp = prev->next;
    }
    free(temp);
}

void delete_with_res_code(lnode **head_ref, char* res_code, int i){
    lnode *temp = head_ref[i], *prev = temp;

    /* Caso em que o elemento se encontra à cabeça da lista */
    if (!strcmp(temp->r->res_code, res_code)){
        head_ref[i] = temp->next;
        free_reservation(temp->r);
        free(temp);
        return;
    }

    temp = temp->next;
    /* Iteramos sobre a lista */
    while(temp != NULL){
        /* Encontramos o elemento a remover */
        if(!strcmp(res_code, temp->r->res_code)){
            /* Elimina-o */
            prev->next = temp->next;
            free_reservation(temp->r);
            free(temp);
            /* Retornamos, pois cada res_code é unico */
            return;
        }
        prev = temp;
        temp = temp->next;  
    }
}

void delete_ptr(lnode **head_ref, ptr_node **ptr_head_ref, int i, 
                        char *r_code, int M, int delete_res){
    int j;
    /* previous refere-se ao endereço do elemento antes do elemento a 
     * remover (to_delete) na hashtable de ponteiros */
    ptr_node **previous_ref = malloc(sizeof(ptr_node)), *previous, *to_delete;
    check_memory(previous_ref);
    previous_ref = NULL;

    /* Procuramos o elemento a remover */
    to_delete = search_for_node(ptr_head_ref, i, r_code, previous_ref);

    /* Não encontrou nenhum elemento com o r_code, não existe */
    if(to_delete == NULL){
        printf("not found\n");
        return;
    }
    else{
        /* O elemento a eliminar encontra-se à cabeça */
        if(previous_ref == NULL){
            /* Nova cabeça é o elemento a seguir ao que foi eliminado */
            ptr_head_ref[i] = to_delete->next;
        }
        else{
            /* Elimina nó to_delete */
            previous = *previous_ref;
            previous->next = to_delete->next;
        }
    }


    if(delete_res){
        /* Decrementa o numero de passageiros referente a esse voo */
        j = flight_exists(to_delete->node->r->flight_code, 
                            *to_delete->node->r->res_date, 1);
        flights[j].passengers -= *to_delete->node->r->passengers;

        /* Elimina a reserva com o codigo r_code da hashtable das reservas */
        i = hashU(to_delete->node->r->flight_code, M); /* key da hashtable */      
        delete_with_res_code(head_ref, r_code, i);
    }
    /* Liberta a memória */
    free(previous_ref);
}

void elimination(lnode **head_ref, ptr_node **ptr_head_ref, int M){
    int i;
    char *code = malloc(65536*sizeof(char));
    /* Lê o input recebido */
    scanf("%s", code);
    code = realloc(code, (strlen(code) + 1)*sizeof(char));
    i = hashU(code, M);

    /* Leu um código de reserva */
    if(strlen(code) >= 10){
        delete_ptr(head_ref, ptr_head_ref, i, code, M, 1);
    }
    /* Leu um código de voo */
    else{
    remove_flight(code);
    delete(head_ref, ptr_head_ref, code, i, M);
    }
    free(code);
}
