#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "RBtree.h"


//a key vai guardar a palavra
//o value vai guardar os arquivos onde essa palavra marca presença
//color guarda a cor do nó
//o "l" guarda o nó a esquerda (menor valor) enquanto o "r" guarda o nó a direita (maior valor)
// as keys que serão utilizadas pra fazer a comparação entre os nós
struct Tree{

    char *key;
    void *value;
    bool color;
    struct Tree *l, *r;
};

//verifica a cor atual do nó na arvore
bool is_Red(Tree *h){
    if(h == NULL){
        return BLACK;
    } else {

        return h->color; //RED == true
    }
}

//rotaciona o nó e seus filhos para a esquerda
Tree *rotate_Left(Tree *h){
    Tree *x = h->r;
    h->r = x->l;
    x->l = h;
    x->color = x->l->color;
    x->l->color = RED;
    return x;
}

//rotaciona o nó e seus filhos para a direita
Tree *rotate_Right(Tree *h){
    Tree *x = h->l;
    h->l = x->r;
    x->r = h;
    x->color = x->r->color;
    x->r->color = RED;
    return x;
}

//troca a cor do nó quando seus dois filhos sao vermelhos 
void flip_Colors(Tree *h){
    h->color = RED;
    h->l->color = BLACK;
    h->r->color = BLACK;
}

//cria um nó para a arvore RB
Tree *create_nodeRB(char *key){
    Tree *n = malloc(sizeof(Tree));

    if(n == NULL){ 
        return NULL; //verifica se foi criado corretamente
    }
    n->key = malloc(strlen(key) + 1); // +1 é pra incluir o '\0'
    strcpy(n->key, key);

    n->value = NULL;
    n->color = RED;   // todo novo nó é inserido como vermelho na árvore
    n->l = NULL;
    n->r = NULL;
    return n;
}

//insere na arvore RB
Tree *insert_RBtree(Tree *h, char *key){
    //caso nao exista na arvore, cria um novo nó
    if(h == NULL){
        h = create_nodeRB(key);
    }
    int cmp = strcmp(key, h->key);
    if(cmp < 0){
        h->l = insert_RBtree(h->l, key);
    } else if(cmp > 0){
        h->r = insert_RBtree(h->r, key);
    } else {
        //a chave ja existe
    }

    //area de ajuste da árvore (rotações e mudanças de cor)

    //lean left
    if(is_Red(h->r) && !is_Red(h->l)){
        h = rotate_Left(h);
    }

    //balance 4-node
    if(is_Red(h->l) && is_Red(h->l->l)){
        h = rotate_Right(h);
    }

    //split 4-node
    if(is_Red(h->l) && is_Red(h->r)){
        flip_Colors(h);
    }

    return h;

}

//procura a existencia daquele elemento na arvore RB
Tree *search_RBtree(Tree *h, char *key){
    while(h !=NULL){

        int cmp = strcmp(key, h->key);

        if(cmp < 0){
            h = h->l;
        } else if(cmp > 0){
            h = h->r;
        } else {
            return h; //achou
        }
    }
    return NULL; //não encontrou
}


//retorna o value do nó
void *get_valueRB(Tree *h){
    return h->value;
}

//configura um valor para o nó
void set_valueRB(Tree *h, void *value){
    h->value = value;
}

//função criada para percorre a arvore aplicando uma função em canda nó
//foi necessario criar essa função para conseguir liberar as arvores internas
//que são criadas no indexador
void run_functRB(Tree *h, void (*funct)(Tree *)){
    if(h != NULL){
        run_functRB(h->l, funct);
        funct(h);
        run_functRB(h->r, funct);
    }
}


//libera a arvore RB da memoria
void destroy_RBtree(Tree *h){
    if(h != NULL){

        destroy_RBtree(h->l); //libera a esquerda do nó
        destroy_RBtree(h->r); //libera a direita do nó
        free(h->key); // libera a chave do nó
        free(h); // libera o nó
    }
}