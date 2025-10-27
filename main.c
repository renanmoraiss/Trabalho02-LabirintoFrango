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
void buscaPorLargura_labirinto(char **labirinto, int **volumeSala, int dimensaoGrid); //realiza a Busca em Largura na fila

int main() {
    int dimensaoGrid;
    fila filaLabirinto;
    scanf("%d\n", &dimensaoGrid);
    char **labirinto = malloc(sizeof(char*) * dimensaoGrid); //aloca na heap memoria para a matriz do labirinto
    int **volumeSala = malloc(sizeof(int*) * dimensaoGrid); //aloca na heap memoria para o volume de cada sala ate a saida
    for (int y = 0; y < dimensaoGrid; y++) {
        labirinto[y] = malloc(sizeof(char) * dimensaoGrid);
        volumeSala[y] = malloc(sizeof(int) * dimensaoGrid);
    }
    for (int y = 0; y < dimensaoGrid; y++) {
        for (int x = 0; x < dimensaoGrid; x++) {
            scanf("%c", &labirinto[y][x]);
            volumeSala[y][x] = dimensaoGrid * dimensaoGrid;
        }
        getchar();
    }
    buscaPorLargura_labirinto(labirinto, volumeSala, dimensaoGrid);
    for (int y = 0; y < dimensaoGrid; y++) {
        for (int x = 0; x < dimensaoGrid; x++) {
            printf("%d ", volumeSala[y][x]);
        } printf("\n");
    }
    for (int i = 0; i < dimensaoGrid; i++) {
        free(labirinto[i]);
        free(volumeSala[i]);
    }
    free(labirinto);
    free(volumeSala);
    return 0;
}

fila criar_fila(int tamanhoFila) {
    fila filaLabirinto = malloc(sizeof(struct fila));
    filaLabirinto->pontos = malloc(sizeof(ponto) * tamanhoFila);
    filaLabirinto->inicio = 0;
    filaLabirinto->final = 0;
    return filaLabirinto;
}

void inserir_fila(fila filaLabirinto, ponto p, int tamanhoFila) {
    if ((filaLabirinto->final+1) % (tamanhoFila) == filaLabirinto->inicio) {
        return;
    }
    filaLabirinto->pontos[filaLabirinto->final] = p;
    filaLabirinto->final = filaLabirinto->final + 1;
    if (filaLabirinto->final == tamanhoFila) {
        filaLabirinto->final = 0;
    }
}

ponto remover_fila(fila filaLabirinto, int tamanhoFila) {
    ponto p = filaLabirinto->pontos[filaLabirinto->inicio];
    if (filaLabirinto->inicio == filaLabirinto->final) {
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

void buscaPorLargura_labirinto(char **labirinto, int **volumeSala, int dimensaoGrid) {
    fila filaLabirinto = criar_fila(dimensaoGrid * dimensaoGrid);
    if (labirinto[0][0] != 'X') {
        ponto inicio = {0,0};
        inserir_fila(filaLabirinto, inicio, dimensaoGrid * dimensaoGrid);
        volumeSala[0][0] = 0;
    }
    int deslocamentoX[4] = {-1, 1, 0, 0};
    int deslocamentoY[4] = {0, 0, -1 , 1};
    while (!fila_vazia(filaLabirinto)) {
        ponto pontoAtual = remover_fila(filaLabirinto, dimensaoGrid * dimensaoGrid);
        for (int i = 0; i < 4; i++) {
            int proximoX = pontoAtual.x + deslocamentoX[i];
            int proximoY = pontoAtual.y + deslocamentoY[i];
            if ((proximoX >= 0 && proximoX < dimensaoGrid) && (proximoY >= 0 && proximoY < dimensaoGrid)) {
                if (labirinto[proximoY][proximoX] != 'X' && volumeSala[proximoY][proximoX] > volumeSala[pontoAtual.y][pontoAtual.x]+1) {
                    volumeSala[proximoY][proximoX] = volumeSala[pontoAtual.y][pontoAtual.x]+1;
                    ponto proximoPonto = {proximoX, proximoY};
                    inserir_fila(filaLabirinto, proximoPonto, dimensaoGrid * dimensaoGrid);
                }
            }
        }
    }
    liberar_fila(filaLabirinto);
}