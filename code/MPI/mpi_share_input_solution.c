#include <mpi.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 32

int main(int argc, char** argv) {
    
    char buffer[BUFFER_SIZE] = "FIX ME!!!";
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(NULL, NULL);

    int world_rank, name_len;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Get_processor_name(processor_name, &name_len);
    
    if(world_rank == 0) {
        if(argc >= 2) {
            strcpy(buffer, argv[1]);
        } else {
            strcpy(buffer, "Korrekter Text");   
        }
    }
    
    printf("BEFORE: %s from processor %s, rank %d\n", buffer, processor_name, world_rank);
    
    // LÖSUNG ANFANG
 
    MPI_Bcast(&buffer, BUFFER_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD); 

    // LÖSUNG ENDE

    // Hier sollte kein Prozess mehr "FIX ME!!!" ausgeben!
    printf("AFTER: %s from processor %s, rank %d\n", buffer, processor_name, world_rank);
    
    MPI_Finalize();
}