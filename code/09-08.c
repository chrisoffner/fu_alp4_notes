// simple program with openmp
// Rauber, Ruenger: Parallele und vert. Prg.

#include <omp.h>
#include <stdio.h>

int main() {
  int iam, neighbor, work[2], sync[2];

#pragma omp parallel private(iam, neighbor) shared(work, sync)
  {
    iam = omp_get_thread_num();
    sync[iam] = 0;
#pragma imp barrier
    work[iam] = do_work();
#pragma omp flush(work)
    sync[iam] = 1;
#pragma omp flush(sync)
    neighbor = (iam != 0) ? (iam - 1) : (omp_get_num_threads() - 1);
    while (sync[neighbor] == 0) {
#pragma omp flush(sync)
      {}
    }
    combine(work[iam], work[neighbor]);
  }
  return 0;
}
