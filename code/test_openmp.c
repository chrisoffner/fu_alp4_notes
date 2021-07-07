#include <omp.h>
#include <stdio.h>

void report_num_threads(int level) {
  // Let a single thread print the team's thread count
#pragma omp single
  printf("Level %d:\t%d threads in team.\n", level, omp_get_num_threads());
}

int main(void) {
  // Allow nested thread creation
  omp_set_nested(1);
#pragma omp parallel num_threads(2)
  {
    // Print level 1 thread count
    report_num_threads(1);
#pragma omp parallel num_threads(2)
    {
      // Print level 2 thread count
      report_num_threads(2);
#pragma omp parallel num_threads(2)
      // Print level 3 thread count
      report_num_threads(3);
    }
  }
}