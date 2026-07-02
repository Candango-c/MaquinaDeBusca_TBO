#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "pageRanking.h"

struct Page{

    char *name; //nome da pagina
    int *links_out; // indice das paginas que ela tem o link
    int nLinks_out; // numero de links para fora
    double pagerank; //pagerank da pagina
    double past_pagerank; //pagerank carregado de iterações passadas
};

struct pageRanking{

    Page *pages; //paginas presentes no ranking
    Tree *grafo_pages; //arvore que vai guardar o indice das paginas, para ter um track dos links
    int n; //numero de paginas no pageRanking
    int capacity; //capacidade do vetor de paginas
};

//cria o pageRank, onde para realizar o ranking
pageRanking *create_pageRanking(){
    
    pageRanking *p = malloc(sizeof(pageRanking));
    
    if(p == NULL){
        return NULL; //verifica se foi criado corretamente
    }
    p->n = 0;
    p->capacity = 2;
    p->grafo_pages = NULL;
    p->pages = malloc(sizeof(Page) * p->capacity); //aloca o vetor de pagians

    return p;
}

//adiciona uma pagina no pageRank
void add_page_pageRanking(pageRanking *p, char *name){
   
    if(get_ind_pageRank(p, name) != -1){
        return; // a pagina ja existe
    }

    //aumenta a capacidade do vetor se necessario
    if(p->n == p->capacity){
        p->capacity *= 2;
        p->pages = realloc(p->pages, sizeof(Page) * p->capacity);
    }

    p->grafo_pages = insert_RBtree(p->grafo_pages, name);
    Tree *pagina = search_RBtree(p->grafo_pages, name);

    int *ind = malloc(sizeof(int));
    *ind = p->n;

    set_valueRB(pagina, ind);

    //inicialização da pagina
    p->pages[p->n].name = strdup(name);
    p->pages[p->n].links_out = NULL;
    p->pages[p->n].nLinks_out = 0;
    p->pages[p->n].pagerank = 0;
    p->pages[p->n].past_pagerank = 0;

    p->n++; //aumenta o numero de paginas presente no vetor de paginas
}

//retorna o indice da pagina no pageRanks
int get_ind_pageRank(pageRanking *p, char *name){
    
    Tree *page = search_RBtree(p->grafo_pages, name);
    if(page == NULL){
        return -1;
    } else {
        return *(int*)get_valueRB(page);
    }
}

//cadastra os links da pagina
void set_links(pageRanking *p, char *link_in, char **links_out, int n_linksOut){
    
    if(get_ind_pageRank(p, link_in) !=-1){ //verifica se a pagina do link in ja existe
        // se ja existe, nao precisa adicionar no pagerank

    } else {
        add_page_pageRanking(p, link_in); //precisa adicionar
    }
    
    int ind = get_ind_pageRank(p, link_in);

    p->pages[ind].nLinks_out = n_linksOut;
    p->pages[ind].links_out = malloc(sizeof(int) * n_linksOut);

    for(int i=0; i < n_linksOut; i++){
        if(get_ind_pageRank(p, links_out[i]) !=-1){ //verifica se a pagina do link out ja existe
        // se ja existe, nao precisa adicionar no pagerank

        } else {
        add_page_pageRanking(p, links_out[i]); //precisa adicionar
        }

        p->pages[ind].links_out[i] = get_ind_pageRank(p, links_out[i]);
    }
}

//utilizando o metodo apresentado nas especificações, calcula o page Rank (power method) 
void calculate_pageRank(pageRanking *p){
//usar numero no formato 0.0 ja que é double
//alpha é 0.85
    if(p->n == 0){
        return;
    }
    
    for(int i=0; i < p->n; i++){
        p->pages[i].pagerank = 1.0/p->n; //inicializa os pageRanks das paginas
    }

    while(1){
        //
        for(int i=0; i < p->n; i++){
            p->pages[i].past_pagerank = p->pages[i].pagerank;
            p->pages[i].pagerank = (1.0 - 0.85)/p->n;
        }

        double aux1=0.0; //auxilar para quando a paigina nao tem nenhum link para fora
        double aux2=0.0; //auxiliar para quando tem link para fora
        int nextP=0; //para onde o pageRank compartilhado vai
        
        for(int i=0; i < p->n; i++){

            if(p->pages[i].nLinks_out == 0){
                aux1 += p->pages[i].past_pagerank;

            } else {
                aux2 =(0.85 * (p->pages[i].past_pagerank/p->pages[i].nLinks_out));

                for(int j=0; j < p->pages[i].nLinks_out; j++){
                    nextP = p->pages[i].links_out[j];
                    p->pages[nextP].pagerank +=aux2;
                }
            }

        }

        //redistribuição 
        double plus=0.0;

        plus = 0.85 * (aux1/p->n);
        
        for(int i=0; i < p->n; i++){
            p->pages[i].pagerank += plus;
        }

        //calculo para finalizar as iterações (pra encerrar o calculo do pagerank)
        double desvioT=0.0;
        double desvio=0.0;

        for(int i=0; i < p->n; i++){
            desvio = p->pages[i].pagerank - p->pages[i].past_pagerank;
            desvioT += fabs(desvio); //fabs serve pra pegar o valor absoluto
        }

        desvioT = desvioT / p->n;

        if(desvioT < (1e-6)){ //1e-6 é 10 elevado a menos 6
            break;
        }

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

//retorna a quantidade de paginas no pageranking
int get_n_pageRank(pageRanking *p){
    return p->n;
}

//retorna o nome da pagina no indice passado
char *get_name_pageRank(pageRanking *p, int i){
    //verifica se é valido
    if(i < 0 || i >= p->n){
        return NULL;
    }
    return p->pages[i].name;
}

//função para liberar o indice (que atua como value) no grafico de paginas
void destroy_ind(Tree *h){
    free(get_valueRB(h));
}

//libera o pageRank da memoria
void destroy_pageRanking(pageRanking *p){

    if(p != NULL){
        for(int i=0; i < p->n; i++){
            free(p->pages[i].name);
            free(p->pages[i].links_out);
        }
        free(p->pages);

        run_functRB(p->grafo_pages, destroy_ind); //libera os values alocados
        destroy_RBtree(p->grafo_pages);

        free(p);

    }
}