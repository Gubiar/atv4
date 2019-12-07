/*
* @file treeB.h
* @author Gustavo Lázaro and Yuri Getaruck
* @date 20 Nov 2019
* @brief Implementacao da árvore B
* Implementa a árvore B vista na disciplina
* pesquisa e classificacao de dados.
*/

#ifndef UNTITLED_TREEB_H
#define UNTITLED_TREEB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 3
#define TAM_REGISTRO 56
#define namefile "arvoreB.txt"

typedef struct key{
    int ID;
    int RRN;
}key;

typedef struct Pagina_str *Apontador;

typedef struct Pagina_str {
    int n;
    int pageNum;
    key key[MAX];
    Apontador filhos[MAX + 1];
} NotreeB;

struct Node{
    key info;
    struct Node *prox;
};
typedef struct Node node;


int particiona(key *V, int inicio, int fim);
/**
 * \brief Função recursiva usada pela quick sort
 *
 * \param V vetor a ser organizado
 * \param inicio posicao do vetor inicial para comecar a organizar
 * \param fim tamanho do vetor
 *
 * Funcao coloca o valor do pivo no seu lugar, garantindo que esse numero esteja ordenado
 */

void quicksort(key *V, int inicio, int fim);
/**
 * \brief Método de organização quick sort
 *
 * \param V vetor a ser organizado
 * \param inicio posicao do vetor inicial para comecar a organizar
 * \param fim tamanho do vetor
 *
 * Funcao recursiva que ordena o vetor por meio da quick sort
 */

void criaIndice(FILE *arq, int tamanho, key *vetor);

/**
 * \brief Funcao que cria um indice primario com ID e RRN, ordenado pelo ID
 *
 * \param arq arquivo de texto a ser escrito
 * \param tamanho tamanho do vetor de registro
 * \param vetor vetor de registros a serem utilizados
 *
 * Cria um indice primario
 */

void buscainFile(key Reg, Apontador pagina);
/**
 * \brief Funcao que usada na funcao Busca
 *
 * \param Reg struct contendo o registro
 * \param pagina pagina na arvore b que contem o registro
 *
 * Funcao base da Busca
 */

void Busca(key Reg, Apontador Ap);
/**
 * \brief Funcao que usada na funcao Busca
 *
 * \param Reg struct contendo o registro
 * \param ap pagina na arvore b que contem o registro
 *
 * busca um ID
 */

void InsereNaPagina(Apontador Ap, key Reg, Apontador ApDir);
/**
 * \brief Funcao base pra funcao Ins
 *
 * \param ap pagina na arvore b que contem o registro
 * \param Reg struct contendo o registro
 * \param ApDir filho da direita
 *
 * Insere um registro na arvore
 */

void Ins(key Reg, Apontador Ap, int *Cresceu, key *RegRetorno, Apontador *ApRetorno);
/**
 * \brief Funcao base pra funcao Ins
 *
 * \param ap pagina na arvore b que contem o registro
 * \param Reg struct contendo o registro
 * \param ApDir filho da direita
 *
 * Insere um registro na arvore
 */

typedef Apontador Struct;

void inicializa(Struct *Dicionario);
/**
 * \brief Inicializa a Struct da arvore
 *
 * \param Dicionario struct da arvore b
 *
 * Inicia a arvore
 */

void inicia(node *LISTA);
/**
 * \brief Inicializa o no da arvore
 *
 * \param LISTA no contido na arvore
 *
 * Inicia a pagina da arvore
 */


#endif //UNTITLED_TREEB_H
