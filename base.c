#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pageRanking.h"

#define ALPHA 0.85
#define EPS   1e-6
#define MAX_ITER 100

struct Page{
    char *name;
    int *links_out;
    int nLinks_out;
    double pagerank;
    double past_pagerank;
};

struct pageRanking{
    Page *pages;
    Tree *grafo_pages;
    int n;
    int cap;              // (novo) capacidade do vetor de paginas
};

//cria o pageRank
pageRanking *create_pageRanking(){
    pageRanking *p = malloc(sizeof(pageRanking));
    if(p == NULL){
        return NULL;
    }

    p->n = 0;                                   // (corrigido) iniciar n
    p->cap = 16;                                // (novo) capacidade inicial
    p->pages = malloc(sizeof(Page) * p->cap);   // (corrigido) alocar pages
    p->grafo_pages = NULL;

    return p;
}

//adiciona uma pagina no pageRank
void add_page_pageRanking(pageRanking *p, char *name){
    if(get_ind_pageRank(p, name) != -1){
        return; // a pagina ja existe
    }

    if(p->n == p->cap){                                 // (novo) cresce se encheu
        p->cap *= 2;
        p->pages = realloc(p->pages, sizeof(Page) * p->cap);
    }

    p->grafo_pages = insert_RBtree(p->grafo_pages, name);
    Tree *pagina = search_RBtree(p->grafo_pages, name);
    set_valueRB(pagina, (void*)(long) p->n);            // (corrigido) guarda o indice

    //inicialização da pagina
    p->pages[p->n].name          = strdup(name);
    p->pages[p->n].links_out     = NULL;                // (corrigido) era 0
    p->pages[p->n].nLinks_out    = 0;
    p->pages[p->n].pagerank      = 0;
    p->pages[p->n].past_pagerank = 0;

    p->n++;
}

//retorna o indice da pagina no pageRank
int get_ind_pageRank(pageRanking *p, char *name){
    Tree *page = search_RBtree(p->grafo_pages, name);
    if(page == NULL){
        return -1;
    } else {
        return (int)(long) get_valueRB(page);           // (corrigido) cast correto
    }
}

//cadastra os links da pagina
void set_links(pageRanking *p, char *link_in, char **links_out, int n_linksOut){
    if(get_ind_pageRank(p, link_in) == -1){
        add_page_pageRanking(p, link_in);
    }

    int ind = get_ind_pageRank(p, link_in);

    p->pages[ind].nLinks_out = n_linksOut;
    p->pages[ind].links_out  = (n_linksOut > 0)
                             ? malloc(sizeof(int) * n_linksOut)
                             : NULL;

    for(int i = 0; i < n_linksOut; i++){
        if(get_ind_pageRank(p, links_out[i]) == -1){
            add_page_pageRanking(p, links_out[i]);
        }
        p->pages[ind].links_out[i] = get_ind_pageRank(p, links_out[i]);
    }
}

//calcula o pageRank (power method)
void calculate_pageRank(pageRanking *p){
    int n = p->n;
    if(n == 0) return;

    // 1. inicializa com 1/n
    for(int i = 0; i < n; i++)
        p->pages[i].pagerank = 1.0 / n;                 // (corrigido) 1.0

    for(int iter = 0; iter < MAX_ITER; iter++){

        // guarda anterior e zera com o termo base (1-alpha)/n
        for(int i = 0; i < n; i++){
            p->pages[i].past_pagerank = p->pages[i].pagerank;
            p->pages[i].pagerank      = (1.0 - ALPHA) / n;
        }

        // distribui a autoridade de cada pagina j
        double dangling = 0.0;
        for(int j = 0; j < n; j++){
            if(p->pages[j].nLinks_out == 0){
                dangling += p->pages[j].past_pagerank;
            } else {
                double share = ALPHA * p->pages[j].past_pagerank
                                     / p->pages[j].nLinks_out;
                for(int k = 0; k < p->pages[j].nLinks_out; k++){
                    int destino = p->pages[j].links_out[k];
                    p->pages[destino].pagerank += share;
                }
            }
        }

        // redistribui a massa dos dangling nodes
        double extra = ALPHA * dangling / n;
        for(int i = 0; i < n; i++)
            p->pages[i].pagerank += extra;

        // erro medio
        double erro = 0.0;
        for(int i = 0; i < n; i++){
            double d = p->pages[i].pagerank - p->pages[i].past_pagerank;
            erro += (d < 0) ? -d : d;
        }
        erro /= n;

        if(erro < EPS) break;
    }
}

//retorna o pageRank da pagina
double get_pageRank(pageRanking *p, char *name){
    int ind = get_ind_pageRank(p, name);
    if(ind != -1){
        return p->pages[ind].pagerank;
    } else {
        return 0;
    }
}

//libera o pageRank da memoria
void destroy_pageRanking(pageRanking *p){
    if(p != NULL){
        for(int i = 0; i < p->n; i++){
            free(p->pages[i].name);
            free(p->pages[i].links_out);
        }
        free(p->pages);

        destroy_RBtree(p->grafo_pages);   // (corrigido) sem run_functRB/destroy_ind
        free(p);
    }
}
