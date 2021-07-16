// Sieve of Erastosthenes
// Quinn: Parallel Prg. in C with MPI and OpenMP

#include <math.h>
#include <mpi.h>
#include <stdio.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id) + 1, p, n))
#define BLOCK_SIZE(id, p, n) (BLOCK_LOW((id) + 1, p, n) - BLOCK_LOW((id), p, n))

int main(int argc, char *argv[]) {
  int count, first, global_count, high_value, i, id, index, low_value, n, p,
      proc0_size, k, size;
  double elapsed_time;
  char *marked;

  MPI_Init(&argc, &argv);
  // Start the timer
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();

  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if (argc != 2) {
    if (!id)
      printf("Command line : %s <m> \n", argv[0]);
    MPI_Finalize();
    exit(1);
  }

  n = atoi(argv[1]);

  // Figure out this procs share of the array
  low_value = 2 + BLOCK_LOW(id, p, n - 1);
  high_value = 2 + BLOCK_HIGH(id, p, n - 1);
  size = BLOCK_SIZE(id, p, n - 1);

  // Bail out if all the primes used for sieving are not all held by proc 0
  proc0_size = (n - 1) / p;
  if ((2 + proc0_size) < (int)sqrt((double)n)) {
    if (!id)
      printf("Too many processes \n");
    MPI_Finalize();
    exit(1);
  }

  // Allocate this proc's share of the array
  marked = (char *)malloc(size);

  if (marked == NULL) {
    printf("Cannot allocate enough memory \n");
    MPI_Finalize();
    exit(1);
  }

  for (i = 0; i < size; i++)
    marked[i] = 0;
  if (!id)
    index = 0;
  k = 2;

  do {
    if (k * k > low_value)
      first = k * k - low_value;
    else {
      if (!(low_value % k))
        first = 0;
      else
        first = k - (low_value % k);
    }

    for (i = first; i < size; i += k)
      marked[i] = 1;

    // proc 0 gets the next k for all
    if (!id) {
      while (marked[++index])
        ;
      k = index + 2;
    }
    MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);
  } while (k * k <= n);

  // counting results
  count = 0;
  for (i = 0; i < size; i++)
    if (!marked[i])
      count++;
  MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  // Stop timer
  elapsed_time += MPI_Wtime();

  // Print results
  if (!id) {
    printf("%d primes are less than or equal to %d \n", global_count, n);
    printf("Total elapsed time: %10.6f \n", elapsed_time);
  }
  MPI_Finalize();
  return 0;
}
