#ifndef ELIMINATION_H
#define ELIMINATION_H

/* Dado o indice de flights, elimina o elemento nessa posição */
void pop(int i);

/* Remove todos os voos com um dado flight_code */
void remove_flight(char* flight_code);

/* 
 * Apaga todas as ocorrencias na hashtable de reservas cujo o fligh code é 
 * code. Eliminando, ainda o nó da hashtable de ponteiros que continha o 
 * ponteiro para a reserva eliminada
 */
void delete(lnode **head_ref, ptr_node **ptr_head_ref, 
            char* code, int i, int M);
/* 
 * Apaga a ocurrencia na hashtable das reservas da reserva cujo r
 * eservation code é igual a res_code 
 */
void delete_with_res_code(lnode **head_ref, char* res_code, int i);

/* 
 * Esta função tem como objetivo eliminar as reservas com base no r_code recebido.
 * A estratégia passa por:
 * - Encontrar o nó da hashtable de ponteiros que contem o ponteiro que aponta 
 * para a reserva com r_code
 * - Removemos esse nó da hashtable
 * - Se delete_res = 1, vamos ainda remover a reserva para qual esse ponteiro
 * aponta na hashtable de reservas
 */ 
void delete_ptr(lnode **head_ref, ptr_node **ptr_head_ref, int i, char *r_code, int M, int delete_res);

/* Comando e, elimina voos ou reserva consoante o codigo dado */
void elimination(lnode **head_ref, ptr_node **ptr_head_ref, int M);

#endif
