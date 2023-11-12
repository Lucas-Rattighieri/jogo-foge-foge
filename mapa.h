# include <stdio.h>
# include <stdlib.h>
# include <conio.h>


typedef struct {
    char ** matriz;
    int linhas;
    int colunas;
    int x;
    int y;
} Mapa;



// Funções
int mod(int num, int N) {
    if (num >= 0) 
        return num % N;
    else {
        return N + (num % N);
    }
}


void declara_mapa(Mapa * mapa) {
    mapa->matriz = (char **) malloc(sizeof(char *) * mapa->linhas);

    for (int i = 0; i < mapa->linhas; i++) {
        mapa->matriz[i] = (char *) malloc(sizeof(char) * (mapa->colunas + 1));
    }    
}


void encontrar_posicao(Mapa * mapa) {
    for (int i = 0; i < mapa->linhas; i++) {
        for (int j = 0; j < mapa->colunas; j++) {
            if (mapa->matriz[i][j] == '@') {
                mapa->x = j;
                mapa->y = i;
                break;
            }
        }
    }
}


void ler_mapa(Mapa * mapa) {
    FILE *f;

    f = fopen("mapa.txt", "r");

    if (f == NULL) {
        printf("Erro na leitura do mapa\n");
        exit(1);
    }
    
    fscanf(f, "%d %d", &(mapa->linhas), &(mapa->colunas));
    
    declara_mapa(mapa);

    for (int i = 0; i < 5; i++)
        fscanf(f, "%s", mapa->matriz[i]);
    
    fclose(f);

    encontrar_posicao(mapa);
}


void liberar_mapa(Mapa * mapa) {
    for (int i = 0; i < mapa->linhas; i++) {
        free(mapa->matriz[i]);
    }
    free(mapa->matriz);
}


void imprimir_mapa(Mapa * mapa) {
    printf("\n");

    for (int i = 0; i < mapa->linhas; i++) {
        printf("%s\n", mapa->matriz[i]);
    }
}


int mover(Mapa * mapa, char direcao) {
    
    mapa->matriz[mapa->y][mapa->x] = '.';
    switch (direcao) {
        case 'a':
            mapa->matriz[mapa->y][mod(mapa->x - 1, mapa->colunas)] = '@';
            mapa->x = mod(mapa->x - 1, mapa->colunas);
            break;
        case 'w':
            mapa->matriz[mod(mapa->y-1, mapa->linhas)][mapa->x] = '@';
            mapa->y = mod(mapa->y-1, mapa->linhas);
            break;
        case 'd':
            mapa->matriz[mapa->y][mod(mapa->x+1, mapa->colunas)] = '@';
            mapa->x = mod(mapa->x+1, mapa->colunas);
            break;
        case 's':
            mapa->matriz[mod(mapa->y+1, mapa->linhas)][mapa->x] = '@';
            mapa->y = mod(mapa->y+1, mapa->linhas);
            break;
        default:
            mapa->matriz[mapa->y][mapa->x] = '@';

            if (direcao == 27)
                exit(0);
    }
    
}


int jogo_acabou() {


    return 0;
}