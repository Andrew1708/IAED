#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "airports.h"
#include "commandT.h"
#include "flights.h"
#include "reservations.h"
#include "elimination.h"


Date date = {1, 1, 2022};

int is_past_date(Date date1, Date date2){
    return (date1.year < date2.year || (date1.year == date2.year && 
            (date1.month < date2.month || (date1.month == date2.month && 
            date1.day < date2.day))));
}


int is_valid(Date new_date){
    return (!is_past_date(new_date, date) && !(new_date.year > date.year && 
            (new_date.month > date.month || (new_date.month == date.month && 
            new_date.day > date.day)))) ? 1 : printf("invalid date\n") - 13;
}

int is_same_date(Date d1, Date d2){
    return (d1.day == d2.day && d1.month == d2.month && d1.year == d2.year);
}

void set_date(){
    Date new;

    /*Lê a nova data e guarda-a em new */
    scanf("%d-%d-%d", &new.day, &new.month, &new.year);
    if (is_valid(new)){
        /* Atulizamos a data e imprimos no formato DD-MM-YYYY */
        date = new;
        /* 
         * %.2d força a imprimir um inteiro de dois digitos, logo se o int
         * a imprimir for apenas um digito, x, será impresso como 0x
         */
        printf("%.2d-%.2d-%d\n", date.day, date.month, date.year);
    }
}
