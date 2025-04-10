#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ROWS 18391
#define LINE 200

char header[150];

//Função responsável por criar Arquivos
void createArq(PROCESSO *arr, char *arq_name){
    FILE *arq = fopen(arq_name, "w");

    if (arq == NULL) {
        printf("Erro ao criar arquivo\n");
        exit(-1);
    }

    fprintf(arq, "%s\n", header);

    for(int i = 0;i<ROWS;i++){
        fprintf(arq, "%d,%s,%s,%s,%s,%d\n", arr[i].id, arr[i].num, arr[i].dt_ajm, arr[i].id_cls, arr[i].id_ass, arr[i].ano_elc);
    }

    fclose(arq);
}

//Identifica e armazena cada campo considerando suas peculiaridades
int extractField(char *line, int start, char *arq_data){
    int i = start;
    int j = 0;
    int quote = 0;
    
    //Identifica os Multivalorados (dentro das aspas)
    if(line[i] == '"'){
        quote = 1;
    }
    
    while (line[i] != '\0') {
        if (quote){
            if (line[i] == '"' && line[i+1] == ','){ //Verifica se é o fim (multivalorado)
                arq_data[j] = '\"';
                arq_data[j+1] = '\0';
                return i + 1;// Retorna a posição onde a leitura parou (depois das aspas)
            }
            arq_data[j++] = line[i++];
        } else {
            if (line[i] == ',' || line[i] == '\0' || line[i] == '\n'){ //Verifica se é o fim
                arq_data[j] = '\0';
                return i; // Retorna a posição onde a leitura parou (multivalorados)
            }
            arq_data[j++] = line[i++];
        }
    }
    
    arq_data[j] = '\0';
    return i;
}

//Função que faz a leitura do arquivo
PROCESSO *readFile(char *arq_csv) {
    FILE *arq = fopen(arq_csv, "r");
    
    if (arq == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(-1);
    }

    PROCESSO *data = (PROCESSO *)malloc(sizeof(PROCESSO) * ROWS);

    if (data == NULL) {
        printf("Falha ao alocar memoria.\n");

        fclose(arq);
        
        exit(-1);
    }

    //Leitura do Cabeçalho
    fscanf(arq, "%[^\n]", header);

    fgetc(arq); //Pula uma linha

    char temp_row[LINE];
    char field[LINE]; //Variável especial para valores de tipo numérico

    //Extraindo os dados das linhas
    int i = 0;
    while (fgets(temp_row, sizeof(temp_row), arq) != NULL){
        int pos = 0;
        
        pos = extractField(temp_row, pos, field);
        data[i].id = atoi(field); //Passando ID como inteiro
        pos++;
        
        pos = extractField(temp_row, pos, data[i].num);
        pos++;
        
        pos = extractField(temp_row, pos, data[i].dt_ajm);
        pos++;

        pos = extractField(temp_row, pos, data[i].id_cls);
        pos++;
        
        pos = extractField(temp_row, pos, data[i].id_ass);
        pos++;
        
        pos = extractField(temp_row, pos, field);
        data[i].ano_elc = atoi(field); //Passando Ano de Eleição como inteiro
        
        i++;
    }

    fclose(arq);
    
    return data;
}

//Função que compara um array ordenado com o conjunto de dados desordenados
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
 
//Chamada para função que ordena por ID
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

//Ordenação por data
void dateOrdering(char *arq_csv){
    PROCESSO *data = readFile(arq_csv);
    
    PROCESSO *aux = (PROCESSO *)malloc(sizeof(PROCESSO));

    int i = 0, j = 0;
    
    for(i = 0; i < ROWS - 1; i++){
        for(j = i + 1; j < ROWS; j++){
            if(strcmp(data[i].dt_ajm, data[j].dt_ajm) < 0){ //Comparando as strings para identificar a maior
                
                *aux =  data[i];
                
                data[i] = data[j];
                
                data[j] = *aux;

            }
        }
    }

    createArq(data, "ordenadoPorData.csv");
}

//Remoção de chaves e aspas de uma string
void rmCurlyBracesQuotes(char *class){
    char tempStr[50];

    int i, j;
    i = j = 0;
    while(class[i] != '\0'){
        if(class[i] != '{' && class[i] != '}' && class[i] != '\"'){
            tempStr[j++] = class[i]; 
        }
        i++;
    }
    tempStr[j] = '\0';

    strcpy(class, tempStr);
}

//Identificador de classe do processo (multivalorado ou não)
PROCESSO *FetchId(char *arq_csv, char *idClass, int *qtd){
    PROCESSO *process;

    process = readFile(arq_csv);

    int i;
    
    for(i = 0;  i < ROWS ; i++){
        rmCurlyBracesQuotes(process[i].id_cls);

        if(strchr(process[i].id_cls, ',')){
            char id[100];
            
            strcpy(id,process[i].id_cls);

            char *token = strtok(id,","); //Armazenando valores até a virgula, se multivalorado

            while(token != NULL){
                if((strcmp(token, idClass)) == 0){
                    *qtd+=1;
                    break;
                }

                token = strtok(NULL,",");
            }
        }else{
            if(strcmp(process[i].id_cls, idClass) == 0){
                *qtd+=1;
            }
        }  
    }
}

//Função chamada para apresentar e identificar classes atreladas a um processo
void fetchByIdClass(char *arq_csv, char *idClass){
    int qtd = 0;

    PROCESSO *process = FetchId(arq_csv, idClass, &qtd);

    printf("o id %s tem %d processos vinculados a ele", idClass, qtd);
}

//Função que identifica e lista todos os assunstos existentes nos processos
void showSubjects(SUBJECT *subjects){
    SUBJECT *arr = (SUBJECT*) malloc(sizeof(SUBJECT) * 100);

    int i, j;
    int count = 0;

    for(i = 0; i < ROWS - 1; i++){
        int alreadyExist = 0;

        for(j = 0; j < count; j++){
            if(strcmp(subjects[i].subj, arr[j].subj) == 0){
                alreadyExist = 1;
                break;
            }
        }

        if(!alreadyExist){
            strcpy(arr[count].subj, subjects[i].subj);
            count++;
        }
    }

    printf("Existem %d id_assunto diferentes.\n", count);
    printf("Valores encontrados: ");
    for(i = 0; i < count; i++){
        printf("[%s] ", arr[i].subj);
    }
    printf("\n");
}

//Faz a leitura e armazena os valores de maneira propricia para a função acima
void checkSubject(char *arq_csv){
    PROCESSO *process = readFile(arq_csv);
    SUBJECT *subjects = (SUBJECT *) malloc(sizeof(SUBJECT) * (ROWS + 200));
    
    int i, j = 0;

    for(i = 0; i < ROWS; i++){
        rmCurlyBracesQuotes(process[i].id_ass);

        if(strchr(process[i].id_ass, ',')){  
            char sub[50];

            strcpy(sub, process[i].id_ass);

            char *token = strtok(sub, ",");

            while(token != NULL){
                strcpy(subjects[j].subj, token);

                token = strtok(NULL, ",");

                j++;
            }
        }else{
            strcpy(subjects[j].subj, process[i].id_ass);
            j++;
        }
    }

    showSubjects(subjects);
}

//Identifica todos os valores multivalorados em assuntos, achando aspas
void MoreThanSubjects(char *arq_csv){
    PROCESSO *process = readFile(arq_csv);

    int i;
    for(i=0;i<ROWS;i++){
        if(process[i].id_ass[0] == '\"'){
            printf("%d,%s,%s,%s,%s,%d\n", process[i].id, process[i].num, process[i].dt_ajm, process[i].id_cls, process[i].id_ass, process[i].ano_elc);
        }
    }
}

//Calcula a diferença do tempo de um processo de sua data inicial até o dia de hoje
void processTime(char *arq_csv){
    PROCESSO *process = readFile(arq_csv);

    PROCESSO *aux_process = (PROCESSO *)malloc(sizeof(PROCESSO));

    int id_aux;

    printf("Digite o ID do processo:\n");

    scanf("%d", &id_aux);

    //Procurando processo por ID
    for(int i=0;i<ROWS;i++){
        if(process[i].id == id_aux){
            *aux_process = process[i];
            break;
        }
    }

    //Coletando dados de tempo do dispositivo
    time_t tnow;
    
    struct tm *dataNow, dataProcess;

    time(&tnow);

    dataNow = localtime(&tnow);

    //Coletando dados de tempo do processo
    sscanf(aux_process->dt_ajm,"%d-%d-%d %d:%d:%d", &dataProcess.tm_year, &dataProcess.tm_mon, &dataProcess.tm_mday, &dataProcess.tm_hour, &dataProcess.tm_min, &dataProcess.tm_sec);
    
    dataProcess.tm_year -= 1900; //Registro de dados a partir de 1900
    
    dataProcess.tm_mon  -= 1; //Registro de mês é feito de 0 (janeiro) até 11 (dezembro)

    time_t processSeconds = mktime(&dataProcess);

    time_t nowSeconds = mktime(dataNow);

    double diference = difftime(nowSeconds, processSeconds); //Calcula a diferença de tempo, considerando bissextos

    time_t diffTime = (time_t) diference;

    struct tm *duration = gmtime(&diffTime);

    duration->tm_year = duration->tm_year - 70; //Ajuste de tempo em anos

    printf("O tempo de tramitacao e de:\nAnos:%d\nMeses:%d\nDias:%d\nHoras:%02d\nMinutos:%02d\nSegundos:%02d", duration->tm_year, duration->tm_mon, duration->tm_mday, duration->tm_hour, duration->tm_min, duration->tm_sec );
}  
