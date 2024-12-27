# Thread Pool Implementation in C++

This project demonstrates the implementation of a thread pool in C++ using semaphores for synchronization.

## Features

- **Thread Pool Management**: Maintains a configurable number of threads that handle tasks from a shared work queue.
- **Task Queue**: Supports an unbounded queue for pending tasks, ensuring no task is dropped even when all threads are busy.
- **Semaphore-Based Synchronization**: Utilizes semaphores to safely manage access to shared resources and coordinate thread operations.
- **Graceful Shutdown**: Ensures all tasks in the queue are processed before the thread pool is stopped.

## Technical Skills Demonstrated

- **Multithreading**: Efficiently manages multiple threads for concurrent task execution.
- **Semaphores**: Used for synchronization between producer (main thread) and consumer (worker threads).
- **C++ STL**: Leveraged `std::thread`, `std::function`, and smart pointers for thread and task management.
- **Design Patterns**: Implemented a producer-consumer model to manage work distribution across threads.
- **Memory Management**: Utilized heap-allocated structures for thread-safe data sharing.

## Implementation Details

- **tPool Class**:
  - Constructor: Initializes the thread pool with a specified number of threads.
  - `addWork`: Adds a task to the work queue, taking a function and a parameter.
  - `stopPool`: Gracefully stops all threads after completing all tasks in the queue.
  - Worker Threads: Consume tasks from the queue and execute them, returning to idle state afterward.

- **poolTester.cc**:
  - Demonstrates the functionality of the thread pool by creating a pool of 4 threads.
  - Adds 20 tasks to the queue and ensures all tasks are processed before the program exits.

## How It Works

1. **Initialization**:
   - The thread pool is initialized with a specified number of threads.
   - Each thread executes a `workerThread` function that continuously checks for tasks in the work queue.

2. **Adding Tasks**:
   - Tasks are added to the queue using the `addWork` method, which pushes a function and its parameters onto the queue.

3. **Processing Tasks**:
   - Idle threads pick up tasks from the queue and execute them. Once a task is completed, the thread returns to the pool for the next task.

4. **Stopping the Pool**:
   - The `stopPool` method ensures all tasks are completed before gracefully shutting down the threads.

## How to Run

1. **Build**: Compile the project using your preferred C++ compiler, ensuring all dependencies are included.
2. **Execute**: Run the resulting executable to observe the thread pool in action.


## Future Improvements

- Add support for prioritizing tasks in the queue.
- Implement dynamic thread pool resizing based on workload.
