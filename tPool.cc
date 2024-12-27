#include "tPool.h"
#include <iostream>
#include <semaphore>
using namespace std;

// Constructor to initialize the thread pool with a given number of threads
tPool::tPool(int numThreads) {
    this->numThreads = numThreads;  // Assign the numThreads value
    this->running = true;           // Set the running flag to true

    // Allocate memory for semaphores and queue
    qLock = new std::binary_semaphore(1);       // Semaphore for queue locking
    screenLock = new std::binary_semaphore(1);  // Semaphore for screen output locking
    qEmpty = new std::binary_semaphore(1);      // Semaphore to track whether the queue is empty
    workAvailable = new std::counting_semaphore<10000>(0);  // Initially, no work is available

    // Allocate memory for the work queues
    funcQueue = new std::queue<std::function<void(void*)>>();
    paramQueue = new std::queue<void*>();

    // Create and start the thread pool
    threads = new std::thread*[numThreads];  // Allocate space for threads
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = new std::thread(&tPool::workerThread, this, i);  // Create thread and start worker
    }
}


// Method to add work to the queue
void tPool::addWork(std::function<void(void*)> func, void* arg) {
    // Lock the queue for thread-safe access
    qLock->acquire();

    // Add the function to the function queue
    funcQueue->push(func);

    // Add the parameter to the parameter queue
    paramQueue->push(arg);

    // Release qEmpty if the queue was empty
    if (funcQueue->size() == 1) {  // The queue was previously empty
        qEmpty->acquire();  // Acquire qEmpty to signal it's no longer empty
    }

    // Unlock the queue
    qLock->release();

    // Signal that work is available by releasing a token
    workAvailable->release();
}


// Helper method that will act as the worker thread's run method
void tPool::workerThread(int threadID) {
    // Print when the worker starts
    screenLock->acquire();
    std::cout << "Worker " << threadID << " started" << std::endl;
    screenLock->release();
    while (running) {
        // Wait for work to be available (blocks if there's no work)
        workAvailable->acquire();

        // Lock the queue to get the next piece of work
        qLock->acquire();
        
        // Pop from both queues
        std::function<void(void*)> func = funcQueue->front();
        void* arg = paramQueue->front();
        funcQueue->pop();
        paramQueue->pop();

        // Check if the queue is now empty after removing the work
        if (funcQueue->empty()) {
            qEmpty->release();  // Release qEmpty to signal the queue is empty
        }

        qLock->release();  // Unlock the queue

        // Process the work by calling the function with its argument
        screenLock->acquire();
        std::cout << "Worker " << threadID << " about to do work" << std::endl;
        screenLock->release();

        // Execute the function with the passed argument
        func(arg);
    }
}

// Method to stop the pool and join all threads
void tPool::stopPool() {
    // First, wait for the queue to be empty
    qEmpty->acquire();  // This will block until the queue is empty

    // Now that the queue is empty, stop accepting new work
    running = false;

    // Wake up any threads that are sleeping by releasing workAvailable tokens
    // We release numThreads tokens to wake up all the worker threads
    for (int i = 0; i < numThreads; ++i) {
        workAvailable->release();  // Wake up the sleeping threads
    }

    // Join all worker threads to ensure they've stopped
    for (int i = 0; i < numThreads; ++i) {
        threads[i]->join();
    }
}




