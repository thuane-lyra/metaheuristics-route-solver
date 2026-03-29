#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "uteis.h"

void init_rand() { srand((unsigned)time(NULL)); }
int random_l_h(int min, int max) { return min + rand() % (max - min + 1); }
float rand_01() { return ((float)rand()) / RAND_MAX; }
int flip() { return (rand_01() < 0.5) ? 0 : 1; }

void printSol(pchrom sol, int numCand) {
    printf("Fitness: %.4f | Sel: ", sol->fitness);
    int c=0;

    // Mostra os indices dos locais escolhidos
    printf("{ ");
    for(int i=0; i<numCand; i++) {
        if(sol->p[i]==1) {
            printf("%d ", i+1); // i+1 para ser o local 1, 2, etc.
            c++;
        }
    }
    printf("} (Total: %d)\n", c);
}
void copiaSolucao(pchrom destino, pchrom origem) {
    *destino = *origem;
}

void init_data(char *filename, struct info *d, float mat[][MAX_OBJ]) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("Erro ficheiro\n"); exit(1); }

    if (fscanf(f, "%d %d", &d->numCand, &d->numSel) != 2) exit(1);

    // Limpar matriz
    for(int i=0; i<d->numCand; i++)
        for(int j=0; j<d->numCand; j++)
            mat[i][j] = 0.0;

    char b1[10], b2[10];
    float dist;
    while(fscanf(f, "%s %s %f", b1, b2, &dist) == 3) {
        int id1 = atoi(&b1[1]) - 1;
        int id2 = atoi(&b2[1]) - 1;
        if(id1 >= 0 && id2 >= 0) {
            mat[id1][id2] = dist;
            mat[id2][id1] = dist;
        }
    }
    fclose(f);
}

// Gera populacao inicial valida (com m 1's)
pchrom init_pop(struct info d) {
    pchrom indiv = malloc(sizeof(chrom) * d.popsize);
    if (indiv == NULL) exit(1);
    for (int i = 0; i < d.popsize; i++) {
        for(int j=0; j<d.numCand; j++) indiv[i].p[j] = 0;
        int count = 0;
        while(count < d.numSel) {
            int r = random_l_h(0, d.numCand - 1);
            if(indiv[i].p[r] == 0) {
                indiv[i].p[r] = 1;
                count++;
            }
        }
        indiv[i].valido = 1;
    }
    return indiv;
}