#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RBtree.h"
#include "indexador.h"
#include "pageRanking.h"
#include "processadorCon.h"

//converte maiscula pra minuscula
void convert_to_lowerS(char *s);

int main(int argc, char *argv[]){

    //inicialização
    char *diretory = argv[1];
    Index *ind = creat_index();
    pageRanking *p = create_pageRanking();
    char *path = NULL;
    size_t sizePath = 0;

    //manipulação das stopwords
    sizePath = strlen(diretory) + strlen("/stopwords.txt") + 1;
    path = malloc(sizePath);
    sprintf(path, "%s/stopwords.txt", diretory);

    FILE *stopF = fopen(path, "r");

    if(stopF != NULL){
        char *line = NULL;
        size_t sizeLine = 0;

        while(getline(&line, &sizeLine, stopF) != -1){
            char *tok = strtok(line, " \t\r\n");

            while(tok != NULL){
                convert_to_lowerS(tok);
                insert_index_stopWord(ind, tok);
                tok = strtok(NULL, " \t\r\n");
            }
        }
        //libera as alocações para poder reutilizar na abertura dos outros arquivos
        free(line);
        fclose(stopF);
    }
    free(path);

    // manipulação do graph e a montagem do pageRanking
    sizePath = strlen(diretory) + strlen("/graph.txt") + 1;
    path = malloc(sizePath);
    sprintf(path, "%s/graph.txt", diretory);

    FILE *graphF = fopen(path, "r");

    if(graphF != NULL){
        char *line = NULL;
        size_t sizeLine = 0;

        while(getline(&line, &sizeLine, graphF) != -1){

            char *origem = strtok(line, " \t\r\n");
            if(origem == NULL){
                continue;
            }

            char *nStr = strtok(NULL, " \t\r\n");
            if(nStr == NULL){
                continue;
            }

            int nOut = atoi(nStr);
            char **links_out = NULL;

            if(nOut > 0){
                links_out = malloc(sizeof(char*) * nOut);
            }

            int cont = 0;

            for(int i = 0; i < nOut; i++){
                char *dest = strtok(NULL, " \t\r\n");

                if(dest == NULL){
                    break;
                }

                links_out[cont] = strdup(dest);
                cont++;
            }

            set_links(p, origem, links_out, cont);

            //libera da memoria
            for(int i = 0; i < cont; i++){
                free(links_out[i]);
            }
            free(links_out);
        }
        free(line);
        fclose(graphF);
    }
    free(path);

    //manipulação do index e indexação das paginas (documentos)
    sizePath = strlen(diretory) + strlen("/index.txt") + 1;
    path = malloc(sizePath);
    sprintf(path, "%s/index.txt", diretory);

    FILE *indexF = fopen(path, "r");
    //libera para reutilizar em pages
    free(path);
    path = NULL;

    if(indexF != NULL){
        char *nome = NULL;
        size_t sizeNome = 0;

        while(getline(&nome, &sizeNome, indexF) != -1){

            // remove quebra de linha do nome
            nome[strcspn(nome, "\r\n")] = '\0';

            if(nome[0] == '\0'){
                continue; // linha vazia
            }

            // garante que o documento existe no pageRanking
            if(get_ind_pageRank(p, nome) == -1){
                add_page_pageRanking(p, nome);
            }

            // monta caminho do documento em pages
            size_t sizeDocPath = strlen(diretory) + strlen("/pages/") + strlen(nome) + 1;
            char *docPath = malloc(sizeDocPath);
            sprintf(docPath, "%s/pages/%s", diretory, nome);

            FILE *docF = fopen(docPath, "r");
            free(docPath);

            if(docF != NULL){
                char *line = NULL;
                size_t sizeLine = 0;

                while(getline(&line, &sizeLine, docF) != -1){
                    char *tok = strtok(line, " \t\r\n");

                    while(tok != NULL){
                        convert_to_lowerS(tok);
                        insert_index_word(ind, tok, nome);
                        tok = strtok(NULL, " \t\r\n");
                    }
                }
                //libera da memorira
                free(line);
                fclose(docF);
            }
        }
        free(nome);
        fclose(indexF);
    }

    //calcula o pageRank
    calculate_pageRank(p);

    //manipulação das searchs, ou seja, a pesquisa de termos (palavras)
    char *line = NULL;
    size_t sizeLine = 0;

    while(getline(&line, &sizeLine, stdin) != -1){

        //remove \n / \r do fim
        line[strcspn(line, "\r\n")] = '\0';

        //consulta original para a linha
        char *original = strdup(line);

        //separa os termos
        int nTerms = 0;
        int capacityTerms = 2;
        char **terms = malloc(sizeof(char*) * capacityTerms);

        char *tok = strtok(line, " \t");

        while(tok != NULL){
            convert_to_lowerS(tok);

            if(nTerms == capacityTerms){
                capacityTerms *= 2;
                terms = realloc(terms, sizeof(char*) * capacityTerms);
            }

            terms[nTerms++] = tok;
            tok = strtok(NULL, " \t");
        }

        //processo de consulta
        init_process(ind, p, original, terms, nTerms);

        free(terms);
        free(original);
    }

    //libera a memoria
    free(line);
    destroy_index(ind);
    destroy_pageRanking(p);

    return 0;
}

void convert_to_lowerS(char *s){
    for(int i = 0; s[i] != '\0'; i++){

        if(s[i] >= 'A' && s[i] <= 'Z'){
            s[i] = s[i] + (32); //32 é a diferença entre o caracter maiusculo e minusculo na tabela ascii
        }
    }
}
