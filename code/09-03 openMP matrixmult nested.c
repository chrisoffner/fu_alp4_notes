// OpenMP Matrix Multiplication

#include <omp.h>
#include <stdio.h>

double MA[100][100], MB[100][100], MC[100][100];
int size = 100;

// Initialise matrices MA and MB
void init_matrices() {
  // Init MA
  for (int i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++) {
      MA[i][j] = (double)(i + j) + 1.0;
      MB[i][j] = (double)(i + j) + 1.0;
    }
}

// Print resulting matrix MC
void print_MC() {
  for (int i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++)
      printf("%f ", MC[i][j]);
  printf("\n");
}

int main() {
  init_matrices();

  // Run next block concurrently where each
  // thread gets its own row, col, and i
#pragma omp parallel
  {
    // Concurrent FOR loop where each threads gets
    // assigned a constant number of iterations
#pragma omp for schedule(static)
    for (int row = 0; row < size; row++) {
      // Threads share access to matrices and size
#pragma omp parallel shared(MA, MB, MC, size)
      {
        // Same as previously, concurrent FOR loop
#pragma omp for schedule(static)
        for (int col = 0; col < size; col++) {
          MC[row][col] = 0.0;
          for (int i = 0; i < size; i++)
            MC[row][col] += MA[row][i] * MB[i][col];
        }
      }
    }
  }
  print_MC();
  return 0;
}
