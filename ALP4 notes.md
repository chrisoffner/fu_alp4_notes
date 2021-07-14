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

...

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
};              // if count < 0: |count| is the number of waiting threads

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

...

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

# Questions

## Deadlocks

----

**Welche vier Bedingungen müssen gelten, damit Deadlocks in einem System auftreten können?**

1. **Mutual Exclusion:** Ressourcen werden exklusiv genutzt.
2. **Hold and Wait:** Prozesse/Threads beanspruchen Zugriff auf Betriebsmittel und fordern zusätzlich Zugriff auf weitere an.
3. **No Preemption:** Betriebsmittel werden ausschließlich durch die Prozesse/Threads freigegeben, die sie nutzen. 
4. **Circular wait:** Der Wait-For-Graph enthält einen Kreis.

----

**Inwiefern unterscheidet sich Deadlock Avoidance von Deadlock Prevention?**

- Bei **Deadlock Avoidance** soll vermieden werden, dass ein System in einen *unsicheren Zustand* eintritt. Ein *unsicherer Zustand* besteht, wenn mindestens ein Thread noch ausstehende Zugriffsanfragen hat, die nicht mit den aktuell freien Betriebsmitteln befriedigt werden können.

    - Ergreift Maßnahmen zur Vermeidung eines unsicheren Zustands basierend auf Kenntnis zukünftiger Betriebsmittelverfügbarkeit und -anfragen.
    - Nutzt Bankiersalgorithmus.

- Bei **Deadlock Prevention** wird die Belegung von Betriebsmitteln so eingeschränkt, dass mindestens eine notwendige Bedingung für einen Deadlock gar nicht erst eintreten kann.

    - Keine Kenntnis über zukünftige Betriebsmittelverfügbarkeit und -anfragen nötig.

----

**Skizziere einen Deadlock in C.**

...

----

**Wie ist ein sicherer Zustand definiert?**

...

----

**Ist jeder Deadlock ein unsicherer Zustand? Warum?**

...

----

**Ist jeder unsichere Zustand ein Deadlock? Warum?**

...

----

**Gegeben sei der folgende Systemzustand:**
$$
R = \begin{pmatrix}2 & 1 & 2\\1&1&1\\2&2&0\\0&0&1\end{pmatrix},\quad G=\begin{pmatrix}4&1&2\\3&2&2\\2&2&1\\1&1&4\end{pmatrix},\quad f=\begin{pmatrix}1&0&1\end{pmatrix}
$$
**(i) Berechne mit dem Bankieralgorithmus, ob der angegebene Systemzustand *sicher* ist.**

...

**(ii) Angenommen der Thread $T_3$ stellt die Teilanforderung $\begin{pmatrix}1&0&0\end{pmatrix}$ an das System. Ist der Systemzustand nach der Erfüllung dieser Teilanforderung sicher? Sollte die Teilanforderung von $T_3$ erfüllt werden?**

...

**(iii)  Beantworte die vorherige Teilaufgabe für den Thread $T_2$ und die Teilanforderung $\begin{pmatrix}0&0&1\end{pmatrix}$.**

...

----

**Analysiere die Laufzeit des Bankieralgorithmus.**

...

----

**Wie praktikabel ist eigentlich der Bankieralgorithmus?**

...

----

## Semaphoren und Monitore

----

**Definiere eine Semaphore. Welche Operationen sind für eine Semaphore definiert?**

...

----

**Ist es wichtig, dass die Operationen aus der vorherigen Frage atomar ausgeführt werden?**

...

----

**Vergleiche Mutexe und Semaphoren. Was sind Gemeinsamkeiten und Unterschiede?**

...

----

**Was ist eine binäre Semaphore? Ist eine binäre Semaphore und ein Mutex dasselbe?**

...

----

**Beschreibe den Aufbau eines Monitors:**

- **Welche Datenstrukturen können zu einem Monitor gehören?**

- **Welche Operationen sind für einen Monitor definiert?**

...

----

**Skizziere eine Lösung in Pseudocode zu der folgenden Aufgabe[^1]:**

**Auf eine Datei wird durch mehrere Threads ausschließlich lesend zugegriffen. Die TIDs der Threads seien durch paarweise verschiedene natürliche Zahlen gegeben. Zu keinem Zeitpunkt darf jedoch die Summe der TIDs derjenigen Threads, welche aktuell auf die Datei zugreifen, eine initial fest gewählte Schranke $N \in \N$ überschreiten. Entwickle einen Monitor, welcher einen gemeinsamen Zugriff auf die Datei unter der eben genannten Einschränkung realisiert.**

...

----

## Parallel Applications

**Erkläre Foster’s Design Methodology. Welche vier Schritte werden dort genannt und was ist das Ziel jedes einzelnen Schritts?**

...

**Erkläre das BSP Model.**

...

**Angenommen man möchte grob überschlagen, ob es sich lohnt ein bestimmtes serielles Programm zu parallelisieren. Durch Benchmarks hat man ermittelt, dass 80% der Ausführungszeit in Funktionen verbracht wird, die sich gut parallelisieren lassen. Die restlichen 20% der Ausführungszeit wird in Codeabschnitten verbracht, die unbedingt seriell ausgeführt werden müssen (z.B. kritische Abschnitte).**

**(i) Wie hoch ist der erwartete Speed-Up für 6 Prozessoren?**

...

**(ii) Wie hoch ist der erwartete Speed-Up für beliebig viele Prozessoren?**

...

**Ist Amdahl’s Law eine realitätsnahe Abschätzung des zu erwartenden Speed-Up für die Parallelisierung eines Programms?**

...

**Gegeben seien n ganze Zahlen $a_1, a_2, . . ., a_n$. Entwickle mithilfe von *Foster’s Design Methodology* einen parallelen Algorithmus, welcher $\mathrm{min}_{i=1,...,n}a_i$ berechnet.**

...

## OpenMP

**Mit welcher Option kann OpenMP in gcc aktiviert werden?**

...

**Was bedeutet Functional Parallelism?**

...

**Was ist eine Barriere?**

...

**Was ist der Unterschied zwischen `#pragma omp single` und `#pragma omp critical`?**

...

**In der Ankündigung befindet sich ein C-Programm `sum.c`, welches die Summe $\sum_{i=1}^{5.000.000}i$ berechnet. Parallelisiere das Programm mit OpenMP. Das Ergebnis sollte mit der seriellen Version immer noch übereinstimmen. Es steht auf Seite 2 ein Tipp, falls man einen braucht.**

...

**Die Menge ${1, 2, . . . , 20}$ enthält die Primzahlen $2,3,5,7,11,13,17,19$. Die größte Lücke zwischen zwei aufeinanderfolgenden Primzahlen ist hier $4 = 17 - 13 = 11 - 7$. In der Ankündigung befindet sich ein C-Programm `primes_gaps.c`, welches das oben beschriebene Problem für die Menge ${1, 2, . . . , 600000}$ löst. Die Lösung des Problems ist in diesem Fall $114 = 492227 − 492113$.**

**(i)  Parallelisiere das Programm mit OpenMP. Das Ergebnis sollte immer noch 114 sein. Es stehen auf Seite 2 einige Tipps, falls etwas Hilfestellung benötigt wird.**

...

**(ii)  Vergleiche die sequentielle Version aus der Ankündigung und die parallelisierte Version aus Teilaufgabe (i) bezüglich der Ausführungszeit (beispielsweise mit time im Terminal).**

...

**(iii)  Ist der erhaltene Speed-Up zufriedenstellend?**

...

## MPI

**Kompiliere die Datei `mpi_hello_world.c` auf `andorra.imp.fu-berlin.de` und führe das kompilierte Programm mittels `mpirun` aus. Verwende als Argumente `-machinefile Machinefile -np 12`. Die Dateien `mpi_hello_world.c` und `Machinefile` befinden sich im Anhang.**

...

**Vergleiche synchrone Kommunikation mit asynchroner Kommunikation und nenne passende Beispiele aus der Vorlesung über MPI.**

...

**Vergleiche `MPI_Gather` und `MPI_Allgather`.**

...

**Betrachte die Datei `mpi_share_input.c`. Das Programm soll einen String von der Kommandozeile lesen und anschließend soll der Knoten mit Rank 0 diesen String an alle anderen Knoten verteilen. Ergänze in der Datei `mpi_share_input.c` ausschließlich im Bereich "//LÖSUNG ANFANG ... //LÖSUNG ENDE" *MPI Befehle*, sodass das Programm die Aufgabe löst. Kein Prozess sollte beim zweiten `printf(...)` den Text `FIX ME!!!` ausgeben.**

...

**Die Datei `mpi_calc_pi.c` enthält eine angepasste Version von Beispiel *09-06* aus der Vorlesung. Dort wird $\pi$ approximiert, indem man $\int_{0}^1 \frac{4}{1+x^2}dx$ numerisch integriert. Verteile die numerische Berechnung auf mehrere Knoten und verwende am Schluss `MPI_Reduce`, um $\pi$ zu approximieren.**

...

**Die Aufgabe von `mpi_calc_max.c` ist es das Maximum von einem Array mit 100000000 vielen 16-Bit Integer zu berechnen. Teile das Problem mittels *MPI* auf verschiedene Knoten auf. Das Ergebnis ist immer eine fünfstellige Zahl und eindeutig.**

...

**Ist es sinnvoll, OpenMP und MPI zu kombinieren?**

...

## Verteilte Systeme I

**Wie definiert man einen eigenen Thread in Java?**

...

**Was ist (Un-)Marshalling?**

...

**Was ist eine Middleware? Gib ein Beispiel für eine Middleware.**

...

**Vergleiche IPC und RPC.**

...

**Skizziere die Implementierung eines Remote Procedure Calls.**

...

**Vergleiche At-least-once semantics und At-most-once semantics bei RPC Calls. In welchen Szenarien würde man welche RPC Call Semantics bevorzugen?**

...

**Betrachte das Beispiel RPCExample im Github-Repository https://github.com/FUB-HCC/WhiteBoard-Implementation-Examples und erkläre, wie das Beispiel funktioniert.**

...

## Wiederholungsfragen

----

**Es gibt Algorithmen, die...**

- ...deterministisch und determiniert sind. ✅
- ...deterministisch und NICHT determiniert sind. ❌
- ...NICHT deterministisch und determiniert sind. ✅
- ...weder deterministisch noch determiniert sind. ✅

----

**Welche der Aussagen über Prozesse und Threads sind korrekt?**

- Im Gegensatz zu Prozessen haben Threads einen eigenen Adressraum. ❌
- Neben dem Adressraum teilen sich mehrere Threads den Stack vom Prozess. ❌
- Ein Kontextwechsel zwischen zwei Threads im selben Prozess ist in der Regel effizienter als ein Kontextwechsel zwischen zwei verschiedenen Prozessen. ✅
- Ein Prozess hat immer einen eigenen PCB. ✅
- Bei der Verwendung von Prozessen können keine kritische Abschnitte entstehen. ❌
- Prozesse werden auf POSIX Systemen mit dem Syscall `create_process()` erzeugt. ❌

----

**Welche der Aussagen über Lockmechanismen sind korrekt?**

- POSIX-Mutexe funktionieren auf einem modernen Maschinenmodell (Pipelining, Out-of-Order Execution, Caching usw.) mit einer einzigen CPU. ✅
- In der Vorlesung wurden exakt vier Kriterien vorgestellt, die ein "guter" Lock erfüllen muss. ❌
- Die Korrektheit von einem Lockmechanismus hängt vom Maschinenmodell ab. ✅
- Ob (Maschinen-)Instruktionen atomar ausgeführt werden, ist für die Korrektheit eines Locks vollkommen irrelevant. ❌

----

**Welche der Aussagen über Deadlocks sind korrekt?**

- Ein kritischer Abschnitt lässt sich trivial absichern, indem man vor dem Eintritt in den kritischen Abschnitt einen Deadlock erzeugt. ✅
- Ein Deadlock ist ein unsicherer Zustand. ✅
- Mit dem Bankieralgorithmus sorgt man dafür, dass eine der vier Bedingungen für das Auftreten von Deadlocks nicht gilt. ❌
- Beim Dining Philosophers Problem (KEINE SHARED Chopsticks!) lässt sich ein Deadlock vermeiden, indem jeder Philosoph zuerst sein rechtes Essstäbchen aufnimmt und dann sein linkes Essstäbchen bis auf einen Philosophen, der zuerst sein linkes Essstäbchen aufnimmt und anschließend sein rechtes Essstäbchen. Dieses Verfahren ist der Deadlock Prevention zuzuordnen, weil keine Zyklen im Wait-for-Graphen entstehen können. ⁉️
- Ein Deadlock liegt genau dann vor, wenn es einen Zyklus in einem Resource-Allocation-Graphen gibt. ❌

----

**Welche der Aussagen über den Bankieralgorithmus sind korrekt?**

- Der Bankieralgorithmus wird in *Deadlock Avoidance* und *Deadlock Resolution* angewandt. ❌
- Die Laufzeit des Bankieralgorithmus ist $\mathcal{O}(nk^2)$, wobei $n$ = Anzahl Resourcen und $k$ = Anzahl Threads. ✅
- Der Bankieralgorithmus entspricht einer iterativen Reduktion von einem Resource Allocation Graph. Bei einem sicheren Zustand kann man den ursprünglichen Resource Allocation Graph zu einem Graphen mit leerer Kantenmenge reduzieren. ✅
- Ein unsicherer Zustand führt zwangsweise zu einem Deadlock. ❌

----

**Ist dieser Systemzustand sicher?**
$$
B := \begin{pmatrix}0&4\\1&0\\1&0\\2&4\\3&3\end{pmatrix}, \qquad G := \begin{pmatrix}4&6\\2&4\\3&1\\10&6\\4&4\end{pmatrix}, \qquad \vec{f} := \begin{pmatrix}3&2\end{pmatrix}
$$

- Ja ✅
- Nein ❌

*Begründung durch Ausführung des Bankiersalgorithmus:*
$$
\begin{align*}
R &= G - B = \begin{pmatrix}4&2\\1&4\\2&1\\8&2\\1&1\end{pmatrix}\\
\begin{pmatrix}4&2\end{pmatrix} = \vec{r}_1 &\not\le \vec{f} = \begin{pmatrix}3&2\end{pmatrix}\\
\begin{pmatrix}1&4\end{pmatrix} = \vec{r}_2 &\not\le \vec{f} = \begin{pmatrix}3&2\end{pmatrix}\\
\begin{pmatrix}2&1\end{pmatrix} = \vec{r}_3 &\le \vec{f} = \begin{pmatrix}3&2\end{pmatrix}\quad\checkmark\\
\vec{f} &\leftarrow \vec{f} + \vec{b}_3\quad\implies\quad \vec{f} = \begin{pmatrix}4&2\end{pmatrix}\\\\
\begin{pmatrix}4&2\end{pmatrix} = \vec{r}_1 &\le \vec{f}=\begin{pmatrix}4&2\end{pmatrix}\quad\checkmark\\
\vec{f} &\leftarrow \vec{f} + \vec{b}_1\quad\implies\quad \vec{f} = \begin{pmatrix}4&6\end{pmatrix}\\\\
\begin{pmatrix}1&4\end{pmatrix} = \vec{r}_2 &\le \vec{f}= \begin{pmatrix}4&6\end{pmatrix}\quad\checkmark\\
\vec{f} &\leftarrow \vec{f} + \vec{b}_2\quad\implies\quad \vec{f} = \begin{pmatrix}5&6\end{pmatrix}\\\\
\begin{pmatrix}8&2\end{pmatrix} = \vec{r}_4 &\not\le \vec{f}= \begin{pmatrix}5&6\end{pmatrix}\\
\begin{pmatrix}1&1\end{pmatrix} = \vec{r}_5 &\le \vec{f}= \begin{pmatrix}5&6\end{pmatrix}\quad\checkmark\\
\vec{f} &\leftarrow \vec{f} + \vec{b}_5\quad\implies\quad \vec{f} = \begin{pmatrix}8&9\end{pmatrix}\\\\
\begin{pmatrix}8&2\end{pmatrix} = \vec{r}_4 &\le \vec{f}= \begin{pmatrix}8&9\end{pmatrix}\quad\checkmark\\
\vec{f} &\leftarrow \vec{f} + \vec{b}_4\quad\implies\quad \vec{f} = \begin{pmatrix}10&13\end{pmatrix}\\\\
\end{align*}
$$
Die Reihenfolge $(T_3, T_1, T_2, T_5, T_4)$ ermöglicht einen Deadlock-freien Programmablauf.

----

**Welche der Aussagen über Semaphoren sind korrekt?**

- Eine Semaphore besteht aus einem Zähler und einer Datenstruktur für die wartenden Threads/Prozesse. ✅
- Der Zähler einer Semaphore ist immer nichtnegativ. ❌
- Eine Semaphore kann man als eine Erweiterung von einem Lock interpretieren. ✅
- Die Funktion `sem_post()` kann nur von einem Thread aufgerufen werden, der sich gerade in einem kritischen Abschnitt befindet. ❌
- Ein Vorteil von Semaphoren gegenüber Locks ist, dass Deadlocks bei Semaphoren nie auftreten können. ❌

----

## Erstklausur 2020

### Grundlagen (10 Punkte)

**Nebenläufige Programmierung (8 Punkte)**

**Sie programmieren ein Programm mit mehreren Threads, das auf einem System ausgeführt werden soll, welches eine nebenläufige Ausführung unterstützt. Worauf müssen Sie achten? Gehen Sie hierbei auf die Begriffe Korrektheit, deterministische Ausführung, determinierte Ausführung und kritischen Abschnitt im Sinne der Vorlesung ein.**

...

----

**Programmiermodell (2 Punkte)**

**Welche Modelle nutzen Sie bei der Programmierung und in welcher Beziehung müssen diese zu einander stehen, damit eine korrekte Ausführung im Sinne der Vorlesung sichergestellt ist?**

...

----

### Gegenseitiger Ausschluss (16 Punkte)

**Implementierung einer Lösung (8 Punkte)**

**Gegeben sei die folgende Lösung zur Implementierung des gegenseitigen Ausschlusses für zwei Threads. Ist diese Lösung im Sinne der Vorlesung geeignet den gegenseitigen Ausschluss verlässlich sicherzustellen? Erfüllt die Lösung auch die anderen Anforderungen an Lösungen zum Schutz des kritischen Abschnitts, die in der Vorlesung genannt wurden? Begründen Sie Ihre Antwort.**

```c
#include ...
#define NUM_THREADS 2
char _lock[2];


int lock (long tid) {
    _lock[tid] = 1;

    while (_lock[NUM_THREADS - 1 - tid]) {
        _lock[tid] = 0;
        sleep (1);
        _lock[tid] = 1;
        }

return 0;
}

int unlock (long tid) {
    
    _lock[tid] = 0;
    return 0;
}
```

...

----

**Erweiterung einer Lösung (4 Punkte)**

**Wenn die dargestellte Lösung nicht alle Anforderungen im Sinne der Vorlesung erfüllt, welche der in der Vorlesung vorgeschlagenen Lösungen, die nicht auf Betriebssystem- unterstützung zurückgreifen, würden Sie wählen? Begründen Sie Ihre Antwort.**

***Hinweis:***
***Sie können die Lösung beschreiben oder kurz skizzieren, indem Sie die Änderungen am Quell-Code der obigen Lösung angeben.***

...

----

**Unterstützung für die Lösung (4 Punkte)**

**Wann und warum kann es notwendig sein, Hardware- und Betriebssystemunterstützung für den Schutz des kritischen Abschnitts durch gegenseitigen Ausschluss einzufordern?**

...

----

### Modellierung (2 Punkte)

**Was modelliert dieses Petri-Netz?**

<img src="ALP4 notes.assets/producerconsumernet.png" alt="producerconsumernet" style="zoom: 33%;" />

...

----

### Verklemmungen (18 Punkte)

**a) Bedingungen (4 Punkte)**

**Nennen Sie alle vier Bedingungen für eine Verklemmung!**

...

----

**b) Art der Ressource (3 Punkte)**

**Wenn wir die CPU als Ressource betrachten, wann kann es nicht zu Verklemmungen kommen? Begründen Sie Ihre Antwort.**

...

----

**c) Bankeralgorithmus (10 Punkte)**

**In einem System mit 5 Prozessen und 4 Betriebsmitteln liege die folgende Betriebsmittelsituation vor mit $G$ für die Gesamtanforderungen, $B$ für die aktuell belegten Ressourcen und $\vec{v}$ als Vektor der grundsätzlich verfügbaren Ressourcen. Wenden Sie den Bankeralgorithmus entsprechend der Vorlesung an, um festzustellen, ob die Situation sicher ist. Geben Sie nach jedem Schritt den Vektor $\vec{f}$ der freien Betriebsmittel, sowie die Menge $DP$ der noch nicht terminierten Prozesse an.**
$$
G:= \begin{pmatrix}2&1&0&0\\5&4&2&1\\6&6&3&2\\5&3&1&3\\5&6&2&3\end{pmatrix},\qquad B:= \begin{pmatrix}2&0&0&0\\3&3&2&1\\1&1&1&1\\1&0&0&1\\1&2&0&2\end{pmatrix},\qquad \vec{v}:=\begin{pmatrix}8&7&3&5\end{pmatrix}
$$
...

----

**d) Bankeralgorithmus II (1 Punkt)**

**Verändern Sie anschließend $\vec{v}$ derart, dass aus einer sicheren eine unsichere bzw. aus einer unsicheren eine sichere Situation wird, während die Gesamtanzahl der verfügbaren Ressourcen $\vec{v}$ unverändert bleibt!**

...

----

### OpenMP (6 Punkte)

**Ergänzen Sie das folgende Programm so, dass die jeweils äußeren `for`-Schleifen parallel ausgeführt werden. Nutzen Sie hierfür OpenMP und die geeignete Befehle aus der gegebenen Auswahl.**

***Hinweise:***
***Achten Sie darauf, welche Variablen wo benutzt werden sollen. Sie können die entsprechenden Nummern für die Quellcode-Zeile und den Befehl angeben. Es dürfen Befehle mehrfach verwendet werden. Nicht alle Befehle müssen eingesetzt werden.***

**Befehle zur Implementierung der Lösung:**

```c
#pragma omp single
#pragma omp parallel
#pragma omp parallel shared (MA, MB, MC, size) private (row, col, i)
#pragma omp parallel privat (MA, MB, MC, size) shared (row, col, i)
#pragma omp for schedule (dynamic)
#pragma omp for schedule (static)
#pragma omp parallel for
#pragma omp parallel for private (x) reduction (+:area)
#pragma omp parallel sections
#pragma omp section
#pragma omp parallel end
#pragma omp barrier
```

````c
#include <omp.h>
double MA[100][100], MB[100][100], MC[100][100];
int i, row, col, size = 100;

int main () {
    read_input (); // MA, MB

    {
        
        for (row = 0; row < size; row++) {
            for (col = 0; col < size; col++)
                MC[row][col] = 0.0;
        }

        for (row = 0; row < size; row++) {
            for (col = 0; col < size; col++)
                for (i = 0; i < size; i++)
                    MC[row][col] += MA[row][i] * !???!
        }
    }
  
    write_output (); // MC
    return 0;
}

````

----

### Parallele Programmierung (8 Punkte)

**Nennen und erläutern Sie kurz im Sinne der Vorlesung die Schritte des Fosterschen Vorgehensmodells zur Entwicklung paralleler Programme am Beispiel der Implementierung eines parallelen Programms zur Matrizenmultiplikation.**

...

----

### Kommunikation mit Sockets (4 Punkte)

**Java stellt Bibliotheksklassen für die Benutzung von Sockets zur Verfügung. Skizzieren Sie den Code bis zu den Stellen, an denen Client und Server mit dem Austausch ihrer Anwendungsdaten beginnen! Achten Sie darauf, dass Client und Server Aktivitäten teilweise aufeinander aufbauend vornehmen und berücksichtigen Sie die zeitliche Reihenfolge.**

**Befehle zur Implementierung der Lösung:**

```java
Socket s = new Socket(clientURL, clientPort);
Socket s = new Socket(serverURL, serverPort);
Socket s = client_s.accept();
Socket s = server_s.accept();
ServerSocket client_s = new ClientSocket(port);
ServerSocket server_s = new ServerSocket(port);
InputStream in = s.getInputStream();
InputStream in = s.getInputStream(s);
OutputStream out = s.getOutputStream();
OutputStream out = s.getOutputStream(s);
```

| Client | Server |
| ------ | ------ |
|        |        |
|        |        |
|        |        |
|        |        |
|        |        |
|        |        |
|        |        |
|        |        |

----

### JavaRMI (12 Punkte)

**Betrachten Sie folgende Implementierung in Java.**

```java
import java.rmi.*;
public interface Abs extends Remote {
    public double abs(Complex x) throws RemoteException;}

import java.rmi.*;
public class AbsImpl implements Abs {
    public double abs(Complex x) {
        return Math.sqrt(x.re * x.re + x.im * x.im);}}

import java.io.Serializable;
public class Complex implements Serializable {
    public double re, im;
    public Complex(double re, double im) {
        this.re = re;
        this.im = im;}}
```

**Ausgabe (4 Punkte)**

**Betrachten Sie folgende Zeile Code:**

```java
System.out.println(a.abs(new Complex(3, 4)));
```

**Wobei `a` ein lokales Objekt der `AbsImpl`-Klasse ist. Was wird auf der Konsole ausgegeben, wenn man die Zeile ausführt?**

...

----

**Ablauf (6 Punkte)**

**Beschreiben Sie in wenigen kurzen Sätzen, was (bezogen auf RMI) passiert, wenn das Objekt `a` aus a) auf ein entferntes `AbsImpl`-Objekt verweist und die Zeile Code ausgeführt wird.**

...

----

**Nutzung nicht-serialisierbarer Objekte (2 Punkte)**

**Was passiert bei der Ausführung der Zeile Code aus a), wenn `a` auf ein entferntes `AbsImpl`-Objekt verweist und die Klasse `Complex` NICHT das Interface `Serializable` implementiert?**

...

----

### Web-Programmierung (6 Punkte)

**Analysieren Sie die folgende Webseite und beantworten Sie diese Fragen:**

```html
<HTML>
    <BODY>
        <SCRIPT language="JavaScript">
        function onSubmit() {
            if(document.pressed == 'Insert')
                document.myForm.action = "insert.html";
            else if(document.pressed == 'Update')
                document.myForm.action = "update.html";
            return true; }
        </SCRIPT>
        <FORM name="myForm" onSubmit="return onSubmit();">
            <INPUT type="submit" name="Operation" value="Insert"
                onClick="document.pressed=this.value" >
            <INPUT type="submit" name="Operation" value="Update"
                onClick="document.pressed=this.value" >
    </BODY>
</HTML>
```

**Erscheinungsbild (1 Punkt)**
**Wie ist das Erscheinungsbild der Webseite? Zeichnen Sie eine Skizze oder beschreiben Sie.**

...

----

**Actions (2 Punkte)**
**Welche actions kann der Benutzer auslösen, und was passiert dann?**

...

----

**Objektzuordnung (1 Punkt)**
**Auf welches DOM-Objekt bezieht sich in `<input …>` das jeweilige `this`?**

...

----

**Eigenschaften des Objekts (1 Punkt)**

**Welche Eigenschaften hat dieses Objekt (jeweils Name und Wert)?**

…

----

**Ersetzung (1 Punkt)**
**Kann man "pressed" überall durch "blablabla" ersetzen? Begründung!**

...

----

### Services (6 Punkte)

**Im Teil der verteilten Entwicklung haben wir immer wieder den Begriff Dienst verwendet, um damit Frage/Antwort-Beziehungen zwischen Client und Servern in verschiedenen Kontexten: (1) TCP Sockets, (2) RMI und (3) Web-Dienste.**

**Nennen und erläutern Sie jeweils mindestens einen grundsätzlichen Unterschied (je 2 Punke) zwischen:**

1. **a)  (1) und (2),**
2. **b)  (2) und (3),**
3. **c)  (1) und (3).**

...

----

### Soundcloud (2 Punkte)

**Anhand von Soundcloud haben wir eine typische Cloud-basierte Datenanalyseplattform kennengelernt. Nennen Sie zwei Gründe, warum sich das Data Analytics Team für die Verwendung einer Cloud-Lösung für die Verwaltung seiner Daten entschieden hat.**

...

----

[^1]: siehe Aufgabe 6.29 in Silberschatz, Abraham, Peter B. Galvin, and Greg Gagne. Operating System Concepts / Abraham Silberschatz ; Peter Baer Galvin ; Greg Gagne. 10. ed.

[^yt_1]: [YouTube: Dining Philosophers](https://www.youtube.com/watch?v=_ruovgwXyYs)