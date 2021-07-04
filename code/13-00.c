// sequential program matrix mult.
// Rauber, Ruenger: Parallele und vert. Prg.

#include <stdio.h>

double MA[100][100], MB[100][100], MC[100][100];
int i, row, col, size = 100;

void read_input() {
  int j;

  for (i = 0; i < 100; i++)
    for (j = 0; j < 100; j++)
      MA[i][j] = (double)(i + j) + 1.0;

  for (i = 0; i < 100; i++)
    for (j = 0; j < 100; j++)
      MB[i][j] = (double)(i + j) + 1.0;
}

void write_output() {
  int j;

  for (i = 0; i < 100; i++)
    for (j = 0; j < 100; j++)
      printf("%f ", MC[i][j]);
  printf("\n");
}

int main() {
  read_input(); // MA, MB

  for (row = 0; row < size; row++) {
    for (col = 0; col < size; col++)
      MC[row][col] = 0.0;
  }

  for (row = 0; row < size; row++) {
    for (col = 0; col < size; col++)
      for (i = 0; i < size; i++)
        MC[row][col] += MA[row][i] * MB[i][col];
  }

  write_output(); // MC

  return 0;
}
