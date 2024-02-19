//Importando as bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/Dados.h"
#include "../includes/IdxSecundario.h"
#include "../includes/IdxPrimario.h"

//Variáveis para receber o valor da flag dos arquivos de índice e guardar quantos filmes há no arquivo de dados
int flag, qtdFilmes;

int main()
{   
    int op, RRN; //variáveis de opção do menu e RRN dos registros do arquivo de dados
    char chave[6], chaveSecundaria[50]; //Variáveis para guardar as chaves primárias e secundárias dos registros
    IDXPrimario *head = NULL, *ind; //Criando a cabeça da lista ligada de índices primários, além da variável auxiliar para inserir na lista
    IDXSecundario *headSecundario = NULL, *indSecundario;//cabeça da lista de índices secundários e variável auxiliar para inserir na lista
    Filme filme; //Variável que representa o registro de dilme
    FILE *movies, *idxPrimario, *idxSecundario;//Arquivos

    //Tenta abrir os arquivos para leitura e escrita
    movies = fopen("movies.dat", "r+");
    idxPrimario = fopen("iprimary.idx", "r+");
    idxSecundario = fopen("ititle.idx", "r+");

    //Verifica se arquivo de dados existe, senão, cria o arquivo
    if (movies == NULL)
        movies = fopen("movies.dat", "w+");
    //Busca a quantidade de registros do arquivo
    fseek(movies, 0, SEEK_END);
    qtdFilmes = ftell(movies) / 192;

    //verifica se o arquivo de índices primários existe, senão, cria o arquivo e já o atualiza baseado no arquivo de dados
    if (idxPrimario == NULL)
    {
        idxPrimario = fopen("iprimary.idx", "w+");
        fprintf(idxPrimario, "flag=0\n");
        atualiza_arquivo(movies, &head, qtdFilmes);
        fflush(idxPrimario);
    }
    else
    {//Se o arquivo existir, verifica o valor da flag
        fseek(idxPrimario, 5, SEEK_SET);
        fscanf(idxPrimario, "%d", &flag);
        if (flag == 0)
        {//Se flag =0, atualiza o arquivo baseado no arquivo de dados
            atualiza_arquivo(movies, &head, qtdFilmes);
        }
        else
        {//Senão, simplesmente lê o arquivo
            flag = 1;
            leitura_indices(&head, idxPrimario);
        }
    }

    //Verifica se o arquivo de índices secundários existe, senão realiza o mesmo processo que o arquivo de índices primários
    if (idxSecundario == NULL){
        idxSecundario = fopen("ititle.idx", "w+");
        fprintf(idxSecundario, "flag=0\n");
        atualiza_arquivoSecundario(movies, &headSecundario, qtdFilmes);
        fflush(idxSecundario);
    }else{//Verifica valor da flagg
        fseek(idxSecundario, 5, SEEK_SET);
        fscanf(idxSecundario, "%d", &flag);
        if (flag == 0)
        {//Se flag =0, atualiza o arquivo
            atualiza_arquivoSecundario(movies, &headSecundario, qtdFilmes);
            
        }
        else
        {//Senão, simplesmente lê o arquivo
            flag = 1;
            leitura_indicesSecundarios(&headSecundario, idxSecundario);
        }
    }

    //Menu do programa
    printf("------------Bem Vindo!------------\n\n");

    do
    {
        printf("\n\nEscolha uma opção: \n\n");
        printf("|1|- Inserir novo filme no arquivo\n");
        printf("|2|- Listar informações de um filme\n");
        printf("|3|- Listar todos os filmes\n");
        printf("|4|- Remover um filme\n");
        printf("|5|- Atualizar nota de um filme\n");
        printf("|6|- Sair\n\n");

        printf("Opção: ");

        scanf("%d", &op);

        switch (op)
        {

        case 1://Inserção de novo filme
            system("clear");
            printf("---------------Cadastro de novo filme---------------\n\n");
            printf("Não utilize acentos, por favor.\n\n");
            flag = insere_filme(movies, filme, &head, ind, &headSecundario, indSecundario, qtdFilmes);
            //Atualiza as flags dos arquivos de índice
            fseek(idxPrimario, 5, SEEK_SET);
            fputc('0', idxPrimario);
            fseek(idxSecundario, 5, SEEK_SET);
            fputc('0', idxSecundario);
            fflush(idxPrimario);
            fflush(idxSecundario);
            //Aumenta a quantidade de filmes
            qtdFilmes++;
            printf("\nFilme cadastrado com sucesso!\n");
            pausa();
        
            break;

        case 2://Listar infos de um filme
            system("clear");
            printf("---------------Listar informações de filmes---------------\n\n");

            printf("Escolha um método de busca:\n\n");
            printf("\n|1|- Procurar por chave primária\n");
            printf("|2|- Procurar por título em português\n");
            printf("Opção: ");
            scanf("%d",&op);

            if(op == 1){//Opção de busca por chave primária
                printf("Digite a chave do filme: ");
                scanf(" %s", chave);

                RRN = busca_RRN(head, chave);//busca o RRN do registro
                if (RRN == -1){//Caso em que o filme não existe
                    printf("Esse filme não está cadastrado!\n");
                    pausa();
                }
                else{//Se o filme existir, imprime as infos na tela
                    busca_filme(movies, RRN);
                    pausa();
                }
            }else if(op == 2){//Opção de busca por chave secundária
                printf("Digite a chave secundária: ");
                scanf(" %s", chaveSecundaria);
                //Imprime todos os filmes que possuem o mesmo título
                busca_filmes_por_chave_secundaria(movies, headSecundario, head, chaveSecundaria);
                pausa();
            }
            break;


        case 3:
            system("clear");
            //Lista as infos de todos os filmes não excluidos
            lista_filmes(movies, qtdFilmes);
            pausa();
            break;


        case 4://Remoção de um filme
            system("clear");
            printf("---------------Remoção de um filme---------------\n\n");
            printf("Digite a chave do filme: ");
            scanf(" %s", chave);

            RRN = busca_RRN(head, chave);
            if (RRN == -1){
                printf("Esse filme não está cadastrado!\n");
                pausa();
            }
            else
            {   //Remove o filme buscado
                flag = remove_filme(movies, RRN);
                remove_indicePrimario(&head, chave);
                remove_indiceSecundario(&headSecundario, chave);
                //Atualiza as flags de cada arquivo de índice
                fseek(idxPrimario, 5, SEEK_SET);
                fputc('0', idxPrimario);
                fseek(idxSecundario, 5, SEEK_SET);
                fputc('0', idxSecundario);
                fflush(idxPrimario);
                fflush(idxSecundario);
                printf("\nFilme removido com sucesso!\n");
                pausa();
            }
            break;


        case 5:
            system("clear");
            printf("---------------Atualizar nota de um filme---------------\n\n");
            printf("Digite a chave do filme: ");
            scanf(" %s", chave);

            RRN = busca_RRN(head, chave);

            if (RRN == -1){
                printf("Esse filme não está cadastrado!\n");
                pausa();
            }
            else
            {   //Se o filme existir, atualiza a nota dele
                atualiza_nota(movies, RRN, filme);
                printf("Nota atualizada com sucesso!\n");
                pausa();
            }

            break;


        default:
            break;
        }

    } while (op != 6 && op >= 1 && op <= 6);

    if (flag != 1)
    { //Se houver mudanças, imprime os índices atualizados nos respectivos arquivos
        imprime_indiceSecundario(headSecundario,idxSecundario);
        imprime_indices(head, idxPrimario);
    }
    //Fecha os arquivos
    fclose(movies);
    fclose(idxPrimario);
    fclose(idxSecundario);
    //Libera a memória utilizada pelas listas
    libera_memoria_primaria(&head);
    libera_memoria_secundaria(&headSecundario);
    return 0;
}