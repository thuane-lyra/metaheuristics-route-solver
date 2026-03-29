#include "funcao.h"
#include "uteis.h"

// Garante que a solucao tem exatamente m locais
void reparacao1(pchrom indiv, struct info d) {
    int count = 0;
    for (int i = 0; i < d.numCand; i++) if (indiv->p[i] == 1) count++;

    // Remove excesso
    while (count > d.numSel) {
        int r = random_l_h(0, d.numCand - 1);
        if (indiv->p[r] == 1) { indiv->p[r] = 0; count--; }
    }
    // Adiciona falta
    while (count < d.numSel) {
        int r = random_l_h(0, d.numCand - 1);
        if (indiv->p[r] == 0) { indiv->p[r] = 1; count++; }
    }
    indiv->valido = 1;
}

float calcula_fitness(int sol[], int numCand, int m, float mat[][MAX_OBJ]) {
    float soma = 0.0;
    int count = 0;
    for(int i=0; i<numCand; i++) if(sol[i] == 1) count++;
    //penalizacao
    if(count != m) return 0.0;

    for(int i=0; i<numCand; i++) {
        if(sol[i] == 1) {
            for(int j=i+1; j<numCand; j++) {
                if(sol[j] == 1) soma += mat[i][j];
            }
        }
    }
    return soma / (float)m;
}

void evaluate(pchrom pop, struct info d, float mat[][MAX_OBJ]) {
    for(int i=0; i<d.popsize; i++) {

        // Se a reparação estiver ativa, ela corrige e mete valido=1
        reparacao1(&pop[i], d);

        // conta quantos 1s tem o vetor
        int count = 0;
        for(int k=0; k<d.numCand; k++) {
            if (pop[i].p[k] == 1) count++;
        }

        // Se a conta estiver errada, MARCA COMO INVÁLIDO
        if (count != d.numSel) {
            pop[i].valido = 0;
            pop[i].fitness = 0.0;   // Penalização
        } else {
            pop[i].valido = 1;
            pop[i].fitness = calcula_fitness(pop[i].p, d.numCand, d.numSel, mat);
        }
    }
}