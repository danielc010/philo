*This project has been created as part of the 42 curriculum by dachin.*

# Philosophers

## DESCRIPTION

Philosophers is designed around a classic problem (Edsger Dijkstra's famous dining philosophers problem) in computer science  which deals with concurrent programming. The learning objectives are learning about threads, mutexes, deadlocks, data races, and initialisation orders. The project simulates threads (philosophers) sharing common resources of memory (forks) and going through actions that will determine their fate.

### Key Concepts

* Threads share memory within a process, enabling concurrency but also races
* Mutexes protect shared data by ensuring only one thread accesses it at a time
* Deadlock happens when a thread waits for a lock it already holds — fix by releasing before re-acquiring
* Data race vs deadlock — wrong results vs program hangs
* Dining philosophers — even/odd fork ordering prevents deadlock, staggering prevents starvation
* Initialization order — dependencies must be satisfied before the code that needs them runs

---

## INSTRUCTIONS

### Compilation

Compile the program using the Makefile:

    make

Remove object files:

    make clean

Remove object files and the executable:

    make fclean

Recompile everything:

    make re

### Execution

Run the program:

./philo number_of_philosophers time_to_die time_to_eat time_to_sleep
      [number_of_times_each_philosopher_must_eat]

e.g. ./philo 2 800 200 200

---

## RESOURCES

* Philosophers — Dining Philosophers problem - Dina ELgammal:
  https://medium.com/@denaelgammal/dining-philosophers-problem-42-project-guide-mandatory-part-a20fb8dc530e

* Linux man pages:
  https://man7.org/linux/man-pages/

* DINING-PHILOSOPHERS PROBLEM: SIMPLIFIED — Zaynab Zakiyah
  https://www.youtube.com/watch?v=VSkvwzqo-Pk

*I ntroduction to Parallel Computing Tutorial:
  https://hpc.llnl.gov/documentation/tutorials/introduction-parallel-computing-tutorial#Whatis

* 42 Philosophers Tester:
  https://github.com/dantonik/42-philosophers-tester

### AI Usage

AI tools (Claude by Anthropic) were used in this project for:

* Explaining concepts of threads, mutexes, race conditions, sleep in detail
* Reviewing and debugging the program
* Discussing the correct behaviour of our program edge cases
* Writing and iterating on test scripts to validate the program against the evaluation sheet and a community tester.
