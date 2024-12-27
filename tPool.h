#ifndef TPOOL_H
#define TPOOL_H

#include <queue>
#include <thread>
#include <semaphore>
#include <chrono>
#include <iostream>
#include <functional>

using namespace std;

class tPool {
    public:
        // Constructor to initialize the thread pool with a given number of threads
        tPool(int numThreads);

        // Method to add work to the queue
        void addWork(std::function<void(void*)> func, void* arg);

        // Method to stop the pool and join all threads
        void stopPool();

    private:
        // Helper method that will act as the worker thread's run method
        void workerThread(int threadID);

        // Number of threads in the pool
        int numThreads;

        // The threads that make up the pool
        std::thread** threads;

        // Separate queues for functions and their parameters
        std::queue<std::function<void(void*)>>* funcQueue;  // Queue for functions
        std::queue<void*>* paramQueue;  // Queue for function arguments

        // Semaphores for managing access to the queue and synchronizing work
        std::binary_semaphore* qLock;    // Queue lock semaphore (pointer)
        std::binary_semaphore* screenLock; // Screen output semaphore (pointer)
        std::binary_semaphore* qEmpty; // Semaphore to track whether the queue is empty
        std::counting_semaphore<10000>* workAvailable; // Work availability semaphore (pointer)

        // Boolean to keep track of whether the pool is running
        bool running;
};

#endif