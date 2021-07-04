// simple program with openmp to calc pi
// Segall, et al: Research and Applications in Global Supercomputing

#include <omp.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
  long long i;                   // loop variable
  long long num_terms = 500000L; // number of terms for estimating pi
  double sign;                   // sign of a term
  double sum = 0.0;              // incremental estimate of pi
  int thread_cnt = 10;           // number of threads to be used

# pragma omp parallel for num_threads(thread_cnt) reduction(+: sum) private(sign) schedule(dynamic)
  for (i = 0; i < num_terms; i++) {
    sign = (i % 2 == 0) ? 1.0 : -1.0;
    sum += sign / (2 * i + 1);
  }
  sum = 4.0 * sum;
  printf("n = %lld terms, %d threads,\n", num_terms, thread_cnt);
  printf(" estimated value of pi =%.14f\n", sum);
  return 0;
}
