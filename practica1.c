
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tokens.h"
#define MAX 3000

int analiza(char *text, int *indice, char *lexema)
{
    int estado = 0;
    free(lexema);
    lexema = (char *)malloc(sizeof(char) * 256);
    while (text[*indice] != '\0')
    {
        //printf("%c - %d\n", text[*indice], estado);
        switch (estado)
        {
        case 0:
        {
            if (text[*indice] == '"')
            {
                estado = 3;
                lexema[(*indice)++] = text[*indice];
                break;
            }
            else if (text[*indice] >= 97 && text[*indice] <= 97 + 26)
            {
                estado = 2;
                lexema[(*indice)++] = text[*indice];
                break;
            }
            else if (isalpha(text[*indice]) || text[*indice] == '_' || isdigit(text[*indice]))
            {
                estado = 1;
                lexema[(*indice)++] = text[*indice];
                break;
            }
            else
            {
                estado = 8;
                lexema[(*indice)++] = text[*indice];
                break;
            }
        }
        break;
        case 1:
        {
            if (text[*indice] >= 97 && text[*indice] <= 97 + 26)
            {
                estado = 2;
                lexema[(*indice)++] = text[*indice];
                break;
            }
            else if (isalpha(text[*indice]) || text[*indice] == '_' || isdigit(text[*indice]))
            {
                estado = 1;
                lexema[(*indice)++] = text[*indice];
                break;
            }
            else
            {
                estado = 8;
                lexema[(*indice)++] = text[*indice];
                break;
            }
        }
        break;
        case 2:
        {
            if (isalpha(text[*indice]) || isdigit(text[*indice]) || text[*indice] == '_')
            {
                if (isblank(text[*indice]) || text[*indice] == '\n')
                {
                    lexema[*indice] = '\0';
                    return ID;
                }
                else
                {
                    estado = 2;
                    lexema[(*indice)++] = text[*indice];
                    break;
                }
            }
            else
            {
                estado = 8;
                lexema[(*indice)++] = text[*indice];
                break;
            }
        }
        break;
        case 3:
        {
            if (text[*indice] == '\\')
            {
                estado = 4;
                lexema[(*indice)++] = text[*indice];
                break;
            }
            else if (text[*indice] == '"')
            {
                estado = 7;
                lexema[(*indice)++] = text[*indice];
                break;
            }
            else
            {
                estado = 5;
                lexema[(*indice)++] = text[*indice];
                break;
            }
        }
        break;
        case 4:
        {
            if (text[*indice] == '"' || text[*indice] == '\'' || text[*indice] == 't' || text[*indice] == 'n')
            {
                estado = 6;
                lexema[(*indice)++] = text[*indice];
                break;
            }
            else
            {
                estado = 8;
                lexema[(*indice)++] = text[*indice];
                break;
            }
        }
        break;
        case 5:
        {
            if (text[*indice] == '"')
            {
                estado = 7;
                lexema[(*indice)++] = text[*indice];
                break;
            }
            else
            {
                estado = 5;
                lexema[(*indice)++] = text[*indice];
                break;
            }
        }
        break;
        case 6:
        {
            if (text[*indice] == '\\')
            {
                estado = 4;
                lexema[(*indice)++] = text[*indice];
                break;
            }
            else
            {
                estado = 5;
                lexema[(*indice)++] = text[*indice];
                break;
            }
        }
        break;
        case 7:
        {
            if (isblank(text[*indice]) || text[*indice] == '\n')
            {
                text[*indice] = '\0';
                return CADENA;
            }
            else
            {
                estado = 8;
                lexema[(*indice)++] = text[*indice];
                break;
            }
        }
        break;
        case 8:
        {
            text[*indice] = '\0';
            return ERROR;
        }
        break;
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    char text[MAX];
    char *lexema = (char *)malloc(sizeof(char) * 256);
    if (argc > 1)
    {
        file = fopen(argv[1], "r");
        int indice = 0;
        while (!feof(file))
        {
            text[indice++] = fgetc(file);
        }
        text[indice] = '\0';
        puts(text);
    }
    else if (file == NULL || argc < 2)
    {
        printf("Nombre del archivo: ");
        scanf("%s", text);
    }
    int *indice = (int *)malloc(sizeof(int));
    *indice = 0;
    while (text[*indice] != '\0')
    {
        switch (analiza(text, indice, lexema))
        {
        case CADENA:
        {
            printf("Encontre una cadena %s\n", lexema);
        }
        break;
        case ERROR:
        {
            printf("Encontre un error %s\n", lexema);
        }
        break;
        case ID:
        {
            printf("Encontre un id %s\n", lexema);
        }
        break;
        }
    }
}