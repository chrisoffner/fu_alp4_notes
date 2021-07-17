// simple matrix mult. with 2 procs

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int ma[2][3];
  int mb[3][2];
  int result = 0;
  int i, j;
  int status = 0;
  pid_t pid;

  // Initialisation
  srand((unsigned)time(NULL));

  // Initialise matrix A
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      ma[i][j] = (int)(((double)rand() / (RAND_MAX - 1)) * 100);
      printf("%d,%d: %d \n", i, j, ma[i][j]);
    }
  }

  // Initialise Matrix B
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 2; j++) {
      mb[i][j] = (int)(((double)rand() / (RAND_MAX - 1)) * 100);
      printf("%d,%d: %d \n", i, j, mb[i][j]);
    }
  }

  pid = fork();
  if (pid == 0) {
    // Child process
    for (i = 0; i < 2; i++) {
      result = 0;
      printf("0,%d : ", i);
      for (j = 0; j < 3; j++) {
        result += ma[0][j] * mb[j][i];
      }
      printf(" %d \n", result);
    }
    exit(0);
  } else if (pid > 0) {
    // parent process is doing...
    for (i = 0; i < 2; i++) {
      result = 0;
      printf("1,%d : ", i);
      for (j = 0; j < 3; j++) {
        result += ma[1][j] * mb[j][i];
      }
      printf(" %d \n", result);
    }
    pid = wait(&status);
    printf("\n  %d\n", WEXITSTATUS(status));
    exit(EXIT_SUCCESS);
  } else {
    printf("fork() failed\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}
