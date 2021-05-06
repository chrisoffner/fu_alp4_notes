# Nichtsequentielle und Verteilte Programmierung

## Nonsequential programming paradigms

- **Concurrent programming**
  Machine with one CPU
- **Parallel programming**
  Machine with several CPUs with or without shared memory
  machine, which requires message exchange
- **Distributed programming**
  Different machines

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

A **deterministic algorithm** is an algorithm which, given a specific input, will always produce the same specific output and will always change the state of the system in the same way (and order).

---

A **determined algorithm** is an algorithm which, given a specific input, produces always the same specific output.

---

**Q: What ensures the sequential execution of a program?**

*A: The assembly code of a program gets executed sequentially due to the automatic incrementation of the instruction pointer.*

---
## Concurrency







## Threads

...

## Petri-Netze

...

## Deadlocks

...

## Semaphor

...

## Monitor

...

## OpenMP

...

## MPI

...

