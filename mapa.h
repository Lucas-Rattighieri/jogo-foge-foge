# include <stdio.h>
# include <stdlib.h>
# include <conio.h>
# include <time.h>
# include <math.h>
#include <unistd.h>
# include "ui.h"

# define HEROI '@'
# define FANTASMA 'F'
# define BOMBA 'P'
# define PAREDE_VERTICAL '|'
# define PAREDE_HORIZONTAL '-'
# define INATIVO -1
# define PASSOS_COM_BOMBA 16


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


void imprimir_parte(char desenho[4][7], int parte) {
    printf("%s", desenho[parte]);
}


void imprimir_mapa(Mapa * mapa, int passo_heroi) {
    printf("\nPassos para comer os fantasmas: %d\n", passo_heroi);

    for(int i = 0; i < mapa->linhas; i++) {

        for(int parte = 0; parte < 4; parte++) {
            for(int j = 0; j < mapa->colunas; j++) {

                switch(mapa->matriz[i][j]) {
                    case FANTASMA:
                        imprimir_parte(desenhofantasma, parte);
                        break;
                    case HEROI:
                        imprimir_parte(desenhoheroi, parte);
                        break;
                    case BOMBA:
                        imprimir_parte(desenhopilula, parte);
                        break;
                    case PAREDE_HORIZONTAL:
                    case PAREDE_VERTICAL:
                        imprimir_parte(desenhoparede, parte);
                        break;
                    case '.':
                        imprimir_parte(desenhovazio, parte);
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

int posicao_parede(Mapa * mapa, int x, int y) {
    if (mapa->matriz[y][x] == PAREDE_HORIZONTAL || mapa->matriz[y][x] == PAREDE_VERTICAL)
        return 1;
    else 
        return 0;
}


int posicao_fantasma(Fantasmas * fantasmas, int x, int y) {
    for (int i = 0; i < fantasmas->num; i++)
        if (fantasmas->posicoes[i].x == x && fantasmas->posicoes[i].y == y) {
            return 1;
        }
    return 0;
            

}

int posicao_bomba(Mapa * mapa, int x, int y) {
    if (mapa->matriz[y][x] == BOMBA)
        return 1;
    else 
        return 0;
}


int posicao_heroi(Heroi * heroi, int x, int y) {
    if (heroi->posicao.x == x && heroi->posicao.y == y)
        return 1;
    else 
        return 0;
}

void escolha_tecla(char tecla, int * pos_x, int * pos_y) {
	if(tecla == 'w' || tecla == 'W') {
        (*pos_y)--;
    }                    
    if(tecla == 'a' || tecla == 'A') {
        (*pos_x)--;
    }                    
    if(tecla == 's' || tecla == 'S') {
        (*pos_y)++;
    }                     
    if(tecla == 'd' || tecla == 'D') {
        (*pos_x)++;
    } 
}


int mover_heroi(Mapa * mapa, Heroi * heroi, Fantasmas * fantasmas, char tecla, int * pegou_bomba, int * passos_heroi) {
    
    int ha_bomba = 0, ha_fantasma = 0;
    int prox_x = heroi->posicao.x, prox_y = heroi->posicao.y;

    if (heroi->posicao.x == INATIVO) 
        return 0;


    escolha_tecla(tecla, &prox_x, &prox_y);

    ha_bomba = posicao_bomba(mapa, prox_x, prox_y);
    ha_fantasma = posicao_fantasma(fantasmas, prox_x, prox_y);
    

    if (!posicao_parede(mapa, prox_x, prox_y)) {
        
        mapa->matriz[heroi->posicao.y][heroi->posicao.x] = '.';

        if (ha_fantasma) {
            if (*pegou_bomba) {
                mapa->matriz[prox_y][prox_x] = heroi->aparencia;
            }
            else {
                mapa->matriz[prox_y][prox_x] = fantasmas->aparencia;
            }
        }
        else {
            mapa->matriz[prox_y][prox_x] = heroi->aparencia;


            if (ha_bomba) {
                (*pegou_bomba) = 1;
                (*passos_heroi) = PASSOS_COM_BOMBA;
            }
        }
        heroi->posicao.x = prox_x;
        heroi->posicao.y = prox_y;        
        printf("\n%d %d\n", heroi->posicao.x, heroi->posicao.y);
        
    }
    
}

int comeu_fantasma(Heroi * heroi, Fantasmas * fantasmas, int * pegou_bomba, int * passos_heroi) {

    if (posicao_fantasma(fantasmas, heroi->posicao.x, heroi->posicao.y)) {
        if (*pegou_bomba) {
            for (int i = 0; i < fantasmas->num; i++) {
                if (fantasmas->posicoes[i].x == heroi->posicao.x && fantasmas->posicoes[i].y == heroi->posicao.y) {
                    fantasmas->posicoes[i].x = INATIVO;
                    fantasmas->posicoes[i].y = INATIVO;

                    return 1;
                }
            }
        }
        else {
            heroi->posicao.x = INATIVO;
            heroi->posicao.y = INATIVO;
            return 0;
        }
    }

    if (*pegou_bomba) {
        (*passos_heroi)--;
    }
    if ((*passos_heroi <= 0)) {
        *passos_heroi = 0;
        *pegou_bomba = 0;
    }
    return 0;
}


int heroi_inativo(Heroi * heroi) {
    if (heroi->posicao.x == INATIVO) {
        return 1;
    }
    else 
        return 0;
}


float distancia(Posicao posicao1, Posicao posicao2) {
    float dist; 

    return (float) sqrt(pow(posicao1.x - posicao2.x, 2) + pow(posicao1.y - posicao2.y, 2));
}


int mover_fantasmas(Mapa * mapa, Heroi * heroi, Fantasmas * fantasmas, int pegou_bomba) {
    
    int candidato_x, candidato_y, prox_x, prox_y;
    int pos_x, pos_y, mover;
    Posicao movimetacoes[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    float distancia_atual, nova_distancia;
    
    srand(time(NULL));

    for (int i = 0; i < fantasmas->num; i++) {
        pos_x = fantasmas->posicoes[i].x;
        pos_y = fantasmas->posicoes[i].y;

        if (pos_x == INATIVO) {
            continue;
        }
            

        prox_x = pos_x;
        prox_y = pos_y;

        distancia_atual = distancia(heroi->posicao, fantasmas->posicoes[i]);

        for (int j = 0; j < 4; j++)  {
            candidato_x = pos_x + movimetacoes[j].x;
            candidato_y =  pos_y + movimetacoes[j].y;

            Posicao nova_posicao = {candidato_x, candidato_y};

            nova_distancia = distancia(heroi->posicao, nova_posicao);
            
            if (posicao_vazio(mapa, candidato_x, candidato_y) || 
                posicao_heroi(heroi, candidato_x, candidato_y)) {

                if (pow(-1, pegou_bomba) * distancia_atual > pow(-1, pegou_bomba) * nova_distancia) {
                    prox_x = candidato_x;
                    prox_y = candidato_y;
                    distancia_atual = nova_distancia;
                }
                else if (prox_x == pos_x && prox_y == pos_y) {
                    prox_x = candidato_x;
                    prox_y = candidato_y;
                }
                if (posicao_heroi(heroi, prox_x, prox_y)) {
                    heroi->posicao.x = INATIVO;
                    heroi->posicao.y = INATIVO;
                }
            }
        }            

        mapa->matriz[pos_y][pos_x] = '.';
        mapa->matriz[prox_y][prox_x] = fantasmas->aparencia;
        fantasmas->posicoes[i].x = prox_x;
        fantasmas->posicoes[i].y = prox_y;
    }
    
}


int jogo_acabou(Mapa * mapa, Heroi * heroi, Fantasmas * fantasmas, int * num_inativos) {
    if (*num_inativos == fantasmas->num)
        return 1;
    else if (heroi->posicao.x == INATIVO)
        return 2;
    return 0;
}