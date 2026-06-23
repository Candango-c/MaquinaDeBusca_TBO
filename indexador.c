#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "indexador.h"

//Relembrando:
// key é a palavra
// value é a arvore de documentos relacionados à palavra
struct Index{
    Tree *root;
};

//criação do indexador
Index *creat_index(){
    Index *ind = malloc(sizeof(Index));
    if(ind == NULL){
        return NULL; // verifica se foi criado corretamente
    }
    ind->root = NULL;
    return ind;
}

//registra os documentos em que a palavra apareceu
void insert_index_word(Index *ind, char *key, char *docId){

    //insere a palavra na arvore de palavras, se ja nao estiver inserida
    ind->root = insert_RBtree(ind->root, key);

    //area para adicionar os documentos na lista dessa palavra

    Tree *word = search_RBtree(ind->root, key);

    Tree *docs = (Tree*) get_valueRB(word);

    docs = insert_RBtree(docs, docId);

    set_valueRB(word, docs);
}

//retorna os documentos relacionados aquela palavra
Tree *get_documents(Index *ind, char *key){
    Tree *word = search_RBtree(ind->root, key);
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
        run_functRB(ind->root, free_docs);
        //
        destroy_RBtree(ind->root);
        free(ind);
    }
}
