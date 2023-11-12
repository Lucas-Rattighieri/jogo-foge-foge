#include "mapa.h"

Mapa mapa_jogo;


int main() {

    char comando;
    
    ler_mapa(&mapa_jogo);

    do {
        imprimir_mapa(&mapa_jogo);

        comando = getche();

        mover(&mapa_jogo, comando);

    } while (!jogo_acabou());

    liberar_mapa(&mapa_jogo);
}