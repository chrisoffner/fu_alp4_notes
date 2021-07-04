# Concurrent & Distributed Programming

## Concurrent Programming Paradigms

- **Concurrent programming**
  Machine with one CPU
- **Parallel programming**
  Machine with several CPUs with or without shared memory
  Machine which requires message exchange
- **Distributed programming**
  Different machines connected via a network

## Programming Model

**Requirements for programs**

- ***Functional requirements***
  - scope of functions
  - correctness
- ***Nonfunctional requirements***
  - Performance
  - Usability
  - Security
  - Maintainability (readable code, etc.)
  - etc.

### Foundations of writing a correct program

1. Set of operations for which we know how they change the state of system
2. Apply them in a specific order in which they perform the desired algorithm

---

**Programming** means constructing a (correct) programming model. Being able to reason accurately about the changes of state in a system after applying specific operations in a specific order constitutes a **programming model**, i.e. a mental model of how a program changes a system.

The **machine model** represents the physical execution of a program on the hardware.

---

> ## Deterministic
>
> A **deterministic algorithm** is an algorithm which, given a specific input, will always produce the same specific output and will always change the state of the system in the same way (and order).

> ## Determined 
>
> A **determined algorithm** is an algorithm which, given a specific input, produces always the same specific output.

> ### What ensures the sequential execution of a program?
>
>The assembly code of a program gets executed sequentially due to the automatic incrementation of the instruction pointer.

---
## 3 Concurrency

### Process generation with `fork()`

The `fork()` system call, defined in the POSIX standard, creates an exact copy of the calling process in a new address space in main memory. Both parent and child hold separate copies of all variables.

The resulting processes can be distinguished by the distinct return values they receive from `fork()`:

- For the child process, `pid = fork()` sets `pid` to  `0`
- For the parent process, `pid = fork()` sets `pid` to the process id of the newly created child process

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  int status;
  pid_t pid = fork(); // returns process ID of child to parent
                      // and 0 to child process
  if (pid == 0) {
    // Child process
    printf("Child process running.\n");
    // Work could be done here...
    printf("Child process done.\n");
    exit(123);
  } else if (pid > 0) {
    // Parent process
    printf("Parent process waiting for child %d...\n", pid);

    pid = wait(&status);

    printf("Parent finds child process %d terminated, status %d.\n", pid,
           WEXITSTATUS(status));

    exit(EXIT_SUCCESS);
  } else {
    printf("fork() failed.\n");
    exit(EXIT_FAILURE);
  }
}
```

Without some special methods not covered in this course, processes cannot access each others' memory, and therefore cannot effectively work on data where they need to continuously exchange information (as opposed to merely merging together separate results in the end). To achieve concurrency with shared memory, we make use of **threads**.

## 4 Threads

Unlike processes, which each contain their own unique process id and address space, threads of the same process share a common process id and "live" in the same address space. They share memory (heap) but each thread has its own stack.

To use threads in different environments there exists a standardized interface: POSIX **pthreads**

A new pthread does not start right after the invocation (as would be the case with `fork()`), but instead starts execution at a given address in the program's text/code segment. Therefore a function pointer is passed to `pthread_create()` to give the thread a starting point for execution.

### Critical Section

...

### Mutual Exclusion

...

- Lock
- Twofold lock
- Twofold Lock with Primary Protection
- Twofold Lock with Mutual Precedence

### Requirements for Protection of the Critical Section

- Solution has to protect the critical section reliably by **mutual exclusion**.
- Solution should be portable across architectures and usable in higher level programming languages.
- Solution must not lead to a **deadlock**.

## 5 Parallelization

...

## 6 Petri Nets

...

## 7 Deadlocks

- Defining Deadlocks
- Requirements for a Deadlock
- Handling Deadlocks
- Modelling Resource Allocations
- **Banker's Algorithm**

## 8 Semaphores & Monitors

### Semaphores

...

### Monitors

...

## 9 OpenMP

...

## 10 Parallel Programming with Message Passing

- Parallel programming with message passing
- Foster's Design Methodology

## 11 MPI – Message Passing Interface

...

## 12 MPI Group Communication and MPI 2

...

## 13—15 Design and Implementation of Parallel Applications

...

## 16 From Concurrent to Parallel Programming

...

## 17 Communication Paradigms in Distributed Systems

...

## 18.1 Interprocess Communication

...

### 18.2 - 18.3 Remote Invocation

#### Distribution Transparency

- Access Transparency
- Location Transparency
- Migration Transparency
- Replication Transparency

### RMI - Remote Method Invocation

...

### Java RMI

... 

## 20 Peer-to-Peer Systems

...

### Basics

...

### 20.2 Centralized, Pure and Hybrid P2P Systems

...

### 20.3 DHT-based P2P Systems

...

### 20.4 P2P-based Whiteboard

...

## 21 Cloud Computing

...

### 21.2 Background Technologies

...

### 21.3 Deployment and Service Models

...

## 22 Web Application Development

...

### 22.2 CSS and JS

...

### 22.3 JS Frameworks

...

### 24.1 URI & URL

...

### 24.2 HTTP

...

### REST & WebSockets

...



# Theme-specific formatting

Using the **[Fluent](https://github.com/HereIsLz/Fluent-Typora)** theme in Typora we can use special rendering for semantically distinct blocks like this:

> # Bookmark
> An `H1` inside a block quote gets rendered as **Bookmark**.

> ## Starred
> An `H2` inside a block quote gets rendered as **Starred**.

> ### Idea
> An `H3` inside a block quote gets rendered as **Idea**.

> #### Warning
> An `H4` inside a block quote gets rendered as **Warning**.

> ##### Question
> An `H5` inside a block quote gets rendered as **Question**.

