#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define SIZE 100000000

/**
 *  DO NOT TOUCH!
 */
void generateInput(int16_t* arr) {
    for(int32_t i = 0; i < SIZE; ++i) {
        arr[i] = rand() % 10000;   
    }
    int32_t maxIndex = rand() % SIZE;
    arr[maxIndex] = 10000 + rand() % 10000;
    printf("Expected: Maximum is %d\n", arr[maxIndex]);
}

int16_t calcMax(int16_t* arr, int32_t size) {
    int16_t max = arr[0];
    for(int32_t i = 0; i < size; ++i) {
        if(max < arr[i]) {
            max = arr[i];
        }
    }
    return max;
}

int main () {
    srand(time(NULL));
    
    MPI_Init(NULL, NULL);

    int16_t *input;
    int32_t world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    int32_t buffer_size = SIZE / world_size;
    
    if(world_rank == 0) {
        input = (int16_t*) malloc(SIZE * sizeof(int16_t));
        generateInput(input);   
    }
    
    int16_t* buffer = (int16_t*) malloc(buffer_size * sizeof(int16_t));
    
    // LÖSUNG ANFANG

    if(world_rank == 0) {
        printf("Calculated: Maximum is %d\n", 0);
        free(input);
    }
    
    // LÖSUNG ENDE
    
    free(buffer);

    MPI_Finalize();
}
