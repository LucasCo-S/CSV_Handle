#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 18391

char header[100];

void createArq(PROCESSO *arr, char *arq_name){
    FILE *arq = fopen(arq_name, "w");

    if (arq == NULL) {
        printf("Erro ao criar arquivo\n");
        return;
    }

    fprintf(arq, "%s\n", header);

    for(int i = 0;i<ROWS;i++){
        fprintf(arq, "%d,\"%s\",%s,%s,%s,%d\n", arr[i].id, arr[i].num, arr[i].dt_ajm, arr[i].id_cls, arr[i].id_ass, arr[i].ano_elc);
    }

    fclose(arq);
}

PROCESSO *readFile(char *arq_csv) {
    FILE *arq = fopen(arq_csv, "r");

    PROCESSO *data = (PROCESSO *)malloc(sizeof(PROCESSO) * ROWS);

    if (data == NULL) {
        printf("Falha ao alocar memoria.\n");
        exit(-1);
    }

    fscanf(arq, "%[^\n]", header);

    fgetc(arq);

    char aux_row[200];

    int i = 0;

    int fields;

    while (fgets(aux_row, sizeof(aux_row), arq) != NULL) {
        //printf("Linha: %s", aux_row);

        /* Tenta extrair valores da string lida usando `sscanf`, com dois formatos diferentes.
        O primeiro formato busca campos delimitados por vírgulas e chaves `{}`.
        O segundo formato usa aspas `"` para alguns campos.*/

        fields = sscanf(aux_row, "%d,\"%[^\"]\",%[^,],%[^,],%[^,],%d", 
                    &data[i].id, data[i].num, data[i].dt_ajm, data[i].id_cls, data[i].id_ass, &data[i].ano_elc);

        if(fields != 6){
            fields = sscanf(aux_row, "%d,\"%[^\"]\",%[^,],\"%[^\"]\",%[^,],%d", 
                    &data[i].id, data[i].num, data[i].dt_ajm, data[i].id_cls, data[i].id_ass, &data[i].ano_elc);
        }

        if(fields != 6){
            fields = sscanf(aux_row, "%d,\"%[^\"]\",%[^,],%[^,],%[^\"],%d", 
                    &data[i].id, data[i].num, data[i].dt_ajm, data[i].id_cls, data[i].id_ass, &data[i].ano_elc);
        }

        if(fields != 6){
            fields = sscanf(aux_row, "%d,\"%[^\"]\",%[^,],\"%[^\"]\",\"%[^\"]\",%d", 
                    &data[i].id, data[i].num, data[i].dt_ajm, data[i].id_cls, data[i].id_ass, &data[i].ano_elc);
        }

        if(i >= ROWS){
            break;
        }

        i++;
    }

    fclose(arq);
    
    return data;
}

void OrderingById(PROCESSO *data, int *arr){
    int i,j;

    PROCESSO *aux = (PROCESSO*) malloc(sizeof(PROCESSO));

    for(i = 0; i < ROWS; i++){
        for(j = 0; j < ROWS; j++){
            if(arr[i] == data[j].id){
                *aux = data[i];
                data[i] = data[j];
                data[j] = *aux;
            }
        }
    }

    createArq(data, "ordenadoPorId.csv");
}
 

void idOrdering(char *arq_csv){
    int *arr = (int*) malloc(sizeof(int) * ROWS);
    
    PROCESSO *data = readFile(arq_csv);

    int i;

    for(i = 0; i < ROWS; i++){
        arr[i] = data[i].id;
    }

   quickSort(arr, 0, ROWS - 1);

   OrderingById(data, arr);
}


//Troca de valores quickSort
void swap(int *a, int *b){
    int  aux = *b;
    *b = *a;
    *a = aux;
}

//Coleta 
int partition(int *arr, int low, int high){
    int pivot = arr[(low + high) / 2];

    int i = low;

    int j = high;

    while(i < j){
        
        while(arr[i] < pivot){
            i++;
        }

        while(arr[j] > pivot){
            j--;
        }

        if(i < j){
            swap(&arr[i], &arr[j]);
        }
    }

    return j;
}


void quickSort(int *arr, int low, int high){
    if(low < high){
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi -1);

        quickSort(arr, pi + 1, high);
    }
}


void dateOrdering(char *arq_csv){
    PROCESSO *data = readFile(arq_csv);
    
    PROCESSO *aux = (PROCESSO *)malloc(sizeof(PROCESSO));

    int i = 0, j = 0;
    
    for(i = 0; i < ROWS - 1; i++){
        for(j = i + 1; j < ROWS; j++){
            if(strcmp(data[i].dt_ajm, data[j].dt_ajm) < 0){
                
                *aux =  data[i];
                
                data[i] = data[j];
                
                data[j] = *aux;

            }
        }
    }

    createArq(data, "ordenadoPorData.csv");
}