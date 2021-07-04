// simple program with openmp

#include <stdio.h>
//#include <omp.h>

// const int NUM_THREADS = 10;

int main() {
  //  omp_set_dynamic(0);
  //  omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
  printf("I'm a thread \n");

  return 0;
}
