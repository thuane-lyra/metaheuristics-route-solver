#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo.h"
#include "uteis.h"
#include "funcao.h"

#define NUM_RUNS 10

int main() {
    struct info params;
    static float matriz[MAX_OBJ][MAX_OBJ];
    char nome_fich[100];
    int opcao;          // Variavel para o menu
    float mbf = 0.0;
    float best_run_val;
    chrom best_ever;
    best_ever.fitness = 0.0;

    init_rand();

    // Pede Ficheiro
    printf("Nome do ficheiro (ex: tourism_50.txt): ");
    scanf("%s", nome_fich);

    init_data(nome_fich, &params, matriz);
    printf("Dados carregados: C=%d m=%d\n", params.numCand, params.numSel);

    // Configura Parametros
    params.popsize = 200;
    params.pm = 0.1;
    params.pr = 0.7;
    params.tsize = 2;
    params.numGenerations = 2500; //evolutivo e hibrido

   // Menu de Opcoes
    printf("\n### MENU DE ALGORITMOS ###\n");
    printf("1 - Pesquisa Local (Trepa-Colinas)\n");
    printf("2 - Algoritmo Evolutivo\n");
    printf("3 - Hibrido 1 (Refina Populacao Inicial)\n");
    printf("4 - Hibrido 2 (Refina Melhor Final)\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    if(opcao < 1 || opcao > 4) {
        printf("Opcao invalida!\n"); return 1;
    }

    printf("\n>>> A INICIAR %d REPETICOES PARA A OPCAO %d <<<\n", NUM_RUNS, opcao);

    for(int r=0; r<NUM_RUNS; r++) {
        printf("\n=== Run %d ===\n", r+1);

        switch(opcao) {
            case 1: // Pesquisa Local
            {
                chrom sol;
                for(int i=0; i<params.numCand; i++) sol.p[i] = 0;
                int count = 0;
                while(count < params.numSel) {
                    int r = random_l_h(0, params.numCand - 1);
                    if(sol.p[r] == 0) { sol.p[r] = 1; count++; }
                }
                sol.valido = 1;
                sol.fitness = calcula_fitness(sol.p, params.numCand, params.numSel, matriz);

                best_run_val = trepa_colinas(sol.p, params, matriz, 10000);
                sol.fitness = best_run_val;
                printf("TC Best: "); printSol(&sol, params.numCand);
                break;
            }

            case 2: // Evolutivo
                best_run_val = algoritmo_evolutivo(params, matriz);
                break;

            case 3: // Hibrido 1
                best_run_val = algoritmo_hibrido_1(params, matriz);
                break;

            case 4: // Hibrido 2
                best_run_val = algoritmo_hibrido_2(params, matriz);
                break;
        }

        mbf += best_run_val;
        if(best_run_val > best_ever.fitness) best_ever.fitness = best_run_val;
    }

    // Resultados Finais
    printf("\n\n=== ESTATISTICAS FINAIS (%d Runs) ===\n", NUM_RUNS);
    printf("Algoritmo Escolhido: %d\n", opcao);
    printf("MBF (Media da Melhor Qualidade): %.4f\n", mbf / NUM_RUNS);
    printf("Melhor Solucao Absoluta Encontrada (BEST): %.4f\n", best_ever.fitness);

    return 0;
}