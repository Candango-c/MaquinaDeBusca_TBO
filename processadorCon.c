#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "processadorCon.h"

struct Document{
    char *name; //nome da pagina
    double pagerank; // pagerank da pagina
};

//realiza a comparação entre dois documentos (paginas)
//a primeira comparação é qual tem o maior pageRank
//em caso de empate faz a comparação lexicografica
int compare_docs_process(Document *a, Document *b){

    if(a->pagerank > b->pagerank){
        return -1;
    } else if(a->pagerank < b->pagerank){
        return 1;
    } else {
        //os pageranks empataram
        return strcmp(a->name, b->name);
    }

}

//função de comparação que o qsort vai utilizar
static int compare_qsort(const void *a, const void *b){
    return compare_docs_process((Document*)a, (Document*)b);
}

//verifica se todos os termos pesquisados estão presentes no documento
int verify_doc_process(Index *ind, Document *doc, char **terms, int nTerms){

    for(int i=0; i < nTerms; i++){

        Tree *docs = get_documents(ind, terms[i]);
        if(docs == NULL){
            return 0;
        }

        if(search_RBtree(docs, doc->name) == NULL){
            return 0;
        }
    }
    return 1; // contem todos os termos
}

//realizia uma consulta
void init_process(Index *ind, pageRanking *p, char *word, char **terms, int nTerms){

    printf("search:%s\n", word);

    //verifica as stopwords e cria um vetor com os termos validos
    char **valid = malloc(sizeof(char*) * nTerms);
    int nValid = 0;

    for(int i=0; i < nTerms; i++){

        if(!is_stopWord(ind, terms[i])){
            valid[nValid] = terms[i];
            nValid++;
        }
    }

    //percorre todos os documentos(paginas) e guarda os que possuem todos os termos
    int nPagerank = get_n_pageRank(p);
    Document *checkDoc = malloc (sizeof(Document) * nPagerank);
    int nCheck=0; //documentos que possuem os termos

    for(int i=0; i < nPagerank; i++){

        Document aux;
        aux.name = get_name_pageRank(p, i);

        if(aux.name == NULL){
            continue;
        }

        if(verify_doc_process(ind, &aux, valid, nValid)){
            checkDoc[nCheck].name = aux.name;
            checkDoc[nCheck].pagerank = get_pageRank(p, aux.name);
            nCheck++;
        }

    }

    //ordena os documentos com quicksort
    qsort(checkDoc, nCheck, sizeof(Document), compare_qsort);

    //imprime o nome das paginas que possuem todos os termos
    printf("pages:");

    for(int i=0; i < nCheck; i++){
        printf("%s", checkDoc[i].name);
        if(!(i== nCheck -1)){
        printf(" ");
        }
    }
    printf("\n");

    //imprime o pagerank delas
    printf("pr:");

    for(int i=0; i < nCheck; i++){
        printf("%.8f", checkDoc[i].pagerank);
        if(!(i== nCheck -1)){
            printf(" ");
        }
    }
    printf("\n");

    //libera as alocações auxiliares
    free(valid);
    free(checkDoc);
    
}