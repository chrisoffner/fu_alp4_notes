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

    for(int64_t i = 0; i < n; ++i) {
        x = ((double) i + 0.5) / n;
        area += 4.0 / (1.0 + x * x);
    }

    double pi;

    if(world_rank == 0) {
        printf ("Pi: %f \n", pi);
    }
    
    // LÖSUNG ENDE

    MPI_Finalize();
}
