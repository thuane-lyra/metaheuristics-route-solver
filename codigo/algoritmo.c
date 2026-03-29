#include <stdlib.h>
#include <stdio.h>
#include "algoritmo.h"
#include "uteis.h"
#include "funcao.h"

// --- PESQUISA LOCAL ---

// Vizinhanca 1: Troca simples
void gera_vizinho(int a[], int b[], int numCand) {
    int p1, p2;
    // Copia a -> b
    for(int i=0; i<numCand; i++) b[i] = a[i];

    do { p1 = random_l_h(0, numCand - 1); } while (b[p1] != 0); //procura um zero
    do { p2 = random_l_h(0, numCand - 1); } while (b[p2] != 1); //procura um um
    b[p1] = 1; b[p2] = 0;
}

// Vizinhanca 2: Troca dupla
void gera_vizinho2(int a[], int b[], int numCand) {
    int p1, p2, p3, p4;
    // Copia a -> b
    for(int i=0; i<numCand; i++) b[i] = a[i];

    do { p1 = random_l_h(0, numCand-1); } while (b[p1] != 0);
    do { p2 = random_l_h(0, numCand-1); } while (b[p2] != 1);
    do { p3 = random_l_h(0, numCand-1); } while (b[p3] != 0 || p3==p1);
    do { p4 = random_l_h(0, numCand-1); } while (b[p4] != 1 || p4==p2);
    b[p1] = 1; b[p2] = 0; b[p3] = 1; b[p4] = 0;
}

// Trepa Colinas
float trepa_colinas(int sol[], struct info d, float mat[][MAX_OBJ], int numIter) {
    int *nova_sol;
    float custo, custo_viz;

    // Alocacao dinamica
    nova_sol = malloc(sizeof(int) * d.numCand);
    if(nova_sol == NULL) { printf("Erro memoria"); exit(1); }

    // Avalia inicial
    custo = calcula_fitness(sol, d.numCand, d.numSel, mat);

    for(int i=0; i<numIter; i++) {
        // Gera vizinho
        // gera_vizinho(sol, nova_sol, d.numCand);
        gera_vizinho2(sol, nova_sol, d.numCand);

        // Avalia vizinho
        custo_viz = calcula_fitness(nova_sol, d.numCand, d.numSel, mat);


        // Aceita se for Melhor
        if(custo_viz > custo) {
            // Substitui
            for(int k=0; k<d.numCand; k++) sol[k] = nova_sol[k];
            custo = custo_viz;
        }
    }
    free(nova_sol); // Liberta memoria
    return custo; // Retorna o valor float
}

// --- EVOLUTIVO  ---

// Selecao dos pais: Torneio
// Escolhe sempre 2 indivíduos e ganha o melhor
void tournament(pchrom pop, struct info d, pchrom parents) {
    int x1, x2;
    for(int i=0; i<d.popsize; i++) {
        x1 = random_l_h(0, d.popsize-1);
        do { x2 = random_l_h(0, d.popsize-1); } while(x1==x2);// Garante oponentes diferentes
        if(pop[x1].fitness > pop[x2].fitness) parents[i] = pop[x1];// O melhor ganha
        else parents[i] = pop[x2];
    }
}

// METODO 2: Torneio K
// Escolhe 'd.tsize' indivíduos e ganha o melhor deles todos
void tournament_k(pchrom pop, struct info d, pchrom parents) {
    int best_idx, candidate_idx;

    // Temos de preencher todinho o vetor parents
    for(int i=0; i<d.popsize; i++) {

        // 1. Escolhe o primeiro candidato provisório
        best_idx = random_l_h(0, d.popsize-1);

        // 2. Compara com outros (tsize-1) candidatos
        for(int j=1; j<d.tsize; j++) {
            candidate_idx = random_l_h(0, d.popsize-1);

            // Se encontrarmos um melhor, ele passa a ser o vencedor provisório
            if(pop[candidate_idx].fitness > pop[best_idx].fitness) {
                best_idx = candidate_idx;
            }
        }
        // O vencedor do torneio vai para a lista de pais
        parents[i] = pop[best_idx];
    }
}



// Crossover 1: Um ponto de corte
void crossover(pchrom parents, struct info d, pchrom offspring) {
    int point;
    for (int i=0; i<d.popsize; i+=2) {
        if (rand_01() < d.pr) {
            point = random_l_h(0, d.numCand - 1);
            for (int j=0; j<point; j++) {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i+1].p[j] = parents[i+1].p[j];
            }
            for (int j=point; j<d.numCand; j++) {
                offspring[i].p[j] = parents[i+1].p[j];
                offspring[i+1].p[j] = parents[i].p[j];
            }
        } else {
            offspring[i] = parents[i]; offspring[i+1] = parents[i+1];
        }
    }
}
// Crossover 2 Pontos
void crossover2(pchrom parents, struct info d, pchrom offspring) {
    int point1, point2, aux;
    for (int i=0; i<d.popsize; i+=2) {
        if (rand_01() < d.pr) {
            point1 = random_l_h(0, d.numCand-1);
            do { point2 = random_l_h(0, d.numCand-1); } while(point1==point2);
            if (point1 > point2) { aux=point1; point1=point2; point2=aux; }

            for (int j=0; j<point1; j++) {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i+1].p[j] = parents[i+1].p[j];
            }
            for (int j=point1; j<point2; j++) { // Troca
                offspring[i].p[j] = parents[i+1].p[j];
                offspring[i+1].p[j] = parents[i].p[j];
            }
            for (int j=point2; j<d.numCand; j++) {
                offspring[i].p[j]= parents[i].p[j];
                offspring[i+1].p[j] = parents[i+1].p[j];
            }
        } else {
            offspring[i] = parents[i]; offspring[i+1] = parents[i+1];
        }
    }
}
// Crossover Uniforme
void crossover_uniforme(pchrom parents, struct info d, pchrom offspring) {
    for(int i=0; i<d.popsize; i+=2) {
        if(rand_01() < d.pr) {
            for(int j=0; j<d.numCand; j++) {
                if(flip()) {
                    offspring[i].p[j] = parents[i].p[j];
                    offspring[i+1].p[j] = parents[i+1].p[j];
                } else {
                    offspring[i].p[j] = parents[i+1].p[j];
                    offspring[i+1].p[j] = parents[i].p[j];
                }
            }
        } else {
            offspring[i] = parents[i]; offspring[i+1] = parents[i+1];
        }
    }
}

// Mutacao 1: Binaria - EXIGE REPARACAO1
void mutation(pchrom offspring, struct info d) {
    for (int i=0; i<d.popsize; i++)
        for (int j=0; j<d.numCand; j++)
            if (rand_01() < d.pm)
                offspring[i].p[j] = !(offspring[i].p[j]);
}

// Mutacao 2: Troca
void mutation2(pchrom offspring, struct info d) {
    for(int i=0; i<d.popsize; i++) {
        if(rand_01() < d.pm) {
            // Verificar se é possível fazer troca (tem de haver pelo menos um 0 e um 1)
            int tem_zero = 0;
            int tem_um = 0;
            for(int k=0; k<d.numCand; k++) {
                if(offspring[i].p[k] == 0) tem_zero = 1;
                else tem_um = 1;
            }
            // Se for tudo 0s ou tudo 1s, salta esta mutação (não dá para trocar)
            if(tem_zero == 0 || tem_um == 0) continue;
            //p1==0
            int p1, p2;
            do { p1 = random_l_h(0, d.numCand-1); } while(offspring[i].p[p1]!=0);
            do { p2 = random_l_h(0, d.numCand-1); } while(offspring[i].p[p2]!=1);
            offspring[i].p[p1] = 1;
            offspring[i].p[p2] = 0;
        }
    }
}

void genetic_operators(pchrom parents, struct info d, pchrom offspring) {
    // RECOMBINACAO (Escolher 1)
     //crossover(parents, d, offspring);
    crossover2(parents, d, offspring);
     //crossover_uniforme(parents, d, offspring);

    // MUTACAO (Escolher 1)
     mutation(offspring, d);
    //mutation2(offspring, d);


}

float algoritmo_evolutivo(struct info d, float mat[][MAX_OBJ]) {
    pchrom pop = init_pop(d);
    pchrom parents = malloc(sizeof(chrom) * d.popsize);
    evaluate(pop, d, mat);

    chrom best = pop[0];
    int gen = 0, inv = 0;

    // Inicializa best com o melhor da população inicial
    for(int i=0; i<d.popsize; i++) if(pop[i].fitness > best.fitness) best = pop[i];

    while (gen < d.numGenerations) {
        tournament(pop, d, parents);
        // tournament_k(pop, d, parents);
        genetic_operators(parents, d, pop);
        evaluate(pop, d, mat);

        inv = 0;
        for(int i=0; i<d.popsize; i++) {
            if(pop[i].valido == 0) inv++;
            if(pop[i].fitness > best.fitness) best = pop[i];
        }
        gen++;
    }

    printf("Evolutivo Best: "); printSol(&best, d.numCand);
    printf("Percentagem Invalidos: %.1f%%\n", (float)inv/d.popsize*100.0);

    float resultado_final = best.fitness; // Guarda valor para retornar
    free(parents);
    free(pop);

    return resultado_final; // Devolve ao main
}


// HÍBRIDO 1 - Refinamento da População Inicial
float algoritmo_hibrido_1(struct info d, float mat[][MAX_OBJ]) {
    pchrom pop = init_pop(d);
    pchrom parents = malloc(sizeof(chrom) * d.popsize);

    // Em vez de avaliar logo, vamos melhorar TODOS os indivíduos iniciais

    for(int i=0; i<d.popsize; i++) {
        // Chamamos o Trepa-Colinas para cada individuo.
        // Usamos menos iterações (ex: 100) para não ser demasiado lento.
        // Passamos pop[i].p (o vetor int) para a função purista.
        float fit_melhorado = trepa_colinas(pop[i].p, d, mat, 100);

        // Atualizamos o fitness na struct
        pop[i].fitness = fit_melhorado;
        // Como o trepa-colinas mantem a validade, garantimos que esta valido
        pop[i].valido = 1;
    }

    chrom best = pop[0];
    int gen = 0, inv = 0;

    // Encontrar o melhor inicial
    for(int i=0; i<d.popsize; i++)
        if(pop[i].fitness > best.fitness) best = pop[i];

    // O resto é o Evolutivo Normal
    while (gen < d.numGenerations) {
        tournament(pop, d, parents);
        genetic_operators(parents, d, pop);
        evaluate(pop, d, mat); // repara e avalia os filhos

        inv = 0;
        for(int i=0; i<d.popsize; i++) {
            if(pop[i].valido == 0) inv++;
            if(pop[i].fitness > best.fitness) best = pop[i];
        }
        gen++;
    }

    printf("Hibrido 1 Best: "); printSol(&best, d.numCand);
    printf("Percentagem Invalidos: %.1f%%\n", (float)inv/d.popsize*100.0);

    float resultado_final = best.fitness;
    free(parents); free(pop);
    return resultado_final;
}


// HÍBRIDO - ABORDAGEM 2 (PÓS-EVOLUÇÃO)
float algoritmo_hibrido_2(struct info d, float mat[][MAX_OBJ]) {
    pchrom pop = init_pop(d);
    pchrom parents = malloc(sizeof(chrom) * d.popsize);
    evaluate(pop, d, mat);

    chrom best = pop[0];
    int gen = 0, inv = 0;

    // Inicializa best
    for(int i=0; i<d.popsize; i++)
        if(pop[i].fitness > best.fitness) best = pop[i];

    // Corre o Evolutivo Completo
    while (gen < d.numGenerations) {
        tournament(pop, d, parents);
        genetic_operators(parents, d, pop);
        evaluate(pop, d, mat);

        inv = 0;
        for(int i=0; i<d.popsize; i++) {
            if(pop[i].valido == 0) inv++;
            if(pop[i].fitness > best.fitness) best = pop[i];
        }
        gen++;
    }

    // FASE HÍBRIDA: Refina a MELHOR solução final com Trepa-Colinas
    // O 'best' é um 'chrom', mas o 'trepa_colinas' precisa de 'int[]'
    // Passamos o vetor interno 'best.p'
    best.fitness = trepa_colinas(best.p, d, mat, 1000);

    printf("Hibrido 2 Best: "); printSol(&best, d.numCand);
    printf("Percentagem Invalidos: %.1f%%\n", (float)inv/d.popsize*100.0);

    float resultado_final = best.fitness;
    free(parents);
    free(pop);

    return resultado_final;
}