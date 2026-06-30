#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "indexador.h"

//Relembrando:
// key é a palavra
// value é a arvore de documentos relacionados à palavra
struct Index{
    Tree *words; //arvore das palvras adicionadas
    Tree *stopWords; //arvore com as palavras que não devem ser adicionadas
};

//criação do indexador
Index *creat_index(){
    Index *ind = malloc(sizeof(Index));
    if(ind == NULL){
        return NULL; // verifica se foi criado corretamente
    }
    ind->words = NULL;
    ind->stopWords = NULL;
    return ind;
}

//registra as palavras que nao devem ser contabilizadas
void insert_index_stopWord(Index *ind, char *key){

    ind->stopWords = insert_RBtree(ind->stopWords, key);
}

//verifica se a palavra está presente nas stopwords
int is_stopWord(Index *ind, char *key){
    
    if(search_RBtree(ind->stopWords, key) == NULL){
        return 0;
    } else {
        return 1;
    }
}

//registra os documentos em que a palavra apareceu
void insert_index_word(Index *ind, char *key, char *docId){

    //verifica se a palavra é uma stopword
    if(is_stopWord(ind, key)){

        //nao adiciona na arvore de words

    } else{

        //insere a palavra na arvore de palavras, se ja nao estiver inserida
        ind->words = insert_RBtree(ind->words, key);

        //area para adicionar os documentos na lista dessa palavra

        Tree *word = search_RBtree(ind->words, key);

        Tree *docs = (Tree*) get_valueRB(word);

        docs = insert_RBtree(docs, docId);

        set_valueRB(word, docs);
    }
}

//retorna os documentos relacionados aquela palavra
Tree *get_documents(Index *ind, char *key){
    Tree *word = search_RBtree(ind->words, key);
    if(word == NULL){
        return NULL;
    }
    return(Tree *) get_valueRB(word);
}

//para liberar a arvore interna de documentos
void free_docs(Tree *word){
    destroy_RBtree((Tree*) get_valueRB(word)); //type do value para uma arvore
}

//libera as arvores associdadas ao indexador
void destroy_index(Index *ind){
    if(ind != NULL){
        //utiliza a função parar percorrer toda arvore interna de documentos
        //e a medida que vai percorrendo, libera da memoria os nós
        run_functRB(ind->words, free_docs);
        //
        destroy_RBtree(ind->words);
        destroy_RBtree(ind->stopWords);
        free(ind);
    }
}
