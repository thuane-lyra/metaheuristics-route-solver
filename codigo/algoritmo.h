
#ifndef INC_2024129510LUANARABELLO2024144204THUANELYRA_ALGORITMO_H
#define INC_2024129510LUANARABELLO2024144204THUANELYRA_ALGORITMO_H
#define MAX_OBJ 500   // Numero maximo de candidaturas

// Estrutura de parametros
struct info {
    int     popsize;
    float   pm;             // Prob. Mutacao
    float   pr;             // Prob. Recombinacao
    int     tsize;          // Tamanho do torneio
    int     numCand;        // C (Candidaturas)
    int     numSel;         // m (Locais a escolher)
    int     numGenerations;
};

// Elemento
typedef struct individual {
    int     p[MAX_OBJ];  // Vetor binario
    float   fitness;     // Qualidade
    int     valido;      // 1 se valido
} chrom, *pchrom;

// ---(Evolutivo) ---
void tournament(pchrom pop, struct info d, pchrom parents);
void tournament_k(pchrom pop, struct info d, pchrom parents);
void genetic_operators(pchrom parents, struct info d, pchrom offspring);

// Operadores
void crossover(pchrom parents, struct info d, pchrom offspring);          // 1 Ponto
void crossover2(pchrom parents, struct info d, pchrom offspring);		  // 2 pontos
void crossover_uniforme(pchrom parents, struct info d, pchrom offspring); // Uniforme
void mutation(pchrom offspring, struct info d);                           // Binaria (Bit Flip)
void mutation2(pchrom offspring, struct info d);                          // Troca (Swap)

float algoritmo_evolutivo(struct info d, float mat[][MAX_OBJ]);

// --- (Pesquisa Local) ---
void gera_vizinho(int a[], int b[], int numCand);
void gera_vizinho2(int a[], int b[], int numCand);
float trepa_colinas(int sol[], struct info d, float mat[][MAX_OBJ], int numIter);

//HÍBRIDOS

// Abordagem 1: Refina a População Inicial (ANTES do Evolutivo)
float algoritmo_hibrido_1(struct info d, float mat[][MAX_OBJ]);

// Abordagem 2: Refina a Melhor Solução (DEPOIS do Evolutivo)
float algoritmo_hibrido_2(struct info d, float mat[][MAX_OBJ]);
#endif //INC_2024129510LUANARABELLO2024144204THUANELYRA_ALGORITMO_H