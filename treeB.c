#include "treeB.h"

int particiona(key *V, int inicio, int fim){  //funcao usada pela quicksort

    int esquerda = inicio;
    int direita = fim;
    int pivo = V[inicio].ID;

    while(esquerda<=direita){

        while(V[esquerda].ID <= pivo){
            esquerda++;
        }

        while(V[direita].ID > pivo){
            direita--;
        }

        if(esquerda < direita){

            key aux = V[esquerda];
            V[esquerda] = V[direita];
            V[direita] = aux;
        }
    }

    key aux =V[inicio];
    V[inicio] = V[direita];
    V[direita] = aux;

    return direita;
}

void quicksort(key *V, int inicio, int fim){ //funcao quick sort
    int pivo;

    if(inicio < fim){
        pivo = particiona(V, inicio, fim);
        quicksort(V,inicio, pivo-1);
        quicksort(V, pivo+1, fim);
    }

}

void criaIndice(FILE *arq, int tamanho, key *vetor){  //funcao que cria o indice primario ordenado pelo ID

    arq = fopen("indicePrimario_ID", "w"); //Cria o arquivo de indice em modo de escrita

    for (int j = 0; j < tamanho ; ++j) {

        fprintf(arq, "%i|%i \n", vetor[j].ID, vetor[j].RRN);  //insere no arquivo de indice o ID e RRN
    }

    fflush(arq);  //garante a passagem de todas as informaçoes pro arquivo
    fclose(arq);  //fecha o arquivo
    printf("Arquivo de indice cirado\n");
}



void inicializa(Struct *Dicionario) {  /* Inicializa a estrutura */
    *Dicionario = NULL;
}

void inicia(node *LISTA) {
    LISTA->prox = NULL;
}

void buscainFile(key Reg, Apontador pagina) {
    key reg[MAX];
    int i;
    FILE *arq = fopen(namefile,"r");
    if (arq == NULL)
        exit(1);
    fseek(arq, pagina->pageNum*(MAX*sizeof(key)), SEEK_SET);
    fread(reg, (MAX*sizeof(key)),1,arq);
    fclose(arq);
    for(i = 0; i < MAX; i++){
        if (Reg.ID == reg[i].ID)
            printf("%d\n",reg[i].RRN);
    }
}


void Busca(key Reg, Apontador Ap){
    int i;

    if (Ap == NULL) {
        printf("chave nao encontrada: %d\n", Reg.ID);
        return;
    }
    i = 1;
    while (i < Ap->n && Reg.ID > Ap->key[i - 1].ID)
        i++;
    if (Reg.ID == Ap->key[i - 1].ID) {
        printf("chave: %d \n", Reg.ID);
        buscainFile(Ap->key[i - 1], Ap);
        return;
    }
    if (Reg.ID < Ap->key[i - 1].ID)
        Busca(Reg, Ap->filhos[i - 1]);
    else
        Busca(Reg, Ap->filhos[i]);
}

void InsereNaPagina(Apontador Ap, key Reg, Apontador ApDir) {

    int k;
    int NaoAchouPosicao;

    k = Ap->n;
    NaoAchouPosicao = k > 0;

    while (NaoAchouPosicao) {
        if (Reg.ID >= Ap->key[k - 1].ID) {
            NaoAchouPosicao = 0;
            break;
        }

        Ap->key[k] = Ap->key[k - 1];

        Ap->filhos[k + 1] = Ap->filhos[k];

        k--;
        if (k < 1)
            NaoAchouPosicao = 0;
    }

    Ap->key[k] = Reg;
    Ap->filhos[k + 1] = ApDir;
    Ap->n++;
}

void Ins(key Reg, Apontador Ap, int *Cresceu, key *RegRetorno, Apontador *ApRetorno) {
    Apontador ApTemp;
    key Aux;
    int i, j;

    if (Ap == NULL) {
        *Cresceu = 1;
        *RegRetorno = Reg;
        *ApRetorno = NULL;
        return;
    }
    i = 1;

    while (i < Ap->n && Reg.ID > Ap->key[i - 1].ID)
        i++;

    if (Reg.ID == Ap->key[i - 1].ID) {
        printf("chave ja existente: %d \n", Reg.ID);
        *Cresceu = 0;
        return;
    }
    if (Reg.ID < Ap->key[i - 1].ID)
        Ins(Reg, Ap->filhos[i - 1], Cresceu, RegRetorno, ApRetorno);

    else
        Ins(Reg, Ap->filhos[i], Cresceu, RegRetorno, ApRetorno);

    if (!*Cresceu)
        return;

    if (Ap->n < 2*MAX) {  /* Verificando se a pagina tem espaco */
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = 0;
        return;
    }

    /* Split: NotreeB tem que ser dividida */
    ApTemp = (Apontador) malloc(sizeof(NotreeB));
    ApTemp->n = 0;
    ApTemp->filhos[0] = NULL;
    ApTemp->n++;

    if (i <= MAX + 1) {
        InsereNaPagina(ApTemp, Ap->key[2 * MAX - 1], Ap->filhos[2 * MAX]);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    }else{
        InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    }

    for (j = MAX + 2; j <= 2*MAX; j++)
        InsereNaPagina(ApTemp, Ap->key[j - 1], Ap->filhos[j]);


    Ap->n = MAX;
    ApTemp->filhos[0] = Ap->filhos[MAX + 1];
    *RegRetorno = Ap->key[MAX];
    *ApRetorno = ApTemp;

    for (j = MAX; j < Ap->n+2; j++){
        Aux.ID = 0;
        Ap->key[j] = Aux;
    }
}

int main(int argc, char const *argv[]) {

    char buffer[5];  //buffer para armazenar o numero do indice
    int tamanho_vetor;  // varaivel para armazenar quantos cadastros tem no arquivo

    FILE *arq; //variavel para abrir o arquivo

    arq = fopen("entrada.txt", "r");

    fseek(arq,0,SEEK_END); //posicionar o "ponteiro" do arquivo para o final dele
    tamanho_vetor = ftell(arq); // pegar o numero de bytes do arquivo
    tamanho_vetor = tamanho_vetor/56; //como cada registro tem 56 bytes, então o total/56 resulta na quantidade de registros
    fseek(arq,0,SEEK_SET); // voltar o "ponteiro" para o começo do arquivo

    key vetor[tamanho_vetor];  //inicia um vetor do tipo Registros para armazenar os IDS e RRNs

    for(int i =0; i<tamanho_vetor; i++){

        fread(buffer, 4,1 ,arq); //ler o ID do registro
        vetor[i].ID = atoi(buffer); //converte a string para int e armazena na struct
        vetor[i].RRN = i; // o RRN de cada regsitro, ou seja a posição no arquivo original
        fseek(arq, 52, SEEK_CUR); // como leu 4 bytes, avança mais 52 para ir pro proximo registro
    }

    quicksort(vetor,0,tamanho_vetor-1); //ordena o vetor de key pelo ID de cada posição
    fclose(arq); //fechar o arquivo

    //**********************************************************************

    criaIndice(arq, tamanho_vetor, vetor); //função que cria o indice primario ordenado pelo ID dos registros

    FILE *arq2; // varaivel necessaria para criar o arquivo ordenado pelo registro

    arq = fopen("entrada.txt", "r"); //abre o arquivo original em modo leitura
    arq2 = fopen("DadosPilotoID.txt", "w"); //abre o arquivo novo em modo escrita

    char registro[57]; //buffer para armazenar cada registro do arquivo original

    for (int k = 0; k < tamanho_vetor ; ++k) {  // for que vai até acabar os registros

        fseek(arq, 56 * vetor[k].RRN, SEEK_SET); //posiciona o ponteiro do arquivo original na posição do registro solicitado pelo RRN do msm
        fgets(registro, 57, arq);  //copia o registro(56 bytes)
        fprintf(arq2, "%s", registro); //copia o registro no arquivo novo, como a struct de registro já está ordenada pelo ID, o arquivo novo também estará
    }
    fflush(arq2); //garante a passagem de todas as informaçoes pro arquivo novo
    fclose(arq); // fecha o arquivo 1
    fclose(arq2); //fecha o arquivo 2

    Struct *Dicionario;

    node LISTA;

    inicializa(Dicionario);

    inicia(&LISTA);


    return 0;
}
