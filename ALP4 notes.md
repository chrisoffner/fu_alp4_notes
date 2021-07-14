## Concurrent & Distributed Programming

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
## Concurrency

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

### Threads

Unlike processes, which each contain their own unique process id and address space, threads of the same process share a common process id and "live" in the same address space. They share memory (heap) but each thread has its own stack.

To use threads in different environments there exists a standardized interface: POSIX **pthreads**

A new pthread does not start right after the invocation (as would be the case with `fork()`), but instead starts execution at a given address in the program's text/code segment. Therefore a function pointer is passed to `pthread_create()` to give the thread a starting point for execution.

### Critical Section

> ## Critical Section
>
> A program section (series of operations) that only one thread is permitted to execute at a time to ensure the correct execution of the program is called **critical section**.

## Mutual Exclusion

...

### Anforderungen zur Sicherung kritischer Abschnitte

- **Mutual Exclusion**
    Der kritische Abschnitt wird durch gegenseitigen Ausschluss geschützt.
- **Fairness**
    Kein Thread/Prozess wird beim Zugang zum kritischen Abschnitt dauerhaft bevor- oder benachteiligt.
- **Low Overhead**
    Die Methode sollte keinen erheblichen Mehraufwand verursachen.
- **Portability**
    Die Lösung sollte auch in höheren Programmiersprachen nutzbar sein.
- **Deadlock-free**
    Sollte nicht zu Deadlocks führen.

### Classical Locks

- Lock
- Twofold lock
- Twofold Lock with Primary Protection
- Twofold Lock with Mutual Precedence
- n-fold
- Peterson
- Dekker
- Lamport's Bakery

### Requirements for Protection of the Critical Section

- Solution has to protect the critical section reliably by **mutual exclusion**.
- Solution should be portable across architectures and usable in higher level programming languages.
- Solution must not lead to a **deadlock**.

## Parallelization

...

## Petri Nets

...

### Properties

- **Reachability**
    A part of the Petri net is reachable if there is a sequence of transitions able to fire at some point in time and leading to mark the places or one of the places of this part with tokens. This is interesting in order to decide if some part of the program code is executed or a sub-system is during operation of the whole system.
- **Liveness**
    Different types of liveness can be investigated, describing if the net or parts of the Petri net are able to show some actions by firing of transitions unless the final state is reached.
    - Weak liveness is given if there is a transition that can fire and the firing of that transition enables the firing of (at least) one other transition.
    - Strong liveliness is given when the weak liveliness applies to all transitions.
- **Termination**
    After a sequence of transitions the Petri net is no longer alive.
- **Boundedness**
    Indicates if parts of the Petri net show or may show an overload situation.
- **Security**
    A Petri net is secure, if an increase in capacities of a place does not lead to an increased possibility in firing of transitions.

## Deadlocks

> ## Deadlock
>
> A **deadlock** is a program state in which execution cannot proceed because at least two threads are waiting for the release of a resource that the other thread currently holds. A deadlock is characterised by a cycle in the wait-for graph.

### Requirements for a Deadlock

1. **Mutual Exclusion**
    Ressourcen werden exklusiv genutzt.
2. **Hold and Wait**
    Prozesse/Threads beanspruchen Zugriff auf Betriebsmittel und fordern zusätzlich Zugriff auf weitere an.
3. **No Preemption**
    Betriebsmittel werden ausschließlich durch die Prozesse/Threads freigegeben, die sie nutzen. 
4. **Circular Wait**
    Der Wait-For-Graph enthält einen Kreis.

### Dining Philosophers Problem[^yt_1]

<img src="ALP4 notes.assets/Philosophers.svg" alt="Philosophers" style="zoom: 50%;" />

### Handling Deadlocks

#### Prevention

- **Pre-claiming:** All resources needed by a thread are requested (and allocated) at start time.
    - 👍 Deadlock requirement 2 (Hold and Wait) cannot occur.
    - 👎 In dynamic systems the overall demand is difficult to predict.
    - 👎 Uneconomical because resources are occupied longer than needed.
- **Overall Release at Request:** When requesting a resource a thread needs to release all resources it currently holds.
    - 👍 Deadlock requirement 2 (*Hold and Wait*) cannot occur.
    - 👍 *Circular Wait* is also prevented since a thread does not hold any resources when allocating a new one.
- **Allocation by (Given) Order:** Resource are sorted $(\mathrm{Res}_1, \mathrm{Res}_2, \mathrm{Res}_3, ...)$ and resource allocation is performed in order only.
    - 👍 *Circular Wait* is prevented.

#### Avoidance

...

#### Detection

A set of threads T is not in a deadlock if there exists an order of termination so that all requests of thread $i$ can be terminated by the free resources or by resources freed up by threads j, where $j<i$.

We use the Banker's Algorithm in which we replace $R$ with $A$.

#### Resolution, Recovery

...

### Resource Allocation Model

#### Model

$$
\begin{align*}
T\text{ with }|T| = m\qquad &\text{Set of threads (or processes)}\\\\
Rs\text{ with }|Rs| = n\qquad &\text{Set of resource types}\\\\
\vec{v}:= (v_1, v_2, ..., v_n)\qquad &\text{Existing resources}\\\\
A:=\begin{pmatrix}a_{11}&...&a_{1n}\\\vdots&&\vdots\\a_{m1}&...&a_{mn}\end{pmatrix}\qquad &\text{Current resource requests}\\\\
B:=\begin{pmatrix}b_{11}&...&b_{1n}\\\vdots&&\vdots\\b_{m1}&...&b_{mn}\end{pmatrix}\qquad &\text{Currently allocated resources}\\\\
G:=\begin{pmatrix}g_{11}&...&g_{1n}\\\vdots&&\vdots\\g_{m1}&...&g_{mn}\end{pmatrix}\qquad &\text{Total resource requests}\\\\
(T, Rs, \vec{v}, A, B)\qquad &\text{Current resource allocation}\\\\
\vec{f}:= (f_1, f_2, ..., f_n)\quad\text{ with }\quad f_j=v_j -\sum_{i}b_{ij} \qquad&\text{Free resources = existing - allocated resources}\\\\
R:= G-B\qquad&\text{Remaining requests = total - currently allocated}\\\\
\vec{a}_i, \vec{b}_i, \vec{g}_i, \vec{r}_i\qquad&\text{Row vectors for requests, etc., of thread $i$}\\\\
\vec{x} \le \vec{y} \Longleftrightarrow \forall k\colon x_k \le y_k\qquad&\text{$\vec{x}\le \vec{y}$ if comparison holds for each component}
\end{align*}
$$

#### Constraints

1. Each resource type can be allocated only as often as it exists.
    $\forall j\in\{1,...,n\}\colon\quad\sum_{i=1}^m b_{ij}\le v_j$
2. Requests can only contain existing resources.
    $\forall i\in\{1,...,m\}\quad\forall j\in\{1,...,n\}:\quad\begin{cases}a_{ij}+b_{ij}&\le v_{j}\\g_{ij}&\le v_j\end{cases}$
3. Requesting threads are blocked until allocation.
4. Only non-blocked threads can request another resource.

#### States

- Thread $T_i$ is blocked if the current request cannot be satisfied.
    $a_i \not\le \vec{f}$
    - The set of threads $T$ is in a deadlock if at any future point there exists a subset of threads whose requests cannot be satisfied by the resources *not* allocated to threads of $T$.
- This subset of threads $\{T_i\mid i\in I\}$ is also said to be in a deadlock.

#### Example

...

### Banker's Algorithm

- Calculate the vector of the (currently) free resources.
    $\vec{f} = \vec{v} - \sum_{i=1}^m b_{i}$ 
- As long as there exists a thread that is not marked as *"terminated"*:
    - Check for each thread $i$ whether the remaining requests can be met with the free resources: $\vec{r}_i \le \vec{f}$ ?
    - If yes:
        - Mark thread $i$ as *"terminated"*
        - and add all resources occupied by it to the vector of free resources: $\vec{f} \leftarrow \vec{f} + \vec{b}_i$
    - If no:
        - Continue with the next thread $i+1$
- Terminate the algorithm if all threads are marked as *"terminated"* or no thread can be marked.
- The situation is safe if all threads have been marked as *"terminated"* and unsafe if not.

## Semaphores & Monitors

### Semaphores

• A semaphore is a generalization of a lock variable, essentially a counting lock (counting variable + queue).
• Unlike a lock, semaphores can provide access to the critical section for more than one thread or process.

#### Example implementation in C

```c
struct semaphore {
  int count;    // count of available resources
  Queue *queue; // count >= 1: free, count <= 0: occupied
};              // if count < 0: |count| = |queue| is the number of waiting threads

void init(struct semaphore *s, int i) {
  s->count = i;    // set i = 1 for mutual exclusion
  s->queue = NULL; // start with empty queue
}

void wait(struct semaphore *s) { // unhelpfully called P in our slides
  s->count -= 1;     // decrement count of available resources
  if (s->count < 0) {
    block(s->queue); // enqueue thread
  }
}

void post(struct semaphore *s) { // unhelpfully called V in our slides
  s->count += 1;       // increment count of available resources
  if (s->count <= 0) {
    unblock(s->queue); // unblock first of queue
  }
}
```

#### Example execution

The following example shows two threads $P_1$ and $P_2$ using a semaphore `sem` with initial value 2.

|     Operation      | `count` | `queue` |  $P_1$  |  $P_2$  |
| :----------------: | :-----: | :-----: | :-----: | :-----: |
|                    |    2    |  empty  | execute | execute |
| $P_1$: `wait(sem)` |    1    |  empty  | execute | execute |
| $P_2$: `wait(sem)` |    0    |  empty  | execute | execute |
| $P_1$: `wait(sem)` |   -1    | $(P_1)$ | blocked | execute |
| $P_2$: `post(sem)` |    0    |  empty  | execute | execute |
| $P_1$: `post(sem)` |    1    |  empty  | execute | execute |
| $P_1$: `post(sem)` |    2    |  empty  | execute | execute |

#### POSIX Semaphores

```c
#include <semaphore.h>

// Initialization of a semaphore
int sem_init(sem_t *sem, int pshared, unsigned int value);

// Decrement counter and block thread if necessary
int sem_wait(sem_t *sem);

// Increment counter and unblock the first queued thread
int sem_post(sem_t *sem);

// Decrement counter and return without blocking. Return value indicates 
// the success of the attempt to get access to the critical section.
int sem_trywait (sem_t *sem);

// Decrement counter and block thread if necessary, with abort after a given time.
int sem_timedwait (sem_t *sem, const struct timespec *abs_timeout);
```

#### Producer and Consumer

...

#### Cigarette Smokers' Problem

...

#### Readers and Writers

...

### Monitors

- A **monitor** is an object that guarantees mutual exclusion without requiring the programmer to explicitly insert lock and unlock operations.
- It consists of procedures (methods/functions) and data structures that ensures that at any time it is used by not more than one thread.

## OpenMP

### Matrix Multiplication Example

```c
// Matrix Multiplication

#include <omp.h>
#include <stdio.h>

double MA[100][100], MB[100][100], MC[100][100];
int i, row, col, size = 100; // <- Bad example from slides, count variables should be
                             //    declared close to usage, see comment in line 32ff.

// Initialise matrices MA and MB
void init_matrices() {
  // Init MA
  for (i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++) {
      MA[i][j] = (double)(i + j) + 1.0;
      MB[i][j] = (double)(i + j) + 1.0;
    }
}

// Print resulting matrix MC
void print_MC() {
  for (i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++)
      printf("%f ", MC[i][j]);
  printf("\n");
}

int main() {
  init_matrices();

  // Run next block concurrently where each
  // thread gets its own row, col, and i
#pragma omp parallel private(row, col, i) // <- Bad example because count variables
  {                                       //    are automatically private and should
      									  //    be declared as "for (int i = 0; ...)"
                                          //    in modern C style
    // Concurrent FOR loop where each threads gets
    // assigned a constant number of iterations
#pragma omp for schedule(static)
    for (row = 0; row < size; row++) {
      // Threads share access to matrices and size
#pragma omp parallel shared(MA, MB, MC, size)
      {
        // Same as previously, concurrent FOR loop
#pragma omp for schedule(static)
        for (col = 0; col < size; col++) {
          MC[row][col] = 0.0;
          for (i = 0; i < size; i++)
            MC[row][col] += MA[row][i] * MB[i][col];
        }
      }
    }
  }
  print_MC();
  return 0;
}
```

### Nested Parallel Regions

```c
#include <omp.h>
#include <stdio.h>

// Let a single thread print its team's thread count
void report_num_threads(int level) {
#pragma omp single
  printf("Level %d:\t%d threads in team.\n", level, omp_get_num_threads());
}

int main(void) {
  // Allow nested thread creation
  omp_set_nested(1);
#pragma omp parallel num_threads(2)
  {
    // Print level 1 thread count
    report_num_threads(1);
#pragma omp parallel num_threads(2)
    {
      // Print level 2 thread count
      report_num_threads(2);
#pragma omp parallel num_threads(2)
      // Print level 3 thread count
      report_num_threads(3);
    }
  }
}

/* PRINTS OUT:
Level 1:	2 threads in team.
Level 2:	2 threads in team.
Level 2:	2 threads in team.
Level 3:	2 threads in team.
Level 3:	2 threads in team.
Level 3:	2 threads in team.
Level 3:	2 threads in team.
/*
```

## Parallel Programming with Message Passing

- Parallel programming with message passing
- Foster's Design Methodology

## MPI – Message Passing Interface

...

## MPI Group Communication and MPI 2

...

## Design and Implementation of Parallel Applications

...

## From Concurrent to Parallel Programming

...

## Communication Paradigms in Distributed Systems

...

## Interprocess Communication

...

### Remote Invocation

#### Distribution Transparency

- Access Transparency
- Location Transparency
- Migration Transparency
- Replication Transparency

### RMI - Remote Method Invocation

...

### Java RMI

... 

## Peer-to-Peer Systems

...

### Basics

...

### Centralized, Pure and Hybrid P2P Systems

...

### DHT-based P2P Systems

...

### P2P-based Whiteboard

...

## Cloud Computing

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

# [Tutoriums- und Klausurfragen](questions.html)➡️



----

[^yt_1]: [YouTube: Dining Philosophers](https://www.youtube.com/watch?v=_ruovgwXyYs)