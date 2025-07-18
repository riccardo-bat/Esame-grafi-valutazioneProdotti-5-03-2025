/*******************************/
/* HEADER MODULO "tipo" */
/*******************************/

#ifndef TIPO_H
#define TIPO_H

typedef struct{
	char descrizione[20];
	char tipo;
} tipo_inf;

int compare(tipo_inf,tipo_inf);
void copy(tipo_inf*,tipo_inf);
void print(tipo_inf);
int trovaUtenteProdotto(tipo_inf*, int, char*);


#endif
