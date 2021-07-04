// simple program with openmp
// Rauber, Ruenger: Parallele und vert. Prg.

#include <omp.h>
#include <stdio.h>

int npoints, iam, np, mypoints;
double *x;

int main() {
  scanf("%d", &npoints);
  x = (double *)malloc(npoints * sizeof(double));
  initialize();
  omp_set_dynamic(0);
  omp_set_num_threads(NUM_THREADS);

#pragma omp parallel shared(x, npoints) private(iam, np, mypoints)
  {
    np = omp_get_num_threads();
    iam = omp_get_thread_num();
    mypoints = npoints / np;
    compute_subdomain(x, iam, mypoints);
  }

  return 0;
}
