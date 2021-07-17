// Execute following block by only one thread
// No synchronisation
#pragma omp single

// Execute following block by only the master thread
// Implicit synchronisation
#pragma omp master

// Execute following block concurrently,
// #threads determined by environment variable OMP_NUM_THREAD=N
#pragma omp parallel
#pragma omp parallel shared(MA, MB, MC, size) private(row, col, i)
#pragma omp parallel privat(MA, MB, MC, size) shared(row, col, i)

// Used if #pragma omp parallel has been called before
// schedule defines whether loop iterations are assigned statically or
// dynamically (depending on which thread is free)
#pragma omp for schedule(dynamic)
#pragma omp for schedule(static)

// Short form for #pragma omp parallel and #pragma omp for
#pragma omp parallel for
#pragma omp parallel for private(x) reduction(+ : area)

// Define sections that get executed by different threads respectively  
#pragma omp parallel sections
#pragma omp section

// Not sure if this exists. If it does, it ends the parallel "block"
#pragma omp parallel end

// All threads get synchronised here
#pragma omp barrier