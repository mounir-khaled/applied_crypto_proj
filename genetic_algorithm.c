#include "stdio.h"
#include "time.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/*
	HYPERPARAMETERS:
	- MUTATION RATE R
	- TOURNAMENT SELECTION PROBABILITY P
	- POPULATION SIZE N
*/

typedef struct genotype_s
{
	int chromos[106];
	double fitness;
} genotype_t;

genotype_t crossover(genotype_t parents[2]);

void mutate(genotype_t child);
void shuffle(int *array, size_t n);
void init_pop(genotype_t * population,int n);
void insertInOrder(genotype_t * pop, int size, genotype_t g);
void t_select(genotype_t * pop, genotype_t * winners, int size, double p, int elites);

int main(){
	genotype_t pop[100];
	init_pop(pop,100);

	printf(" - Fitness=%f\n", pop[0].fitness);




}

// tournament selection w/ elitism
void t_select(genotype_t * pop, genotype_t * winners, int size, double p, int elites){
	int w=0;
	int i=0;

	for (i; i<elites; i++){
		winners[w].fitness=pop[i].fitness;
		for (int c=0; c<106; c++)
			winners[w].chromos[c]=pop[i].chromos[c];
		w++;
	}

	while (w<size){
		if ( ( (rand()/RAND_MAX < p) && w-elites==-1 ) || ( (rand()/RAND_MAX < p*pow((double)(1-p),(double)(w-elites+1))) && w-elites>-1 ) ){
			winners[w].fitness=pop[i].fitness;

			for (int c=0; c<106; c++)
				winners[w].chromos[c]=pop[i].chromos[c];

			w++;
		}
		i++;
	}
}

// Bullshit fitness function
double fitness(genotype_t g){
	double f=0;
	for (int i=0; i<106; i++)
		f+=g.chromos[i]*i;

	//f=f;

	return f;
}

// Initialize population with random genotype_ts
void init_pop(genotype_t* population,int n){
	srand(time(NULL));

	int perm[106];
	for (int i=0; i<106; i++) perm[i]=i;

	for (int i=0; i<n; i++){
		shuffle(perm,106);

		population[i].fitness=-1;

		genotype_t g;

		for (int c=0; c<106; c++)
			g.chromos[c]=perm[c];

		g.fitness=fitness(g);

		insertInOrder(population,n,g);
	}
}

// Use carefully
void insertInOrder(genotype_t * pop, int size, genotype_t g){
	int i=0;
	int pos=0;

	while(g.fitness<pop[i].fitness)
		i++;

	pos=i;
	for (i=(size-1);i!=pos;i--){
		pop[i].fitness=pop[i-1].fitness;
		for (int c=0; c<106; c++)
			pop[i].chromos[c]=pop[i-1].chromos[c];
	}

	pop[pos].fitness=g.fitness;
	for (int c=0; c<106; c++)
		pop[pos].chromos[c]=g.chromos[c];
}

// Mate 2 parents
genotype_t crossover(genotype_t parents[2]){

	int swap_map[106];
	for (int i=0; i<106; i++) swap_map[i]=i;

	int g;
	bool p;

	genotype_t child;

	for (int i=0; i<106; i++){
		p=rand()%2;
		g=parents[p].chromos[i];
		child.chromos[i]=swap_map[g];
		swap_map[g]=parents[!p].chromos[i];
	}

	child.fitness=fitness(child);

	return child;
}

// Introduce a mutation
void mutate(genotype_t child){
	int pos1=rand()%106;
	int pos2=rand()%106;
	while (pos1==pos2) pos2=rand()%106;

	// swap
	int temp=child.chromos[pos1];
	child.chromos[pos1]=child.chromos[pos2];
	child.chromos[pos2]=temp;

	child.fitness=fitness(child);
}

void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}










































