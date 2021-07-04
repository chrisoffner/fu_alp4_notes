// https://www.open-mpi.org/doc/v1.8/man3/MPI_Op_create.3.php

typedef struct {
  double real, imag;
} Complex;

// the user-defined function
void myProd(Complex *in, Complex *inout, int *len, MPI_Datatype *dptr) {
  int i;
  Complex c;

  for (i = 0; i < *len; ++i) {
    c.real = inout->real * in->real - inout->imag * in->imag;
    c.imag = inout->real * in->imag + inout->imag * in->real;
    *inout = c;
    in++;
    inout++;
  }
}

// and, to call it...
...
    // each process has an array of 100 Complexes
    Complex a[100],
    answer[100];
MPI_Op myOp;
MPI_Datatype ctype;

// explain to MPI how type Complex is defined
MPI_Type_contiguous(2, MPI_DOUBLE, &ctype);
MPI_Type_commit(&ctype);
// create the complex-product user-op
MPI_Op_create(myProd, True, &myOp);

MPI_Reduce(a, answer, 100, ctype, myOp, root, comm);

// At this point, the answer, which consists of 100 Complexes,
// resides on process root
