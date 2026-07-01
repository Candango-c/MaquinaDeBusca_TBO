#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indexador.h"
#include "RBtree.h"

//converte maiscula pra minuscula
void convert_to_lowerS(char *s);

int main(int agrc, char *argv[]){


    return 0;
}

void convert_to_lowerS(char *s){
    for(int i = 0; s[i] != '\0'; i++){

        if(s[i] >= 'A' && s[i] <= 'Z'){
            s[i] = s[i] + (32); //32 é a diferença entre o caracter maiusculo e minusculo na tabela ascii
        }
    }
}