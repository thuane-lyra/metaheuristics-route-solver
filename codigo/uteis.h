
#ifndef INC_2024129510LUANARABELLO2024144204THUANELYRA_UTEIS_H
#define INC_2024129510LUANARABELLO2024144204THUANELYRA_UTEIS_H
#include "algoritmo.h"

void init_rand();
int random_l_h(int min, int max);
float rand_01();
int flip();

// Leitura de dados
void init_data(char *filename, struct info *d, float mat[][MAX_OBJ]);

// Auxiliares
pchrom init_pop(struct info d);
void printSol(pchrom sol, int numCand);
void copiaSolucao(pchrom destino, pchrom origem);
#endif //INC_2024129510LUANARABELLO2024144204THUANELYRA_UTEIS_H