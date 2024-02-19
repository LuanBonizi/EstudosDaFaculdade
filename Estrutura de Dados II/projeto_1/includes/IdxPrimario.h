#ifndef IDXPRIMARIO_H
#define IDXPRIMARIO_H

#include <stdio.h>
#include <stdlib.h>

//Struct utilizada para representar a estrutura de índice primário
typedef struct idxPrimario{
    char chave[5];//chave do filme
    int RRN;//RRN do filme
    struct idxPrimario *prox;
}IDXPrimario;

//Escopos das funções relacionadas aos índices primários
IDXPrimario *novo_indice(char *chave, int RRN);

void insere_indicePrimario(IDXPrimario **head, IDXPrimario *indice);

void remove_indicePrimario(IDXPrimario **head, char *chave);

int busca_RRN(IDXPrimario *head, char *chave);

int busca_maior_RRN(IDXPrimario *head);

void leitura_indices(IDXPrimario **head, FILE *idxPrimario);

void imprime_indices(IDXPrimario *head, FILE *idxPrimario);

void atualiza_arquivo(FILE *movies, IDXPrimario **head, int qtdFilmes);

void libera_memoria_primaria(IDXPrimario **head);

#endif