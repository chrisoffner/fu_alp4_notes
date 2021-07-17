// zwei Prozesse rechnen

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
  int status;
  pid_t pid;
  int a, b = 0;

  pid = fork();
  if (pid == 0) {
    // child process is doing something...
    a = 2;
    b = 3;
    a = a * b;
    printf(" result of child process: %d \n", a);
    exit(123);
  } else if (pid > 0) {
    // parent process is doing something and is waiting for child ...
    a = 2;
    b = 3;
    a = a * b;
    pid = wait(&status);
    printf(" result of parent process: %d \n", a);
    printf("Child process %d terminated, status %d.\n", pid,
           WEXITSTATUS(status));
    exit(EXIT_SUCCESS);
  } else {
    printf("fork() failed\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}
