# Tourist Route Optimization - Meta-heuristics & Evolutionary Solver

![C](https://img.shields.io/badge/Language-C-00599C)
![AI](https://img.shields.io/badge/Field-AI%20Optimization-A61206)
![Meta-heuristics](https://img.shields.io/badge/Logic-Meta--heuristics-FF8C00)
![Excel](https://img.shields.io/badge/Results-Excel%20Analysis-217346?style=for-the-badge&logo=microsoft-excel&logoColor=white)

---

##  Project Overview
This project addresses the **Tourist Route Problem**, an optimization challenge aimed at selecting **m** locations from **C** candidates. The goal is to maximize the average geographic distance between selected points to promote sustainable tourism and visitor dispersion.

The solution's quality is evaluated using a fitness function that calculates the mean distance between all pairs of selected elements:

$$DM(P)=\frac{1}{m}\times\sum_{i=1}^{m-1}\sum_{j=i+1}^{m}dist(e_{i},e_{j})$$

##  Implemented Algorithms

### Local Search (Hill Climbing / Trepa-Colinas)
* **Strategy**: Uses a stochastic start and explores neighbors to find local optima.
* **Neighborhoods**: Implemented **Simple Swap** (1-bit exchange) and **Double Swap** (2-bit simultaneous exchange).
* **Optimization**: The algorithm operates strictly within the valid solution space, maintaining exactly **m** selections per iteration.

### Evolutionary Algorithm (EA)
* **Methodology**: Simulates natural selection through populations, crossovers, and mutations.
* **Selection**: Features both **Binary Tournament** and **K-Tournament** methods.
* **Crossover**: Includes **1-Point**, **2-Point**, and **Uniform** recombination.
* **Mutation**: Supports standard **Bit-flip** and a specialized **Swap Mutation** to maintain solution validity.
* **Repair Mechanism**: Automatically corrects offspring that do not meet the **m** selection constraint.

###  Hybrid Approaches
* **Hybrid 1**: Initial population refinement using Local Search before starting the evolution.
* **Hybrid 2**: Post-evolution refinement of the single best solution found by the EA.

###  Project Structure
* **`codigo/`**: Directory containing all C source code (`.c`) and header files (`.h`).
* **`excel/`**: Folder containing the complete experimental study and detailed statistical data analysis.
* **`Relatório-ThuaneLyra-LuanaRabello.pdf`**: Detailed technical report including the project's methodology, experimental analysis, and conclusions.

## Authors
* **Thuane Lyra** 
* **Luana Rabello** 


---
*Developed for the **Introduction to Artificial Intelligence** course at ISEC - Polytechnic of Coimbra, 2025.*
