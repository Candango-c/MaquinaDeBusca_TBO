#ifndef PROCESSADORCON_H
#define PROCESSADORCON_H


#include "indexador.h"
#include "RBtree.h"
#include "pageRanking.h"

//documento representa a pagina que contem as palavras
typedef struct Document Document;

//realiza a comparação entre dois documentos (paginas)
//a primeira comparação é qual tem o maior pageRank
//em caso de empate faz a comparação lexicografica
int compare_docs_process(Document *a, Document *b);

//verifica se todos os termos pesquisados estão presentes no documento
int verify_doc_process(Index *ind, Document *doc, char **terms, int nTerms);

//realizia uma consulta
void init_process(Index *ind, pageRanking *p, char *word, char **terms, int nTerms);


#endif