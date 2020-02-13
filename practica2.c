/*
    Practica 2 Analizador léxico (Cadenas, Identificadores)
    Autores: Villalobos Chirino Joshua Manuel
             Alamazan Trejo Ricardo Alejandro
    Grupo: 3CM5 - Compiladores
    Descripcion: El programa recibe como argumento el nombre 
                 del archivo que se quiere analizar, e 
                 imprime todos los tokens encontrados, si no se
                 recibe como argumento se toma la cadena desde
                 la entrada estándar
    Fecha: 12 / Febrero / 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tokens.h"
#define MAX 3000

int analiza(char * text, int * indice, char * lexema){
    int estado = 0;
    int indice_lexema = 0;
    free(lexema);
    lexema = (char *)malloc(sizeof(char) * 256);
    while(text[*indice] != '\0'){
        //printf("%c, indice: %d, estado: %d\n", text[*indice], *indice, estado);
        switch (estado){
        case 0:{
            if((text[*indice] >= 'A' && text[*indice] <= 'Z') || text[*indice] == '_'){
                estado = 1;
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            } else if(text[*indice] >= 'a' && text[*indice] <= 'z'){
                estado = 2;
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            } else if(text[*indice] == '"'){
                estado = 3;
                (*indice)++;
            } else if(isblank(text[*indice]) || text[*indice] == '\n'){
                (*indice)++;
                break;
            } else {
                estado = 7;
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            }
            break;
        }
        case 1:{
            if(isblank(text[*indice]) || text[*indice] == '\n' || text[(*indice)+1] == '\0'){
                lexema[indice_lexema] = '\0';
                (*indice)++;
                return ERROR;
            }else if((text[*indice] >= 'A' && text[*indice] <= 'Z') || text[*indice] == '_' || isdigit(text[*indice])){
                estado = 1;
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            } else if(isalpha(text[*indice])){
                estado = 2;
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            } else {
                estado = 7;
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            }
            break;
        }
        case 2:{
            if(isblank(text[*indice]) || text[*indice] == '\n' || text[(*indice)+1] == '\0'){
                lexema[indice_lexema] = '\0';
                (*indice)++;
                return ID;
            }else if(isalpha(text[*indice]) || isdigit(text[*indice]) || text[*indice] == '_'){
                estado = 2;
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            } else {
                estado = 7;
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            }
            break;
        }
        case 3:{
            if(text[*indice] == '"'){
                estado = 4;
                (*indice)++;
            } else if(text[*indice] == '\\'){
                estado = 5;
                (*indice)++;
            } else {
                estado = 3;
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            }
            break;
        }
        case 4:{
            lexema[indice_lexema] = '\0';
            (*indice)++;
            return CADENA;
        }
        case 5:{
            if(text[*indice] == 'n'){
                estado = 6;
                lexema[indice_lexema++] = '\n';
                (*indice)++;
            } else if(text[*indice] == 't'){
                estado = 6;
                lexema[indice_lexema++] = '\t';
                (*indice)++;
            } else if(text[*indice] == '"'){
                estado = 6;
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            } else if(text[*indice] == '\''){
                estado = 6;
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            } else {
                estado = 7;
                lexema[indice_lexema++] = '\\';
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            }
            break;
        }
        case 6:{
            if(text[*indice] == '"'){
                estado = 4;
                (*indice)++;
            } else if(text[*indice] == '\\'){
                estado = 5;
                (*indice)++;
            } else {
                estado = 3;
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
            }
            break;
        }
        case 7:{
            if (isblank(text[*indice]) || text[*indice] == '\n' || text[(*indice)+1] == '\0'){
                lexema[indice_lexema] = '\0';
                (*indice)++;
                return ERROR;
            } else {
                lexema[indice_lexema++] = text[*indice];
                (*indice)++;
                estado = 7;
            }
            break;
        }
        default:
            break;
        }
    }
}


int main(int argc, char *argv[]){
    FILE *file = NULL;
    char text[MAX];
    char *lexema = (char *)malloc(sizeof(char) * 256);
    if (argc > 1){
        file = fopen(argv[1], "r");
        if(file == NULL){
            printf("No se encontró el archivo\n");
            return -1;
        }
        int indice = 0;
        while (!feof(file)){
            text[indice++] = fgetc(file);
        }
        text[--indice] = '\0';
    }
    else if (argc < 2){
        fgets(text,MAX,stdin);
    }
    puts(text);
    int *indice = (int *)malloc(sizeof(int));
    *indice = 0;
    while (text[*indice] != '\0'){
        switch (analiza(text, indice, lexema)){
        case CADENA:{
            printf("Encontre una cadena: %s\n", lexema);
        }
        break;
        case ERROR:{
            printf("Encontre un error: %s\n", lexema);
        }
        break;
        case ID:{
            printf("Encontre un id: %s\n", lexema);
        }
        break;
        }
    }
}