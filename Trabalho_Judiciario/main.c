#include"func.h"
#include<stdio.h>


int main(){
    int response;

    printf("Escolhe a maneira de ordenacao\n");

    printf("1 - ID\n2 - DATA\n");

    scanf("%d", &response);

    if(response == 1){
        idOrdering("processo.csv");
    }else if(response == 2){
        dateOrdering("processo.csv");
    }else{
        printf("Escolha uma alternativa disponivel");
    }
}