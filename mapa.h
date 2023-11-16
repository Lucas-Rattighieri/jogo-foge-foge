# include <stdio.h>
# include <stdlib.h>
# include <conio.h>
# include <time.h>
# include <math.h>
# include "ui.h"

# define HEROI '@'
# define FANTASMA 'F'
# define BOMBA 'P'
# define PAREDE_VERTICAL '|'
# define PAREDE_HORIZONTAL '-'


typedef struct {
    int x;
    int y;
} Posicao;


typedef struct {
    char ** matriz;
    int linhas;
    int colunas;
} Mapa;


typedef struct {
    char aparencia;
    Posicao posicao;
} Heroi;


typedef struct {
    int num;
    char aparencia;
    Posicao * posicoes;
} Fantasmas;




void iniciar_heroi(Heroi * heroi, char aparencia, int x, int y) {
    heroi->aparencia = aparencia;
    heroi->posicao.x = x;
    heroi->posicao.y = y;
}


void iniciar_fantasmas(Fantasmas * fantasmas, int num_fantasmas, char aparencia) {
    fantasmas->num = num_fantasmas;
    fantasmas->aparencia = aparencia;

    fantasmas->posicoes = (Posicao *) malloc(sizeof(Posicao) * num_fantasmas);
}


void liberar_fantasmas(Fantasmas * fantasmas) {

    free(fantasmas->posicoes);
}



void declara_mapa(Mapa * mapa) {
    mapa->matriz = (char **) malloc(sizeof(char *) * mapa->linhas);

    for (int i = 0; i < mapa->linhas; i++) {
        mapa->matriz[i] = (char *) malloc(sizeof(char) * (mapa->colunas + 1));
    }    
}




int encontrar_heroi(Mapa * mapa, Heroi * heroi) {
    for (int i = 0; i < mapa->linhas; i++) {
        for (int j = 0; j < mapa->colunas; j++) {

            if (mapa->matriz[i][j] == heroi->aparencia) {
                heroi->posicao.x = j;
                heroi->posicao.y = i;
                return 1;
            }
        }
    }
    return 0;
}


void encontrar_fantasmas(Mapa * mapa, Fantasmas * fantasmas, Heroi * heroi) {
    int k = 0;
    for (int i = 0; i < mapa->linhas; i++) {
        for (int j = 0; j < mapa->colunas; j++) {

            if (mapa->matriz[i][j] == fantasmas->aparencia) {
                fantasmas->posicoes[k].x = j;
                fantasmas->posicoes[k].y = i;
                k++;
            }

            if (k == fantasmas->num)
                break;
        }
    }
}


void ler_mapa(Mapa * mapa, Heroi * heroi, Fantasmas * fantasmas) {
    FILE *f;
    int num_fantasmas;

    f = fopen("mapa.txt", "r");

    if (f == NULL) {
        printf("Erro na leitura do mapa\n");
        exit(1);
    }
    
    fscanf(f, "%d %d %d", &(mapa->linhas), &(mapa->colunas), &num_fantasmas);
    
    declara_mapa(mapa);

    for (int i = 0; i < mapa->linhas; i++)
        fscanf(f, "%s", mapa->matriz[i]);
    
    fclose(f);

    iniciar_fantasmas(fantasmas, num_fantasmas, FANTASMA);

    encontrar_heroi(mapa, heroi);
    encontrar_fantasmas(mapa, fantasmas, heroi);
}


void liberar_mapa(Mapa * mapa) {
    for (int i = 0; i < mapa->linhas; i++) {
        free(mapa->matriz[i]);
    }
    free(mapa->matriz);
}


void imprimeparte(char desenho[4][7], int parte) {
    printf("%s", desenho[parte]);
}


void imprimir_mapa(Mapa * mapa) {
    for(int i = 0; i < mapa->linhas; i++) {

        for(int parte = 0; parte < 4; parte++) {
            for(int j = 0; j < mapa->colunas; j++) {

                switch(mapa->matriz[i][j]) {
                    case FANTASMA:
                        imprimeparte(desenhofantasma, parte);
                        break;
                    case HEROI:
                        imprimeparte(desenhoheroi, parte);
                        break;
                    case BOMBA:
                        imprimeparte(desenhopilula, parte);
                        break;
                    case PAREDE_HORIZONTAL:
                    case PAREDE_VERTICAL:
                        imprimeparte(desenhoparede, parte);
                        break;
                    case '.':
                        imprimeparte(desenhovazio, parte);
                        break;
                }

            }
            printf("\n");
        }

    }
}


int posicao_vazio(Mapa * mapa, int x, int y) {
    if (mapa->matriz[y][x] != '.')
        return 0;
    else 
        return 1;
}


int posicao_fantasma(Mapa * mapa, Fantasmas * fantasmas, int x, int y) {
    if (mapa->matriz[y][x] != fantasmas->aparencia)
        return 0;
    else 
        return 1;
}


int posicao_heroi(Mapa * mapa, Heroi * heroi, int x, int y) {
    if (mapa->matriz[y][x] != heroi->aparencia)
        return 0;
    else 
        return 1;
}


int mover_heroi(Mapa * mapa, Heroi * heroi, char tecla) {
    
    int prox_x = heroi->posicao.x, prox_y = heroi->posicao.y;

	if(tecla == 'w' || tecla == 'W' || tecla == 72){
        prox_y--;
    }                    
    if(tecla == 'a' || tecla == 'A' || tecla == 75){
        prox_x--;
    }                    
    if(tecla == 's' || tecla == 'S' || tecla == 80){
        prox_y++;
    }                     
    if(tecla == 'd' || tecla == 'D' || tecla == 77){
        prox_x++;
    } 


    if (posicao_vazio(mapa, prox_x, prox_y)) {
        mapa->matriz[heroi->posicao.y][heroi->posicao.x] = '.';
        mapa->matriz[prox_y][prox_x] = heroi->aparencia;
        heroi->posicao.x = prox_x;
        heroi->posicao.y = prox_y;
    }
    
}


float distancia(Posicao posicao1, Posicao posicao2) {
    float dist; 

    return (float) sqrt(pow(posicao1.x - posicao2.x, 2) + pow(posicao1.y - posicao2.y, 2));
}


int mover_fantasmas(Mapa * mapa, Heroi * heroi, Fantasmas * fantasmas) {
    
    int candidato_x, candidato_y, prox_x, prox_y;
    int pos_x, pos_y, mover;
    Posicao movimetacoes[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    float distancia_atual, nova_distancia;
    

    srand(time(NULL));

    for (int i = 0; i < fantasmas->num; i++) {
        pos_x = fantasmas->posicoes[i].x;
        pos_y = fantasmas->posicoes[i].y;

        prox_x = pos_x;
        prox_y = pos_y;

        distancia_atual = distancia(heroi->posicao, fantasmas->posicoes[i]);

        for (int j = 0; j < 4; j++)  {
            candidato_x = pos_x + movimetacoes[j].x;
            candidato_y =  pos_y + movimetacoes[j].y;

            Posicao nova_posicao = {candidato_x, candidato_y};

            nova_distancia = distancia(heroi->posicao, nova_posicao);
            
            if (posicao_vazio(mapa, candidato_x, candidato_y) || 
                posicao_heroi(mapa, heroi, candidato_x, candidato_y)) {

                if (distancia_atual > nova_distancia) {
                    prox_x = candidato_x;
                    prox_y = candidato_y;
                    distancia_atual = nova_distancia;
                }
                else if (prox_x == pos_x && prox_y == pos_y) {
                    prox_x = candidato_x;
                    prox_y = candidato_y;
                }
            }
        } 
                

        mapa->matriz[pos_y][pos_x] = '.';
        mapa->matriz[prox_y][prox_x] = fantasmas->aparencia;
        fantasmas->posicoes[i].x = prox_x;
        fantasmas->posicoes[i].y = prox_y;
    }
    
}


int jogo_acabou(Mapa * mapa, Heroi * heroi) {
    
    return !encontrar_heroi(mapa, heroi);
}