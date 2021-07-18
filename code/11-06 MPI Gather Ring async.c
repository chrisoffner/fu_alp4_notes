// MPI Gather Ring with asynchronous communication

void Gather_ring(float x[], int blocksize, float y[]) {
  int i, p, my_rank, succ, pred;
  int send_offset, recv_offset;
  MPI_status status;
  MPI_Request send_request, recv_request;

  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  for (i = 0; i < blocksize; i++)
    y[i + my_rank * blocksize] = x[i];
  succ = (my_rank + 1) % p;
  pred = (my_rank - 1 + p) % p;
  send_offset = my_rank * blocksize;
  recv_offset = ((my_rank - 1 + p) % p) * blocksize;
  for (i = 0; i < p - 1; i++) {
    MPI_Isend(y + send_offset, blocksize, MPI_FLOAT, succ, 0, MPI_COMM_WORLD,
              &send_request);
    MPI_Irecv(y + recv_offset, blocksize, MPI_FLOAT, pred, 0, MPI_COMM_WORLD,
              &recv_request);
    send_offset = ((my_rank - i - 1 + p) % p) * blocksize;
    recv_offset = ((my_rank - i - 2 + p) % p) * blocksize;
    MPI_Wait(&send_request, &status);
    MPI_Wait(&recv_request, &status);
  }
}
