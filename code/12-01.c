// part of simple MPI program with MPI_Allreduce
// Rauber, Ruenger: Parallele und vert. Prg.

int m, local_m, n, p;
float a[MAX_N][MAX_LOC_M], local_b[MAX_LOC_M];
float c[MAX_N], sum[MAX_N];

local_m = m / p;
for (i = 0; i < n; i++) {
  sum[i] = 0;
  for (j = 0; j < local_m; j++)
    sum[i] = sum[i] + a[i][j] * local_b[j];
}
MPI_Allreduce(sum, c, n, MPI_FLOAT, MPI_SUM, comm);
