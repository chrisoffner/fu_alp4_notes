#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int status;
  pid_t pid;
  pid = fork();
  int a, b = 0;

  if (pid == 0) {
    // Child process
    printf("Child process running.\n");
    a = 2;
    b = 3;
    a *= b;
    printf("Result of child process is %d.\n", a);
    // lalala
    printf("Child process done.\n");
    exit(123);
  } else if (pid > 0) {
    // Parent process
    printf("Parent process waiting for child %d...\n", pid);
    pid = wait(&status);
    a = 3;
    b = 4;
    a *= b;
    printf("Result of parent process is %d.\n", a);

    printf("Parent finds child process %d terminated, status %d.\n", pid,
           WEXITSTATUS(status));

    exit(EXIT_SUCCESS);
  } else {
    printf("fork() failed.\n");
    exit(EXIT_FAILURE);
  }
}