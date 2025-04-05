#ifndef FUNC_H
#define FUNC_H

typedef struct{
    int id;
    char num[30];
    char dt_ajm [30];
    char id_cls[30];
    char id_ass[30];
    int ano_elc;
}PROCESSO;

PROCESSO *readFile(char *arq_csv);

void quickSort(int *arr, int low, int high);

void idOrdering(char *arq_csv);

void dateOrdering(char *arq_csv);

#endif 
