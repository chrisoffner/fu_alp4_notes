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

## 5 Parallelization

...

## 6 Petri Nets

...

## 7 Deadlocks

> ## Deadlock
>
> A **deadlock** is a program state in which execution cannot proceed because at least two threads are waiting for the release of a resource that the other thread currently holds. A deadlock is characterised by a cycle in the wait-for graph.

### Requirements for a Deadlock

1. **Mutual Exclusion:** Ressourcen werden exklusiv genutzt.
2. **Hold and Wait:** Prozesse/Threads beanspruchen Zugriff auf Betriebsmittel und fordern zusätzlich Zugriff auf weitere an.
3. **No Preemption:** Betriebsmittel werden ausschließlich durch die Prozesse/Threads freigegeben, die sie nutzen. 
4. **Circular Wait:** Der Wait-For-Graph enthält einen Kreis.

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

## 8 Semaphores & Monitors

### Semaphores

...

### Monitors

...

## 9 OpenMP

### Matrix Multiplication

```c
// Matrix Multiplication

#include <omp.h>
#include <stdio.h>

double MA[100][100], MB[100][100], MC[100][100];
int i, row, col, size = 100;

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
#pragma omp parallel private(row, col, i)
  {
    // Concurrent FOR loop where each threads gets
    // assigned a constant number of iterations
#pragma omp for schedule(static)
    for (row = 0; row < size; row++) {
      // All matrices share access to matrices and size
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

# Questions

## 1 Deadlocks

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

**Analysiere die Laufzeit vom Bankieralgorithmus.**

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

**Erkläre Foster’s Design Methodology! Welche vier Schritte werden dort genannt und was ist das Ziel jeden einzelnen Schritts?**

...

**Erkläre das BSP Model!**

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

----

[^1]: siehe Aufgabe 6.29 in Silberschatz, Abraham, Peter B. Galvin, and Greg Gagne. Operating System Concepts / Abraham Silberschatz ; Peter Baer Galvin ; Greg Gagne. 10. ed.
