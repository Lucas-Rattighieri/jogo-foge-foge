#include "mapa.h"

Mapa mapa_jogo;
Heroi heroi_jogo;
Fantasmas fantasmas_jogo;


int main() {

    char comando;

    iniciar_heroi(&heroi_jogo, HEROI, 0, 0);

    
    ler_mapa(&mapa_jogo, &heroi_jogo, &fantasmas_jogo);

    do {
        imprimir_mapa(&mapa_jogo);

        comando = getche();

        mover_heroi(&mapa_jogo, &heroi_jogo, comando);
        mover_fantasmas(&mapa_jogo, &heroi_jogo, &fantasmas_jogo);

    } while (!jogo_acabou(&mapa_jogo, &heroi_jogo));

    imprimir_mapa(&mapa_jogo);
    printf("\nQue pena, voce perdeu!\n\n");

    liberar_mapa(&mapa_jogo);
    liberar_fantasmas(&fantasmas_jogo);
}