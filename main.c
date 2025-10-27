#include <stdio.h>
#include <stdlib.h>

//struct representando um ponto do labirinto
//cada ponto tem as coordenadas x e y -> (coluna(x), linha(y))
struct ponto {
    int x;
    int y;
};

typedef struct ponto ponto;

//struct de uma fila circular usada para fazer a Busca em Largura
struct fila {
    int inicio;
    int final; 
    ponto *pontos; 
};

typedef struct fila *fila;

fila criar_fila(int tamanhoFila); //cria uma fila de tamanho definido pelo usuário
void inserir_fila(fila filaLabirinto, ponto p, int tamanhoFila); //insere um novo ponto na fila
ponto remover_fila(fila filaLabirinto, int tamanhoFila); //remove um ponto da fila
int fila_vazia(fila filaLabirinto); //verifica se a fila esta vazia
void liberar_fila(fila filaLabirinto); //libera toda a memoria alocada na heap pela fila
void buscaEmLargura_labirinto(char **labirinto, int **volumeSala, int dimensaoGrid); //realiza a Busca em Largura na fila

int main() {
    int dimensaoGrid;
    fila filaLabirinto;

    scanf("%d\n", &dimensaoGrid);

    char **labirinto = malloc(sizeof(char*) * dimensaoGrid); //aloca na heap memoria para as linhas do labirinto (vetor de ponteiros)
    int **volumeSala = malloc(sizeof(int*) * dimensaoGrid); //aloca na heap memoria para o volume das salas (vetor de ponteiros)

    for (int y = 0; y < dimensaoGrid; y++) {
        labirinto[y] = malloc(sizeof(char) * dimensaoGrid); //aloca na heap memoria paras as colunas de cada linha do labirinto
        volumeSala[y] = malloc(sizeof(int) * dimensaoGrid); //aloca na heap memoria para as colunas de cada lnha do volume das salas
    }

    for (int y = 0; y < dimensaoGrid; y++) { ///percorre as linhas da matriz
        for (int x = 0; x < dimensaoGrid; x++) { //percorre as colunas da matriz
            scanf("%c", &labirinto[y][x]); //le e atribui os caracteres a cada ponto (X ou 0)
            volumeSala[y][x] = dimensaoGrid * dimensaoGrid; //inicializa cada ponto com a distancia sendo n² (infinita)
        }
        getchar();
    }

    buscaEmLargura_labirinto(labirinto, volumeSala, dimensaoGrid);

    for (int y = 0; y < dimensaoGrid; y++) {
        for (int x = 0; x < dimensaoGrid; x++) {
            printf("%d ", volumeSala[y][x]);
        } printf("\n");
    }

    for (int i = 0; i < dimensaoGrid; i++) { //for para liberar a memória da heap
        free(labirinto[i]); //libera memoria das linhas do labirinto
        free(volumeSala[i]); //libera memoria das linhas do volume das salas
    }
    free(labirinto); //libera a "cabeca" do labirinto
    free(volumeSala); //libera a "cabeca" do volume das salas
    return 0;
}

fila criar_fila(int tamanhoFila) {
    fila filaLabirinto = malloc(sizeof(struct fila)); //aloca na heap memoria para uma struct da fila
    filaLabirinto->pontos = malloc(sizeof(ponto) * tamanhoFila); //aloca na heap um vetor para os pontos do labirinto
    filaLabirinto->inicio = 0; //inicializa inicio da fila
    filaLabirinto->final = 0; //inicializa final da fila
    return filaLabirinto;
}

void inserir_fila(fila filaLabirinto, ponto p, int tamanhoFila) {
    if ((filaLabirinto->final+1) % (tamanhoFila) == filaLabirinto->inicio) { //verifica se a fila ta cheia
        return;
    }
    filaLabirinto->pontos[filaLabirinto->final] = p; //se nao estiver vazia, coloco o ponto no final dela
    filaLabirinto->final = filaLabirinto->final + 1;
    if (filaLabirinto->final == tamanhoFila) {
        filaLabirinto->final = 0; 
    }
}

ponto remover_fila(fila filaLabirinto, int tamanhoFila) {
    ponto p = filaLabirinto->pontos[filaLabirinto->inicio]; //pega o ponto da primeira posicao da fila para remover (FIFO)
    if (filaLabirinto->inicio == filaLabirinto->final) { //verifica se a fila ta vazia, se sim, retorna o proprio ponto
        return p;
    }
    p = filaLabirinto->pontos[filaLabirinto->inicio];
    filaLabirinto->inicio = filaLabirinto->inicio + 1;
    if (filaLabirinto->inicio == tamanhoFila) {
        filaLabirinto->inicio = 0;
    }
    return p;
}

int fila_vazia(fila filaLabirinto) {
    if (filaLabirinto->inicio == filaLabirinto->final) {
        return 1; //retorna 1 se a fila estiver vazia
    }
    return 0; //retorna 0 se a fila nn estiver vazia
}

void liberar_fila(fila filaLabirinto) {
    free(filaLabirinto->pontos); //libera memoria dos pontos do labirinto
    free(filaLabirinto); //libera memoria da cabeca da fila
}

void buscaEmLargura_labirinto(char **labirinto, int **volumeSala, int dimensaoGrid) {
    fila filaLabirinto = criar_fila(dimensaoGrid * dimensaoGrid); //cria uma fila como n², para suportar todos os pontos (matriz)
    ponto inicio = {0,0}; //ponto inicial eh sempre (0,0)

    inserir_fila(filaLabirinto, inicio, dimensaoGrid * dimensaoGrid); //coloca o ponto de inicio na fila
    volumeSala[0][0] = 0; //distancia/volume do ponto inicial (0,0) eh sempre 0
    ponto movimentos[4] = {{-1,0}, {1,0}, {0,-1}, {0,1}}; //array com todos os movimentos possiveis de um ponto (excete diagonais)
    
    while (!fila_vazia(filaLabirinto)) { //itera enquanto a fila estiver pontos ainda
        ponto pontoAtual = remover_fila(filaLabirinto, dimensaoGrid * dimensaoGrid); //pega o prox ponto da fila para tirar (FIFO)
        for (int i = 0; i < 4; i++) { //for para percorrer os 4 pontos vizinhos do ponto atual
            int proximoX = pontoAtual.x + movimentos[i].x;
            int proximoY = pontoAtual.y + movimentos[i].y;
            if ((proximoX >= 0 && proximoX < dimensaoGrid) && (proximoY >= 0 && proximoY < dimensaoGrid)) { //verifica se o ponto vizinho está dentro do limite da matriz
                if (labirinto[proximoY][proximoX] != 'X' && volumeSala[proximoY][proximoX] > volumeSala[pontoAtual.y][pontoAtual.x]+1) { //verifica se o ponto vizinho nn é uma sala bloqueada
                    volumeSala[proximoY][proximoX] = volumeSala[pontoAtual.y][pontoAtual.x]+1; //muda o valor do ponto vizinho para a menor distancia 
                    ponto proximoPonto = {proximoX, proximoY}; //cria o ponto vizinho
                    inserir_fila(filaLabirinto, proximoPonto, dimensaoGrid * dimensaoGrid); //insere o ponto vizinho na fila
                }
            }
        }
    }
    liberar_fila(filaLabirinto); //libera a memoria na heap da fila dps de fazer a busca em largura
}