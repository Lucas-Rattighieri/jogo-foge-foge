#include "mapa.h"

# define ESPERA 0.95

Mapa mapa_jogo;
Heroi heroi_jogo;
Fantasmas fantasmas_jogo;



int main() {

    char comando;
    int pegou_bomba = 0;
    int num_fantasmas_inativos = 0;
    int termino_jogo;
    int passos_heroi = 0;

    iniciar_heroi(&heroi_jogo, HEROI, 0, 0);

    
    ler_mapa(&mapa_jogo, &heroi_jogo, &fantasmas_jogo);


    imprimir_mapa(&mapa_jogo, passos_heroi);
    sleep(ESPERA); 

    do {

        mover_fantasmas(&mapa_jogo, &heroi_jogo, &fantasmas_jogo, pegou_bomba);

        imprimir_mapa(&mapa_jogo, passos_heroi);
        sleep(ESPERA);

        if (!heroi_inativo(&heroi_jogo)) {
            printf("\n\nInforme o movimento: ");
            comando = getche();
            printf("\n");
        }

        mover_heroi(&mapa_jogo, &heroi_jogo, &fantasmas_jogo, comando, &pegou_bomba, &passos_heroi);

        imprimir_mapa(&mapa_jogo, passos_heroi);
        sleep(ESPERA);
        
        num_fantasmas_inativos += comeu_fantasma(&heroi_jogo, &fantasmas_jogo, &pegou_bomba, &passos_heroi);

        termino_jogo = jogo_acabou(&mapa_jogo, &heroi_jogo, &fantasmas_jogo, &num_fantasmas_inativos);

    } while (!termino_jogo);

    imprimir_mapa(&mapa_jogo, passos_heroi);

    if (termino_jogo == 1)
        printf("\nParabens, voce ganhou!\n\n");
    else if (termino_jogo == 2)
        printf("\nQue pena, voce perdeu!\n\n");

    liberar_mapa(&mapa_jogo);
    liberar_fantasmas(&fantasmas_jogo);
}