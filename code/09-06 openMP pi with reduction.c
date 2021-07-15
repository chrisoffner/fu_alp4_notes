// simple program with openmp to calc pi
// Quinn: parallel prog. in c with mpi and openmp

#include <omp.h>
#include <stdio.h>

int main() {
  double area, pi, x;
  int i, n;

  n = 1000000000;
  area = 0.0;
#pragma omp parallel for private(x) reduction(+ : area)
  for (i = 0; i < n; i++) {
    x = ((double)i + 0.5) / n;
    area += 4.0 / (1.0 + x * x);
  }
  pi = area / n;

  printf("Pi: %f \n", pi);

  return 0;
}
