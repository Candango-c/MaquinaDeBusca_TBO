#ifndef INDEXADOR_H
#define INDEXADOR_H

#include "RBtree.h"

typedef struct Index Index;

//criação do indexador
Index *creat_index();

//registra os documentos em que a palavra apareceu
void insert_index_word(Index *ind, char *key, char *docId);

//verifica se a palavra está presente nas stopwords
int is_stopWord(Index *ind, char *key);

//registra as palavras que nao devem ser contabilizadas
void insert_index_stopWord(Index *ind, char *key);

//retorna os documentos relacionados aquela palavra
Tree *get_documents(Index *ind, char *key);

//para liberar a arvore interna de documentos
void free_docs(Tree *word);

//libera as arvores associdadas ao indexador
void destroy_index(Index *ind);

#endif