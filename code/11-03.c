// Example for a program with a deadlock depending on the implementation of the
// MPI environment Rauber, Ruenger: Parallele und vert. Prg.

MPI_Comm_rank(comm, &my_rank);
if (my_rank == 0) {
  MPI_Send(sendbuf, count, MPI_INT, 1, tag, comm);
  MPI_Recv(recvbuf, count, MPI_INT, 1, tag, comm, &status);
} else if (my_rank == 1) {
  MPI_Send(sendbuf, count, MPI_INT, 0, tag, comm);
  MPI_Recv(recvbuf, count, MPI_INT, 0, tag, comm, &status);
}
