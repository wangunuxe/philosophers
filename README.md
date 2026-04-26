# philosophers

> A 42 School concurrency project — the classic Dining Philosophers problem implemented with threads and mutexes.

---

## 📋 Overview

**philosophers** is a simulation of the Dining Philosophers problem. Each philosopher sits at a round table, alternating between eating, thinking, and sleeping. Forks are shared between adjacent philosophers, requiring synchronization to avoid data races and deadlocks.

### Rules
- Each philosopher is a **thread**
- Each fork is protected by a **mutex**
- A philosopher must pick up both adjacent forks to eat
- If a philosopher hasn't eaten within `time_to_die` milliseconds, they die
- The simulation stops when a philosopher dies or all philosophers have eaten `n` times

---

## 📁 File Structure

```
philosophers/
├── Makefile
├── include/
│   └── philo.h
└── source/
    ├── main.c
    ├── init.c
    ├── parsing.c
    ├── philosopher.c
    ├── grim_reaper.c
    ├── exit.c
    ├── output.c
    └── time.c
```

### File breakdown

| File | Role |
|------|------|
| `main.c` | Entry point, program launch |
| `init.c` | Initialization of philosophers, forks, and mutexes |
| `parsing.c` | Argument validation and parsing |
| `philosopher.c` | Philosopher thread routine |
| `grim_reaper.c` | Death monitoring logic |
| `output.c` | Thread-safe state logging |
| `time.c` | Time utility functions |
| `exit.c` | Cleanup and resource freeing |
| `philo.h` | Shared structs, prototypes, and includes |

---

## 🔧 Usage

```bash
# Compile
make

# Run
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat (optional)]
```

### Example

```bash
./philo 5 800 200 200
./philo 4 410 200 200 5
```

---

*42 Paris — philosophers*
