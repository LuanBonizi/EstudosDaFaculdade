#include "../includes/Dados.h"
#include "../includes/IdxPrimario.h"
#include "../includes/IdxSecundario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



//Funcão que monta a chave primária do filme
void monta_chave(char *sobrenome, char *ano, char *codigo){
    //Monta a chave primária(código) baseado na especificação
    codigo[0] = toupper(sobrenome[0]);
    codigo[1] = toupper(sobrenome[1]);
    codigo[2] = toupper(sobrenome[2]);
    codigo[3] = ano[2];
    codigo[4] = ano[3];
}

//Funções relacionadas ao arquivo de dados

int insere_filme(FILE *movies, Filme filme, IDXPrimario **head, IDXPrimario *ind, IDXSecundario **headSecundario, IDXSecundario *indSecundario, int qtdFilmes){

    int size;
    char stringFormatada[192], *nome, *sobrenome;

    fseek(movies,0,SEEK_END);//Seek End para inserir o filme no fim do arquivo

    //leitura das informações
    printf("\nDigite o titulo em português do filme: ");
    scanf(" %[^\n]s", filme.titulo_portugues);
    printf("\nDigite o titulo original do filme (digite 'idem' caso este seja igual ao título em português): ");
    scanf(" %[^\n]s", filme.titulo_original);
    printf("\nDigite o nome completo do diretor: ");
    scanf(" %[^\n]s", filme.diretor);
    printf("\nDigite o ano de lançamento do filme: ");
    scanf(" %s", filme.ano);
    printf("\nDigite o país em que foi feito o filme: ");
    scanf(" %[^\n]s", filme.pais);

    //Lê a nota e verifica se é válida
    do{
        printf("\nDigite a nota do filme (digite uma nota de 0 a 9): ");
        scanf(" %d", &filme.nota);
    }while(filme.nota < 0 || filme.nota > 9);

    //Quebra do nome do diretor em duas variáveis(nome e sobrenome)
    nome = strtok(filme.diretor," ");
    sobrenome = strtok(NULL," ");

    //Montando a chave
    monta_chave(sobrenome,filme.ano,filme.codigo);

    //Formata a string baseado nas informações concebidas e no que foi especificado
    sprintf(stringFormatada,"%s@%s@%s@%s, %s@%s@%s@%d@", filme.codigo,filme.titulo_portugues,filme.titulo_original,sobrenome,nome,filme.ano,filme.pais,filme.nota);
    size = strlen(stringFormatada);

    //Calcula o tamanho da string formatada e preenche o restante com #
    for(int i = size; i < 191; i++)
        stringFormatada[i] = '#';
    strcat(stringFormatada,"#");

    //Monta os elemento das listas e os inserem nas respectivas lista de índices logo em seguida
    //Nesse caso, o RRN do filme cadastrado é justamente a quantidade de filmes, assim, o novo filme
    //possuirá o RRN como sendo a última posição no arquivo de dados
    ind = novo_indice(filme.codigo,qtdFilmes);
    indSecundario = novo_indiceSecundario(filme.titulo_portugues,filme.codigo);

    insere_indicePrimario(head,ind);
    insere_indiceSecundario(headSecundario,indSecundario);

    //Grava a string no arquivo de dados
    fputs(stringFormatada,movies);
    //Limpa o buffer do arquivo
    fflush(movies);

    return 0;
}

void busca_filme(FILE *movies, int RRN){
    char nome[20], sobrenome[20], pausa;
    Filme filme;

    //realiza um fseek baseado no RRN fornecido
    fseek(movies, RRN * 192, SEEK_SET);
    //Faz a leitura dos dados do registro
    fscanf(movies,"%[^@]@%[^@]@%[^@]@%[^,], %[^@]@%[^@]@%[^@]@%d@", filme.codigo,filme.titulo_portugues,filme.titulo_original, sobrenome, nome, filme.ano, filme.pais, &filme.nota);
    //Veifica se não é um filme removido, senão, imprime na tela as informações
    if(filme.codigo[0] != '*' && filme.codigo[1] != '|'){
        printf("\n\n----------Informações do filme---------\n\n");
        printf("Chave: %s\n",filme.codigo);
        printf("Título em português: %s\n",filme.titulo_portugues);
        printf("Título original: %s\n",filme.titulo_original);
        printf("Nome do autor: %s %s\n",nome, sobrenome);
        printf("Ano: %s\n", filme.ano);
        printf("País: %s\n",filme.pais);
        printf("Nota: %d\n\n",filme.nota);
    }

}

void lista_filmes(FILE *movies, int qtdFilmes){

    Filme filme;
    char stringTotal[192], nome[20], sobrenome[20];
    //Movimenta o ponteiro pro começo do arquivo
    fseek(movies,0,SEEK_SET);
    printf("\n\n================ Lista de todos os filmes ================\n\n");

    //Realiza um fseek para cada registro no arquivo de dados, fazendo a leitura
    // e imprimindo na tela caso o filme não seja removido
   for(int i =0; i < qtdFilmes; i++){
        fseek(movies, i * 192, SEEK_SET);
        fscanf(movies,"%[^@]@%[^@]@%[^@]@%[^,], %[^@]@%[^@]@%[^@]@%d@", filme.codigo,filme.titulo_portugues,filme.titulo_original, sobrenome, nome, filme.ano, filme.pais, &filme.nota);

        if(filme.codigo[0] != '*' && filme.codigo[1] != '|'){
            printf("\n\n----------Informações do filme---------\n\n");
            printf("Chave: %s\n",filme.codigo);
            printf("Título em português: %s\n",filme.titulo_portugues);
            printf("Título original: %s\n",filme.titulo_original);
            printf("Nome do autor: %s %s\n",nome, sobrenome);
            printf("Ano: %s\n", filme.ano);
            printf("País: %s\n",filme.pais);
            printf("Nota: %d\n\n",filme.nota);
        }
    }
}

int remove_filme(FILE *movies, int RRN){
    //Busca o registro e o marca com *|
    fseek(movies, RRN * 192, SEEK_SET);
    fprintf(movies,"*|");

    fflush(movies);

    return 0;
}

void atualiza_nota(FILE *movies, int RRN, Filme filme){
    int novaNota;
    char nome[20],sobrenome[20];
    //Busca o filme, faz uma leitura para mover o ponteiro, e logo em seguida lê a nova nota e 
    //atualiza no arquivo
    fseek(movies,192 * RRN,SEEK_SET);

    fscanf(movies,"%[^@]@%[^@]@%[^@]@%[^,], %[^@]@%[^@]@%[^@]@", filme.codigo,filme.titulo_portugues,filme.titulo_original, sobrenome, nome, filme.ano, filme.pais);
    //Lê a nota e verifica se é válida
    do{
        printf("Digite a nova nota do filme (digite uma nota de 0 a 9): ");
        scanf(" %d", &novaNota);
    }while(novaNota < 0 || novaNota > 9);
    fprintf(movies,"%d",novaNota);
    fflush(movies);
}

//Função que pausa o programa e limpa a tela
void pausa(){
    char c;
    printf("Pressione qualquer tecla para continuar...");
    scanf(" %c", &c);
    system("clear");
}