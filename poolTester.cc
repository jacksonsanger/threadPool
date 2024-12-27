// Compile as: g++ poolTester.cc tPool.cc -std=c++20 -pthread -o poolTester
// Note the 2020 standard is necesarry to use this version of semaphores

#include <iostream>
#include "tPool.h"

typedef struct fooParm {
    int id;
    int waitTime;
} fooParm;

void foo(void* parm) {
    fooParm* p = (fooParm*)parm; // downcast

    // can't get the screenLock from the tPool class here
    //  so just put it together into one string before printing
    std::string output = "  Foo called with id " + std::to_string(p->id) + "\n";
    std::cout << output;
    this_thread::sleep_for(chrono::milliseconds(p->waitTime));
}

int main() {
    // Initialize the random seed
    //srand(time(NULL));  // each run will be different
    srand(0);  // each run will be the same - good for testing


    tPool* tp = new tPool(4);

    for (int i=0; i<20; i++) {
        // Slight pause before adding the next piece of work
        this_thread::sleep_for(chrono::milliseconds(rand()%1000));

        // Ask the thread pool to do the work when it gets a chance
        tp->addWork(foo, new fooParm{i, rand()%10000});
    }
    tp->stopPool();
}