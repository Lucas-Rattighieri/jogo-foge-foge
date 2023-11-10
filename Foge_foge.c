# include <stdio.h>
# include <stdlib.h>

char ** mapa;
int linhas, colunas;

int main() {
    

    FILE *f;

    f = fopen("mapa.txt", "r");

    if (f == NULL) {
        printf("Erro na leitura do mapa\n");
        exit(1);
    }
    
    fscanf(f, "%d %d", &linhas, &colunas);
    
    mapa = (char **) malloc(sizeof(char *) * linhas);

    for (int i = 0; i < linhas; i++) {
        mapa[i] = (char *) malloc(sizeof(char) * (colunas + 1));
    }

    for (int i = 0; i < 5; i++)
        fscanf(f, "%s", mapa[i]);

    for (int i = 0; i < 5; i++)
        printf("%s\n", mapa[i]);

    for (int i = 0; i < linhas; i++) {
        free(mapa[i]);
    }
    free(mapa);
}