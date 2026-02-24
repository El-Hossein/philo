# 🍝 Philosophers – 42 Project

Implementation of the **Dining Philosophers Problem** using:

- 🧵 Threads & Mutexes (`philo`)
- 🔁 Processes & Semaphores (`philo_bonus`)

This project is part of the 42 School curriculum and focuses on concurrency, synchronization, and race condition handling.

---

## 📌 Problem Overview

The Dining Philosophers Problem is a classic concurrency problem:

- A number of philosophers sit at a round table.
- There is one fork between each philosopher.
- A philosopher must take **two forks** to eat.
- Philosophers alternate between:
  - 🍽 Eating
  - 😴 Sleeping
  - 🤔 Thinking
- If a philosopher doesn't eat within `time_to_die`, they die.

The goal is to:

- Avoid **deadlocks**
- Avoid **race conditions**
- Correctly detect **death**
- Properly synchronize output

---

## 📂 Project Structure

```
.
├── philo
│   ├── Makefile
│   ├── main.c
│   ├── philo.h
│   ├── utils.c
│   ├── utils_1.c
│   ├── utils_2.c
│   └── utils_3.c
└── philo_bonus
    ├── Makefile
    ├── main_bonus.c
    ├── philo_bonus.h
    ├── utils_bonus.c
    ├── utils_bonus_1.c
    └── utils_bonus_2.c
```

---

## 🧵 Mandatory Part – Threads Version (`philo`)

### Concepts Used

- `pthread_create`
- `pthread_join`
- `pthread_mutex_init`
- `pthread_mutex_lock`
- `pthread_mutex_unlock`
- `gettimeofday`

### Synchronization

- Each fork is protected by a **mutex**
- A mutex protects:
  - Printing
  - Death state
  - Eating time
- A monitoring loop checks:
  - If a philosopher died
  - If all philosophers ate the required number of times

---

## 🔁 Bonus Part – Processes Version (`philo_bonus`)

### Concepts Used

- `fork`
- `kill`
- `sem_open`
- `sem_wait`
- `sem_post`
- `sem_unlink`

### Synchronization

- Forks are handled using a **counting semaphore**
- Printing is protected with a semaphore
- Death detection triggers:
  - Posting to `dead` semaphore
  - Killing all child processes

---

## ⚙️ Compilation

### Mandatory

```bash
cd philo
make
```

### Bonus

```bash
cd philo_bonus
make
```

---

## ▶️ Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Example

```bash
./philo 5 800 200 200
```

- 5 philosophers
- 800ms to die
- 200ms to eat
- 200ms to sleep

### Optional

```bash
./philo 5 800 200 200 7
```

Each philosopher must eat 7 times.

---

## 🧠 Key Implementation Details

### Deadlock Avoidance

- Even-numbered philosophers start slightly delayed.
- Forks are picked in a consistent order.

### Death Detection

- Each philosopher stores `last_eating_time`.
- The monitor checks: `current_time - last_eating_time > time_to_die`
- If true → simulation stops.

### Thread Safety

- All shared data is protected with mutexes/semaphores.
- No data races.
- Output is synchronized.

---

## 🚀 What I Learned

- Multithreading fundamentals
- Process management
- Mutex vs Semaphore differences
- Handling race conditions
- Avoiding deadlocks
- Writing safe concurrent code

---

## 👤 Author

**eel-ghal**  
42 Network Student