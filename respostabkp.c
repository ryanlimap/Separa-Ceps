#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tadlista.h"

typedef struct {
    char cep[10];
    int numero;
    char familia[20];
} t_endereco1;

typedef struct {
    char cep[10];
    Lista enderecos;
} t_endereco2;

// Função para criar uma nova lista
Lista new_lista() {
    Lista lista = (Lista)malloc(sizeof(struct tipo_lista));
    lista->primeiro = NULL;
    lista->ultimo = NULL;
    lista->tamanho = 0;
    return lista;
}

// Função para obter o tamanho da lista
int lenLista(Lista l) {
    return l->tamanho;
}

// Função para adicionar um dado ao final da lista
Lista appendLista(Lista l, t_dado d) {
    t_no novo_no = (t_no)malloc(sizeof(struct tipo_no));
    novo_no->dado = d;
    novo_no->anterior = l->ultimo;
    novo_no->proximo = NULL;
    
    if (l->ultimo) {
        l->ultimo->proximo = novo_no;
    } else {
        l->primeiro = novo_no;
    }
    
    l->ultimo = novo_no;
    l->tamanho++;
    return l;
}

// Função para acessar um elemento da lista
t_dado elemLista(Lista l, int pos) {
    if (pos < 0 || pos >= l->tamanho) return NULL;
    
    t_no atual = l->primeiro;
    for (int i = 0; i < pos; i++) {
        atual = atual->proximo;
    }
    return atual->dado;
}

// Função para inserir um dado na lista em uma posição específica
Lista insereLista(Lista l, t_dado dado, int pos) {
    if (pos < 0 || pos > l->tamanho) return l;
    
    t_no novo_no = (t_no)malloc(sizeof(struct tipo_no));
    novo_no->dado = dado;
    
    if (pos == 0) {
        novo_no->proximo = l->primeiro;
        novo_no->anterior = NULL;
        if (l->primeiro) {
            l->primeiro->anterior = novo_no;
        }
        l->primeiro = novo_no;
        if (l->tamanho == 0) {
            l->ultimo = novo_no;
        }
    } else {
        t_no atual = l->primeiro;
        for (int i = 0; i < pos - 1; i++) {
            atual = atual->proximo;
        }
        novo_no->proximo = atual->proximo;
        novo_no->anterior = atual;
        if (atual->proximo) {
            atual->proximo->anterior = novo_no;
        }
        atual->proximo = novo_no;
        if (novo_no->proximo == NULL) {
            l->ultimo = novo_no;
        }
    }
    
    l->tamanho++;
    return l;
}

// Função para remover um elemento da lista de uma posição específica
t_dado removeLista(Lista l, int pos) {
    if (pos < 0 || pos >= l->tamanho) return NULL;
    
    t_no atual = l->primeiro;
    for (int i = 0; i < pos; i++) {
        atual = atual->proximo;
    }
    
    if (atual->anterior) {
        atual->anterior->proximo = atual->proximo;
    } else {
        l->primeiro = atual->proximo;
    }
    
    if (atual->proximo) {
        atual->proximo->anterior = atual->anterior;
    } else {
        l->ultimo = atual->anterior;
    }
    
    t_dado dado = atual->dado;
    free(atual);
    l->tamanho--;
    return dado;
}

// Função para carregar endereços do arquivo
Lista loadbdceps(char *nomearq) {
    FILE* file;
    Lista lista = new_lista();
    file = fopen(nomearq, "r");

    /* debug */
    if (!file) {
        printf("Erro ao abrir o arquivo");
        return NULL;
    }

    /* debug */
    if (lista == NULL) {
        printf("erro de alocação de vetor");
        return NULL;
    }
    
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        t_endereco1 *endereco = (t_endereco1 *)malloc(sizeof(t_endereco1));
        sscanf(line, "%[^,],%d,%[^,\n]", endereco->cep, &endereco->numero, endereco->familia);
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
            printf("%s, %d, %s\n", endereco1->cep, endereco1->numero, endereco1->familia);
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
