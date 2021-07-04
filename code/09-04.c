// simple program with openmp
// Quinn: parallel prog. in c with mpi and openmp

#include <omp.h>
#include <stdio.h>

double alpha() {
  printf("alpha \n");
  return 1.1;
}

double beta() {
  printf("beta \n");
  return 2.2;
}

double delta() {
  printf("delta \n");
  return 3.3;
}

double my_gamma(double v, double w) {
  printf("gamma \n");
  return v + w;
}

double epsilon(double x, double y) {
  printf("epsilon \n");
  return x + y;
}

int main() {
  double v, w, x, y;

#pragma omp parallel sections
  {
#pragma omp section
    v = alpha();
#pragma omp section
    w = beta();
#pragma omp section
    y = delta();
  }
  x = my_gamma(v, w);
  printf("%6.2f \n", epsilon(x, y));

  return 0;
}
