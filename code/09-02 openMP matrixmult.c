// OpenMP Matrix Multiplication

#include <omp.h>
#include <stdio.h>

double MA[100][100], MB[100][100], MC[100][100];
int i, row, col, size = 100;

void read_input() {
  for (i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++) {
      MA[i][j] = (double)(i + j) + 1.0;
      MB[i][j] = (double)(i + j) + 1.0;
    }
}

void write_output() {
  for (i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++)
      printf("%f ", MC[i][j]);
  printf("\n");
}

int main() {
  read_input(); // MA, MB
#pragma omp parallel shared(MA, MB, MC, size) private(row, col, i)
  {
#pragma omp for schedule(static)
    for (row = 0; row < size; row++) {
      for (col = 0; col < size; col++)
        MC[row][col] = 0.0;
    }
#pragma omp for schedule(static)
    for (row = 0; row < size; row++) {
      for (col = 0; col < size; col++)
        for (i = 0; i < size; i++)
          MC[row][col] += MA[row][i] * MB[i][col];
    }
  }
  write_output(); // MC

  return 0;
}
