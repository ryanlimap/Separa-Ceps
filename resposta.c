#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tadlista.h"

typedef struct {
    char cep[10];
    char numero[10];
    char familia[20];
} t_endereco1;

typedef struct {
    char cep[10];
    Lista enderecos;
} t_endereco2;

// Função para carregar endereços do arquivo
Lista loadbdceps(char *nomearq) {
    FILE *file = fopen(nomearq, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo");
        return NULL;
    }
    
    Lista lista = new_lista();
    char line[100];
    
    while (fgets(line, sizeof(line), file)) {
        // Remover o caractere de nova linha, se presente
        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        // Criar uma nova estrutura t_endereco1
        t_endereco1 *endereco = (t_endereco1 *)malloc(sizeof(t_endereco1));
        if (!endereco) {
            printf("Erro ao alocar memória");
            fclose(file);
            return NULL;
        }

        // Extrair o CEP
        char *p = line;
        int i = 0;
        while (*p != ',' && *p != '\0') {
            endereco->cep[i++] = *p++;
        }
        endereco->cep[i] = '\0';

        // Avançar o ponteiro após a vírgula
        if (*p == ',') {
            p++;
        }

        // Extrair o número como uma string
        i = 0;
        while (*p != ',' && *p != '\0') {
            endereco->numero[i++] = *p++;
        }
        endereco->numero[i] = '\0';

        // Avançar o ponteiro após a vírgula
        if (*p == ',') {
            p++;
        }

        // Extrair o nome da família
        i = 0;
        while (*p != '\0' && i < sizeof(endereco->familia) - 1) {
            endereco->familia[i++] = *p++;
        }
        endereco->familia[i] = '\0';

        // Adicionar o endereço à lista
        appendLista(lista, endereco);
    }
    
    fclose(file);
    return lista;
}

// Função para converter lista de endereços do tipo 1 para tipo 2
Lista converte(Lista lst1) {
    Lista lst2 = new_lista();
    
    for (int i = 0; i < lenLista(lst1); i++) {
        t_endereco1 *endereco1 = (t_endereco1 *)elemLista(lst1, i);
        
        // Verifica se o cep já está na lista de tipo 2
        int encontrado = 0;
        for (int j = 0; j < lenLista(lst2); j++) {
            t_endereco2 *endereco2 = (t_endereco2 *)elemLista(lst2, j);
            if (strcmp(endereco2->cep, endereco1->cep) == 0) {
                appendLista(endereco2->enderecos, endereco1);
                encontrado = 1;
                break;
            }
        }
        
        // Se não encontrado, cria um novo t_endereco2
        if (!encontrado) {
            t_endereco2 *novo_endereco2 = (t_endereco2 *)malloc(sizeof(t_endereco2));
            strcpy(novo_endereco2->cep, endereco1->cep);
            novo_endereco2->enderecos = new_lista();
            appendLista(novo_endereco2->enderecos, endereco1);
            appendLista(lst2, novo_endereco2);
        }
    }
    
    return lst2;
}

// Função para imprimir endereços do tipo 2
void print_enderecos_2(Lista lst) {
    for (int i = 0; i < lenLista(lst); i++) {
        t_endereco2 *endereco2 = (t_endereco2 *)elemLista(lst, i);
        printf("CEP: %s\n", endereco2->cep);
        
        for (int j = 0; j < lenLista(endereco2->enderecos); j++) {
            t_endereco1 *endereco1 = (t_endereco1 *)elemLista(endereco2->enderecos, j);
            printf("%s, %s, %s\n", endereco1->cep, endereco1->numero, endereco1->familia);
        }
        printf("\n");
    }
}

int main() {
    char *nomearq = "bdceps.txt";
    Lista enderecos1 = loadbdceps(nomearq);
    Lista enderecos2 = converte(enderecos1);
    print_enderecos_2(enderecos2);

    return 0;
}