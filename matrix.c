// Frederick Luke Jamar
// CS 3339 Computer Arch
// matrix.c

#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

#include<omp.h>

#define REPS 100


int main(int argc, char *argv[]) {
  int **a, **b;
  
  int matrix = atoi(argv[1]);   // Number of units in each matrix
  int mult = atoi(argv[2]);     // Number to multiply the matrix
  int cores = atoi(argv[3]);    // Number of cores

  omp_set_num_threads(cores);

  a = (int **) malloc(sizeof(int *) * matrix);
  b = (int **) malloc(sizeof(int *) * matrix);
  
  int i, j, k;
  for (i = 0; i < matrix; i++) {
    a[i] = (int *) malloc(sizeof(int) * matrix);
    b[i] = (int *) malloc(sizeof(int) * matrix);
  }

  for (j = 0; j < matrix; j++)
    for (i = 0; i < matrix; i++)
      b[i][j] = i + j;

#pragma omp parallel for private(j,i)
  for (k = 0; k < REPS; k++) {
    for (j = 0; j < matrix; j++) 
      for (i = 0; i < matrix; i++)
        a[i][j] = b[i][j] * mult;
  }
  
  printf("Matrix Multiplied!/n");


  return 0;

}
