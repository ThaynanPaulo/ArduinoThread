#include "Scheduler.h"

SchedulerClass Scheduler;

/********************
 * Classe Scheduler *
 ********************/

SchedulerClass::SchedulerClass() {
    cached_size = 0;
    clear();
}

/*
    ThreadController run() (cool stuf)
*/
void SchedulerClass::run() {
    long time = millis();
    unsigned int checks = 0;
    for(unsigned int i = 0; i < MAX_THREADS && checks <= cached_size; i++){
        // Object exists? Is enabled? Timeout exceeded?
        if(thread[i]){
            checks++;
            if(thread[i]->shouldRun(time)){
                thread[i]->run();
            }
        }
    }
}

/*
    List controller (boring part)
*/

void SchedulerClass::remove(unsigned int id){
    // Find Threads with the id, and removes
    for(unsigned int i = 0; i < MAX_THREADS; i++){
        if(thread[i]->getThreadID() == id){
            thread[i] = NULL;
            cached_size--;
            return;
        }
    }
}

void SchedulerClass::clear(){
    for(unsigned int i = 0; i < MAX_THREADS; i++){
        thread[i] = NULL;
    }
    cached_size = 0;
}

unsigned int SchedulerClass::size(bool cached){
    if(cached)
        return cached_size;

    int size = 0;
    for(int i = 0; i < MAX_THREADS; i++){
        if(thread[i])
            size++;
    }
    cached_size = size;

    return cached_size;
}