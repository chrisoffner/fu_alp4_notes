// zwei Prozesse rechnen sinnvoll

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
  int data[2][3];
  int i, j;
  int status = 0;
  int result = 0;
  pid_t pid;

  // init data
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      data[i][j] = (i + 1) * j;
    }
  }

  pid = fork();
  if (pid == 0) {
    // child process is doing
    // something...
    for (j = 0; j < 3; j++) {
      result += data[0][j];
    }
    exit(result);
  } else if (pid > 0) {
    // parent process is doing...
    for (j = 0; j < 3; j++) {
      result += data[1][j];
    }
    pid = wait(&status);
    printf("\n Results: %d, %d\n", result, WEXITSTATUS(status));
    exit(EXIT_SUCCESS);
  } else {
    printf("fork() failed\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}
