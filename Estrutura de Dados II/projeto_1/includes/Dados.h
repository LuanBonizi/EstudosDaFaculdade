#ifndef DADOS_H
#define DADOS_H

#include <stdio.h>
#include <stdlib.h>
#include "IdxPrimario.h"
#include "IdxSecundario.h"

//Struct que representa o dado do registro de filmes do arquivo dados.dat
typedef struct filme{
    char codigo[6];//Chave primária do filme
    char titulo_portugues[50];
    char titulo_original[50];
    char diretor[40];
    char ano[5];
    char pais[30];
    int nota;
}Filme;

//Escopos das funções relacionadas aos filmes do arquivo de dados
void monta_chave(char *sobrenome, char *ano, char *codigo);

int insere_filme(FILE *filmes, Filme filme, IDXPrimario **head, IDXPrimario *ind, IDXSecundario **headSecundaria, IDXSecundario *indSecundario, int qtdFilmes);

void busca_filme(FILE *movies, int RRN);

void lista_filmes(FILE *movies, int qtdFilmes);

int remove_filme(FILE *movies, int RRN);

void atualiza_nota(FILE *movies, int RRN, Filme filme);

void pausa();

#endif