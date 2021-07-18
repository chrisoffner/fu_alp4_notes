#include <mpi.h>
#include <stdio.h>
#include <stdint.h>

int main () {

    MPI_Init(NULL, NULL);

    double area = 0.0, x;
    int32_t world_rank, world_size;
    int64_t n = 1000000000;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    // LÖSUNG ANFANG

    for(int64_t i = world_rank * n / world_size; i < (world_rank + 1) * n / world_size; ++i) {
        x = ((double) i + 0.5) / n;
        area += 4.0 / (1.0 + x * x);
    }

    double pi;
    MPI_Reduce(&area, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(world_rank == 0) {
        pi /= n;
        printf("Pi: %f \n", pi);
    }
    
    // LÖSUNG ENDE

    MPI_Finalize();
}
