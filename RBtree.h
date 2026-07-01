#ifndef RBTREE_H
#define RBTREE_H

#include <stdbool.h>

#define RED true
#define BLACK false

typedef struct Tree Tree;

//verifica a cor atual do nó na arvore
bool is_Red(Tree *h);

//rotaciona o nó e seus filhos para a esquerda
Tree *rotate_Left(Tree *h);

//rotaciona o nó e seus filhos para a direita
Tree *rotate_Right(Tree *h);

//troca a cor do nó quando seus dois filhos sao vermelhos 
void flip_Colors(Tree *h);

//cria um nó na folha
Tree *create_nodeRB(char *key);

//insere na arvore RB
Tree *insert_RBtree(Tree *h, char *key);

//procura a existencia daquele elemento na arvore RB
Tree *search_RBtree(Tree *h, char *key);

//retorna o value do nó
void *get_valueRB(Tree *h);

//retorna a key do nó
void *get_keyRB(Tree *h);

//configura um valor para o nó
void set_valueRB(Tree *h, void *value);

//função criada para percorre a arvore aplicando uma função em canda nó
//foi necessario criar essa função para conseguir percorrer as arvores internas
//que sao utilizadas no indexador
void run_functRB(Tree *h, void (*funct)(Tree *));

//libera a arvore RB da memoria
void destroy_RBtree(Tree *h);

#endif