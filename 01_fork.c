#include <stdio.h>
#include <unistd.h>

int main(void) {
  pid_t pid;
  pid = fork();

  if (pid == 0) {
    printf("Child process running.\n");
    // lalala
    printf("Child process done.\n");
  } else if (pid > 0) {
    printf("Waiting for child %d...\n", pid);
    printf("Child process %d terminated.\n", pid);
  } else {
    printf("fork() failed.\n");
  }
}