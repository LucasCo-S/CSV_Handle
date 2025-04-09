#include"func.h"
#include<stdlib.h>
#include<stdio.h>

int main(){
    int response;
    do{
        system("cls");

        char idClass[50]; 

        printf("ESCOLHA UMA MANEIRA DE VISUALIZACAO\n");

        printf("1 - ID\n2 - DATA\n3 - QUANTOS PROCESSO ASSOCIADOS A UM ID_CLASSE\n4 - CONTAR ID_ASSUNTO EM PROCESSOS\n5 - LISTAR PROCESSOS DE UM ASSUNTO\n6 - DIAS DE TRAMITACAO\n0 - SAIR\n");

        scanf("%d", &response);

        switch (response){
        case 1: idOrdering("processo.csv");
            break;
        
        case 2: dateOrdering("processo.csv");
            break;

        case 3: printf("DIGITE O ID_CLASSE PARA SER PESQUISADO\n");
                scanf("%s", idClass);
                fetchByIdClass("processo.csv", idClass);
            break;
        
        case 4: checkSubject("processo.csv");
            break;

        case 5: MoreThanSubjects("processo.csv");
            break;

        case 6: processTime("processo.csv");
            break;
        
        default: printf("ESCOLHA UMA ALTERNATIVA DISPONIVEL");
            break;
        }

        printf("\nDESEJA FAZER MAIS ALGUMA OPERACAO?[0=N,1=S]\n");

        scanf("%d", &response);
    }while(response != 0);
}