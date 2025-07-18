#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "tipo.h"

//prototipi 
long int int_input(char* msg, int minimum_value, int max_value);
char* string_input(char* msg, int max_length);
tipo_inf* creaNodi(int* n);
void load_graph_from_file(graph* grafo_valutazioni, tipo_inf* ArrayNodes, int dim);
void stampa(graph* grafo_valutazioni, tipo_inf* arrayNodes);
float media_utente(char* account, graph g, tipo_inf* arrayNodes, int dim);
float media_prod(char* prod, graph g, tipo_inf* arrayNodes, int dim);

int main(){
    int dim = 0;
    tipo_inf* arrayNodes = creaNodi(&dim);
    graph grafo_valutazioni; 

    load_graph_from_file(&grafo_valutazioni, arrayNodes, dim);
    stampa(&grafo_valutazioni, arrayNodes);



    //punto 2a
    printf("\n-------------");
    char* account = string_input("\nInserire account: ", 20);
    printf("\nValutazione media di %s: %f", account, media_utente(account, grafo_valutazioni, arrayNodes, dim));


    //punto 3
    printf("\n-------------");
    int id_prod = (int) int_input("\nInserire id prodotto: ", 1, dim);
    printf("\nLa media delle valutazioni del prodotto e' %f", media_prod(arrayNodes[id_prod-1].descrizione, grafo_valutazioni, arrayNodes, dim));

    //libero la memoria
    free(arrayNodes);

    printf("\n\n");
    return 0;
}

long int int_input(char* msg, int minimum_value, int max_value){
    long int input;
    char buffer[100];

    printf("%s", msg);
    while(fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("\nErrore nella lettura dell'input. Riprova: ");
    }

    char *endptr;
    input = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || input < minimum_value || input > max_value) {
        printf("\nInput non valido\n.");
        return int_input(msg, minimum_value, max_value);
    }

    return input;
}

char* string_input(char* msg, int max_length){
    printf("%s", msg); 
    char* buffer = malloc(max_length * sizeof(char));
    if(!buffer){printf("\n\nMalloc failed"); exit(EXIT_FAILURE);} 

    while(fgets(buffer, max_length, stdin) == NULL){
        printf("\nInput fallito\n");
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    return buffer;
}

/**
 * @brief Funzione che genera e restituisce un vettore dinamico dei nodi contenente la descrizione
 * 
 * @param n 
 * @return tipo_inf* 
 */
tipo_inf* creaNodi(int* n){
    char* filename = "nodi.txt";
    char* mode = "r";

    FILE* fp_nodi = fopen(filename, mode); 
    if(fp_nodi == NULL){printf("\nERRORE"); exit(EXIT_FAILURE);}

    //la prima riga indica il numero totale di nodi
    char buffer[100];
    int totNodi = 0; 
    if(fgets(buffer, sizeof buffer, fp_nodi) == NULL){printf("\nERRORE NELLA LETTURA"); exit(EXIT_FAILURE);}
    buffer[strcspn(buffer, "\r\n")] = '\0';
    if(sscanf(buffer, "%d", &totNodi) != 1){printf("\nRIGA NON VALIDA"); exit(EXIT_FAILURE);}

    //costruisco l'array dinamico
    tipo_inf* arrayNodes = malloc(totNodi * sizeof(tipo_inf));
    if(!arrayNodes){printf("\nMALLOC FAILED"); exit(EXIT_FAILURE);}


    //dalla seconda riga leggo solo i nodi
    for(int i=1; i<=totNodi; i++){
        tipo_inf tmp;
        int index = i-1; 
        //printf("\nLETTURA NODO %d", i);

        //per ogni nodo leggo 2 righe
        //prima riga nodo
        if(fgets(buffer, sizeof buffer, fp_nodi) == NULL){printf("\nERRORE NELLA LETTURA"); exit(EXIT_FAILURE);}
        buffer[strcspn(buffer, "\r\n")] = '\0';
        strcpy(tmp.descrizione, buffer); 

        //seconda riga nodo
        if(fgets(buffer, sizeof buffer, fp_nodi) == NULL){printf("\nERRORE NELLA LETTURA"); exit(EXIT_FAILURE);}
        buffer[strcspn(buffer, "\r\n")] = '\0';
        tmp.tipo = buffer[0]; 

        //una volta letto il nodo, lo inserisco nell'array
        //printf("\nNodo letto: %s %c", tmp.descrizione, tmp.tipo);
        arrayNodes[index] = tmp;

    }

    if(fclose(fp_nodi) == EOF){printf("\nERRORE"); exit(EXIT_FAILURE);}

    //ritorno i valori
    *n = totNodi;
    return arrayNodes;

}

/**
 * @brief Funzione per il caricamento di un grafo dal file valutazioni.txt
 * 
 * @param grafo_valutazioni 
 * @param ArrayNodes 
 * @param dim 
 */
void load_graph_from_file(graph* grafo_valutazioni, tipo_inf* ArrayNodes, int dim){
    char* filename = "valutazioni.txt";
    char* mode = "r";

    FILE* fp_valutazioni = fopen(filename, mode); 
    if(fp_valutazioni == NULL){printf("\nERRORE"); exit(EXIT_FAILURE);}

    //creo il nuovo grafo 
    graph grafo_tmp = new_graph(dim);

    //ogni nodo è su una riga 
    char buffer[200]; 

    //ogni riga è una valutazione diversa
    while(fgets(buffer, sizeof buffer, fp_valutazioni) != NULL){
        char descrizione_utente[20];
        char descrizione_prodotto[20];
        int stelle = 0;

        buffer[strcspn(buffer, "\r\n")] = '\0';
        if(sscanf(buffer, "%s %s %d", descrizione_utente, descrizione_prodotto, &stelle) != 3){printf("\nRIGA NON VALIDA"); exit(EXIT_FAILURE);}

        //devo aggiungere un arco che va da utente --> prodotto, con peso = stelle (prodotto diventa vicino di utente)
        int id_utente = trovaUtenteProdotto(ArrayNodes, dim, descrizione_utente);
        int id_prodotto = trovaUtenteProdotto(ArrayNodes, dim, descrizione_prodotto); 

        if(id_prodotto == -1 || id_utente == -1)
            {printf("\nDATI NON VALIDI"); exit(EXIT_FAILURE);}

        //printf("\nAGGIUNGO L'ARCO %s [%d] --> %s [%d], di peso %d", descrizione_utente, id_utente, descrizione_prodotto, id_prodotto, stelle);
        add_arc(&grafo_tmp, id_utente+1, id_prodotto+1, stelle);
    }

    if(fclose(fp_valutazioni) == EOF){printf("\nERRORE"); exit(EXIT_FAILURE);}
    *grafo_valutazioni = grafo_tmp; 

}

/**
 * @brief Procedura di stampa delle valutazioni
 * 
 * @param grafo_valutazioni 
 * @param arrayNodes 
 */
void stampa(graph* grafo_valutazioni, tipo_inf* arrayNodes){
    for(int i=0; i<get_dim(*grafo_valutazioni); i++){
        int id_nodo = i+1; //1-based
        //printf("\nNodo di partenza: %s", arrayNodes[i].descrizione);

        //scorro la lista del nodo con id id_nodo
        adj_list cursor = get_adjlist(*grafo_valutazioni, id_nodo);
        while(cursor != NULL){
            printf("\n%s valuta %lf %s", arrayNodes[i].descrizione, cursor->weight, arrayNodes[cursor->node].descrizione);

            cursor = cursor->next;
        }


    }

}

/**
 * @brief Funzione che calcola la media delle valutazioni dell'utente con la descrizione fornita per parametro
 * 
 */
float media_utente(char* account, graph g, tipo_inf* arrayNodes, int dim){
    //cerco l'id dell'utente
    int pos = trovaUtenteProdotto(arrayNodes, dim, account);
    if(pos == -1){printf("\nDATI NON VALIDI"); exit(EXIT_FAILURE);}
    //acquisisco la lista di adiacenza del nodo, per trovare tutti i prodotti che ha valutato
    adj_list cursor = get_adjlist(g, pos+1);
    float somma = 0.0F; 
    int totValutazioni = 0;

    while(cursor != NULL){
        somma += cursor->weight;
        totValutazioni++;
        cursor = cursor->next;
    }

    return somma / (float) totValutazioni;
}

/**
 * @brief Funzione che calcola la valutazione media del prodotto
 * 
 */
float media_prod(char* prod, graph g, tipo_inf* arrayNodes, int dim){
    //scorrere tutte le liste di adiacenza per scoprire la valutazione del prodotto
    int id_prodotto = trovaUtenteProdotto(arrayNodes, dim, prod);
    if(id_prodotto == -1) {printf("\nDATI NON VALIDI"); exit(EXIT_FAILURE);}

    float tot_valutazioni = 0.0F;
    int n_valutazioni = 0;

    for(int i=0; i<get_dim(g); i++){
        //di ogni nodo UTENTE prendo la sua lista di adiacenza
        int index = i+1;
        if(arrayNodes[i].tipo == 'U'){
            adj_list cursor = get_adjlist(g, index);
            while(cursor != NULL){
                //se l'utente index ha recensito il prodotto prod, sommo la recensione alla valutazione totale
                if(compare(arrayNodes[cursor->node], arrayNodes[id_prodotto]) == 0){
                    tot_valutazioni += cursor->weight;
                    n_valutazioni++;
                }

                //else --> non considero altri prodotti
                cursor = cursor->next;
            }

        } 

        //else --> non faccio, esistono solo gli archi U->P

    }

    return tot_valutazioni/(float) n_valutazioni;

}

