*This project has been created as part of the 42 curriculum by nmagro-r.*

# Philosophers

The dining philosophers problem, solved in C with POSIX threads and mutexes. Each philosopher is a thread, each fork is a mutex, and one more thread watches over the table.

## Description

A group of philosophers sit around a table with one fork between each pair. To eat, a philosopher needs both neighbouring forks; then they sleep, think and try again. If one goes too long without eating, they die and the simulation ends.

The problem looks simple, but it is a compact exercise in the two classic failures of concurrent code: **deadlocks**, where everybody waits forever, and **data races**, where threads read and write the same memory without coordination.

## Instructions

Requires a POSIX system, `make` and a C compiler. Built and tested with GCC and Clang on Linux.

```bash
make
./philo 5 800 200 200
```

```
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [meals]
```

Times are in milliseconds. `meals` is optional: the number of times each philosopher must eat before the simulation stops. All values must be positive integers, with a maximum of 200 philosophers.

The output is one timestamped line per event:

```
0 1 has taken a fork
1 1 has taken a fork
1 1 is eating
1 3 has taken a fork
1 3 has taken a fork
1 3 is eating
1 5 has taken a fork
201 3 is sleeping
```

Some cases worth trying:

| Command | What happens |
|---|---|
| `./philo 1 800 200 200` | There is only one fork, so the philosopher dies |
| `./philo 5 800 200 200` | Nobody dies; stop it with `Ctrl+C` |
| `./philo 5 800 200 200 7` | Everyone eats exactly 7 times and the simulation stops |
| `./philo 4 410 200 200` | Nobody dies, with a margin of only 10 ms |
| `./philo 4 310 200 100` | There is not enough time, and someone dies at about 310 ms |

With margins that small, a stall on a very busy machine can still cost a philosopher their life.

## How it works

**Avoiding deadlock.** A deadlock needs a circle in which everyone holds one fork and waits for the next. Here odd philosophers take their right fork first and even philosophers take their left fork first, so that circle cannot form.

**Keeping it fair.** Avoiding deadlock is not enough, because someone can still starve while others eat twice. Even philosophers start a little later, so that the odd ones eat first. And with an odd number of philosophers, nobody can eat more often than once every three meal times, so after eating they wait a moment and let the others go first.

**Protecting shared data.** Every piece of shared state has its own mutex:

| Mutex | Protects |
|---|---|
| One per fork | The fork itself |
| `meal_lock`, one per philosopher | Time of the last meal and number of meals |
| `dead_lock` | The "someone has died" flag |
| `eated_lock` | The "everyone has eaten enough" flag |
| `write_lock` | Terminal output, so lines never get mixed |

**The waiter.** A separate thread loops with a pause of 100 microseconds, checking whether anyone has starved and whether everyone has eaten enough. When either happens it raises a flag, and the philosophers check that flag before each action and before printing, so nothing appears after a death. They also wait in slices of 100 microseconds rather than all at once, so they can stop quickly. `meal_lock` is only held to update the meal data, never during the meal itself, so the waiter is never left waiting while someone eats.

**Checking for races.** `make tsan` builds the program with ThreadSanitizer. It reported no data races in the test runs, including one with 200 philosophers.

## Resources

### References

- [Dining philosophers problem on Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [pthread_mutex_lock manual page](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [ThreadSanitizer documentation](https://github.com/google/sanitizers/wiki/ThreadSanitizerCppManual)

### AI Usage

AI assistants were used for two tasks, always with me reviewing and testing the result:

- **Bug fixing:** how the waiter detects deaths and how the simulation ends, the wait that keeps tables with an odd number of philosophers fair, and making ThreadSanitizer an optional build.
- **Documentation:** this README, checked against the code.

The design decisions were mine, and I can explain every part of the code.
