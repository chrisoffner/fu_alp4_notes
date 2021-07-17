#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
  int status;
  pid_t pid;

  pid = fork();
  if (pid == 0) {
    printf("Child process running...\n");
    // Do something...
    printf("Child process done.\n");
    exit(123);
  } else if (pid > 0) {
    printf("Parent process, waiting for child %d...\n", pid);
    pid = wait(&status);
    printf("Child process %d terminated, status %d.\n", pid,
           WEXITSTATUS(status));
    exit(EXIT_SUCCESS);
  } else {
    printf("fork() failed\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}
