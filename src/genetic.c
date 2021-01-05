#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INIT_MAX_VAL 1000000
#define POP_COUNT 40
#define CHROMOSOMES 6
#define MUTATION_PROB 20 // on in many much should be mutated?
#define PARENTS_RATIO 1
#define PARENTS_SIZE (POP_COUNT / PARENTS_RATIO)
#define TARGET_SCORE 1000000

extern unsigned int run_count;

typedef struct DNA {
  unsigned int args[CHROMOSOMES];
  int score;
} DNA;

int comp(const void *elem1, const void *elem2) {
  DNA f = *((DNA *)elem1);
  DNA s = *((DNA *)elem2);
  if (f.score > s.score)
    return -1;
  if (f.score < s.score)
    return 1;
  return 0;
}

double gen_range(double start, double end) {
  double range = end - start;
  double result = rand() / (RAND_MAX * 1.0);

  result *= range;
  return result + start;
}

int main34() {
  /* Generation #14, Rounds: 1, Current best: 108804
  #####
  ARG_1:  330883  ARG_2:  935658  ARG_3:  821186  ARG_4:  722742  ARG_5:  904476
  ARG_6:  771457
  #####

  Current local time and date: Fri Aug 14 22:57:15 2020 */
  unsigned int args[6] = {330883, 935658, 821186, 722742, 904476, 771457};
  tetris_main(args);
  return 0;
}

int main() {
  DNA population[POP_COUNT];
  DNA parents[PARENTS_SIZE];
  DNA best;
  int generation = 1;
  time_t rawtime;
  struct tm *timeinfo;

  srand(time(0));
  // init population
  for (int i = 0; i < POP_COUNT; i++)
    for (int k = 0; k < CHROMOSOMES; k++)
      population[i].args[k] = rand() % INIT_MAX_VAL;

  best.score = 0;
  for (int i = 0; i < CHROMOSOMES; i++)
    best.args[i] = 1;

  while (best.score < TARGET_SCORE) {
    // calculate fitness of the individuals
    for (int i = 0; i < POP_COUNT; i++) {
      srand(0); // re initializing random numb generator for consistency

      population[i].score = tetris_main(population[i].args);

      if (population[i].score > best.score && best.score) {
        printf("!->%d<-! ", population[i].score);
      } // else
        // printf("%d ", population[i].score);
    }
    printf("\n");

    // reseed
    srand(time(0));

    // choose parents and make chidrens and mutate them
    // first sort them
    qsort(population, POP_COUNT, sizeof(DNA), comp);

    best = population[0].score > best.score ? population[0] : best;
    printf("\nGeneration #%d, Rounds: %d, Current best: %d\n#####\n",
           generation++, run_count, best.score);
    for (int k = 0; k < CHROMOSOMES; k++)
      printf("ARG_%d:\t%d\t", k + 1, best.args[k]);
    printf("\n#####\n\n");

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Current local time and date: %s\n", asctime(timeinfo));

    if (0) {
      for (int i = 0; i < POP_COUNT; i++) {
        printf("[score: %d; args: ", population[i].score);
        for (int j = 0; j < CHROMOSOMES - 1; j++)
          printf("%d ", population[i].args[j]);
        printf("%d] ", population[i].args[CHROMOSOMES - 1]);
      }
      printf("\n\n");
    }

    // choose parents
    for (int i = 0; i < PARENTS_SIZE; i++)
      parents[i] = population[i];

    for (int i = POP_COUNT / 2; i < POP_COUNT; i++)
      for (int k = 0; k < CHROMOSOMES; k++)
        population[i].args[k] = rand() % INIT_MAX_VAL;

    for (int idx = 0; idx < POP_COUNT / 2; idx++) {
      int parent1_idx = rand() % (PARENTS_SIZE);
      int parent2_idx = rand() % (PARENTS_SIZE);

      // if the two parents are the same then choose again until they arent
      while (parent1_idx == parent2_idx)
        parent2_idx = rand() % (PARENTS_SIZE);

      population[idx] = parents[parent1_idx];
      for (int i = 0; i < CHROMOSOMES; i++)
        if (rand() % 2)
          population[idx].args[i] = parents[parent2_idx].args[i];

      if (rand() % MUTATION_PROB == 0) {
        int param_idx = rand() % CHROMOSOMES;
        int new_val = 0;

        switch (rand() % 3) {
        default:
          new_val = population[idx].args[param_idx] * gen_range(-0.75, 0.75);
          break;
        case 0:
          new_val = population[idx].args[param_idx] * gen_range(-0.25, 0.25);
          break;
        case 1:
          new_val = population[idx].args[param_idx] * gen_range(-0.05, 0.05);
          break;
        }
        if (population[idx].args[param_idx] + new_val >= 0)
          population[idx].args[param_idx] += new_val;
      }
    }
  }

  return 0;
}