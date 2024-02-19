#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/Dados.h"
#include "../includes/IdxPrimario.h"

//Funções relacionadas à lista dinamica dos índices primários que fica na memória principal

//Função que cria um novo elemento para inserir na lista de índices primários
IDXPrimario *novo_indice(char *chave, int RRN){
    IDXPrimario *aux;
    aux = malloc(sizeof(IDXPrimario));//Aloca memória para o elemento
    strcpy(aux->chave,chave);//Copia a chave e o RRN no elemento
    aux->RRN = RRN;
    aux->prox = NULL;
    return aux;//Retorna o elemento
}

//Função que insere um novo elemento na lista de índices primários
void insere_indicePrimario(IDXPrimario **head, IDXPrimario *indice){
    IDXPrimario *aux, *antes;

    if(*head == NULL){//caso de lista vazia, o elemento se torna a cabeça
        *head = indice;
        return;
    }
    
    aux = antes = *head;

    //Procura a melhor posição para inserir o elemento, matendo a lista ordenada
    while(aux->prox && strcmp(indice->chave,aux->chave) > 0){
        antes = aux;
        aux = aux->prox;
    }

    if(aux == *head){//caso em que a inserção pode envolver a cabeça da lista
        if(strcmp(indice->chave,aux->chave) > 0){//Elemento a ser inserido é 'maior' que a cabeça
            //Elemento é inserido na frente da cabeça
            indice->prox = aux->prox;
            (*head)->prox = indice;
            return;
        }else if(strcmp(indice->chave,aux->chave) < 0){//Elemento é menor que a cabeça
            //Elemento se torna a nova cabeça
            indice->prox = aux;
            (*head) = indice;
            return;
        }
    }
    if(strcmp(indice->chave,aux->chave) > 0){//Elemento é maior que o elemento apontado pelo ponteiro auxiliar
        //Elemento é inserido após o auxiliar
        aux->prox = indice;
        return;
    }else if(strcmp(indice->chave,aux->chave) < 0){//Elemento é menor que o elemento apontado pelo ponteiro auxiliar
        //Elemento é inserido antes do ponteiro auxiliar
        antes->prox = indice;
        indice->prox = aux;
        return;
    }

    return;
}

//Função que remove um elemento da lista baseado na chave fornecida
void remove_indicePrimario(IDXPrimario **head, char *chave){
    IDXPrimario *aux, *antes;
    if(*head == NULL)//Caso de lista vazia, apenas retorna a função
        return;
    aux = antes = *head;

    //Busca o elemento
    while(aux && strcmp(aux->chave,chave) != 0){
        antes = aux;
        aux = aux->prox;
    }

    if(aux == *head){//Se a cabeça precisar ser removida, o elemento após ela se torna a nova cabeça
        *head = aux->prox;
        free(aux);//Libera a memória
        return;
    }else if(aux == NULL){//Caso em que o elemento não foi encontrado
        printf("Esse filme não está cadastrado!");
        return;
    }else{//Em ultimo caso, o elemento é removido, e seu anterior passa a ocupar seu lugar
        antes->prox = aux->prox;
        free(aux);
        return;
    }
}

//Função que busca o RRN do elemento cuja chave primária é fornecida
int busca_RRN(IDXPrimario *head, char *chave){
    IDXPrimario *aux = head;

    if(aux == NULL){//Caso de lista vazia
        printf("\nNão há filmes cadastrados!\n");
        return -1;
    }

    //procura o elemento cuja chave foi fornecida
    while(aux != NULL && strcmp(aux->chave,chave) != 0){
        aux = aux->prox;
    }

    if(aux == NULL)//elemento não está na lista
        return -1;
    
    return aux->RRN;//Se encontrar o elemento, retorna o elemento deste
}

    //Função que percorre a lista de índices primários, liberando a memória de cada elemento
    void libera_memoria_primaria(IDXPrimario **head){
        IDXPrimario *antes;
        if(*head == NULL)
            return;
        antes = *head;
        while(*head){
            (*head) = (*head)->prox;
            free(antes);
            antes = *head;
        }
        return;
    }
//Funções relacionadas ao arquivo de índices primários

//Função que monta a lista de índices primários baseado no arquivo iprimary.idx
void leitura_indices(IDXPrimario **head, FILE *idxPrimario){
    char chave[5];
    int RRN;
    IDXPrimario *aux;
    //fseek para pular o header com a flag
    fseek(idxPrimario,6,SEEK_SET);
    //a cada iteração, faz a leitura do registro e cria um novo elemento baseado na chave e RRN lidos
    //Logo em seguida, insere o novo elemento na lista
    while(fgetc(idxPrimario) != EOF){
        fscanf(idxPrimario,"%[^@]@%d#",chave, &RRN);
        aux = novo_indice(chave,RRN);
        insere_indicePrimario(head,aux);
    }
}

//Função que imprime a lista no arquivo de índices primários
void imprime_indices(IDXPrimario *head, FILE *idxPrimario){
    idxPrimario = freopen("iprimary.idx","w+",stdout);//reabre o arquivo para escrita e leitura

    fprintf(idxPrimario,"flag=1\n");

    if(head == NULL)//Lista vazia
        return; 

    while(head != NULL){
        //Percorre a lista, printando no arquivo cada elemento no formato chave@RRN##
        fprintf(idxPrimario,"%s@%d##",head->chave,head->RRN);
        head = head->prox;
    }
}

//Função que monta a lista de índices primários baseado no arquivo de dados
void atualiza_arquivo(FILE *movies, IDXPrimario **head, int qtdFilmes){
    char chave[6];
    IDXPrimario *aux;
    for(int i = 0; i<qtdFilmes; i++){
        //percorre todo o arquivo de dados, lendo a chave de cada registro, e, logo em
        //seguida, criando um novo elemento e o inserindo na lista de índices secundários
        //Nesse caso, o RRN é dado como i do for, que é a posição do registro
        fseek(movies, i * 192, SEEK_SET);
        fscanf(movies, "%[^@]@", chave);

        if(chave[0] != '*' && chave[1] != '|'){
            aux = novo_indice(chave, i);
            insere_indicePrimario(head, aux);
        }
    }
}