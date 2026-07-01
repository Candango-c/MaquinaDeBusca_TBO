#ifndef PAGERANKING_H
#define PAGERANKING_H

#include "RBtree.h"

typedef struct Page Page;

typedef struct pageRanking pageRanking;

//cria o pageRank, onde serão mapeados todos os links entre arquivos
pageRanking *create_pageRanking();

//adiciona uma pagina no pageRank
void add_page_pageRanking(pageRanking *p, char *name);

//retorna o indice da pagina no pageRank
int get_ind_pageRank(pageRanking *p, char *name);

//cadastra os links da pagina
void set_links(pageRanking *p, char *link_in, char **links_out, int n_linksOut);

//utilizando o metodo apresentado nas especificações, calcula o page Rank (power method) 
void calculate_pageRank(pageRanking *p);

//retorna o pageRank da pagina
double get_pageRank(pageRanking *p, char *name);

//função para liberar o indice (que atua como value) no grafico de paginas
void destroy_ind(Tree *h);

//libera o pageRank da memoria
void destroy_pageRanking(pageRanking *p);

#endif
