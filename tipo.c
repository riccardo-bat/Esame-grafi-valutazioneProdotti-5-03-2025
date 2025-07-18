#include "tipo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(tipo_inf a, tipo_inf b){
    if(a.tipo < b.tipo) return -1;
    if(a.tipo > b.tipo) return 1;

    return strcmp(a.descrizione, b.descrizione);
}

void copy(tipo_inf* dest,tipo_inf source){
    *dest = source;
}

void print(tipo_inf obj){
    if(obj.tipo == 'U')
        printf("\nUtente %s", obj.descrizione);
    else
        printf("\nProdotto %s", obj.descrizione);
}

int trovaUtenteProdotto(tipo_inf* arrayNodes, int dim, char* descrizione){
    for(int i=0; i<dim; i++){
        if(strcasecmp(arrayNodes[i].descrizione, descrizione) == 0)
            return i;
    }

    return -1;
}