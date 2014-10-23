#include "Scheduler.h"

/********************
 * Classe Scheduler *
 ********************/

Scheduler::Scheduler() {
    cached_size = 0;

    clear();
}

/*
    ThreadController run() (cool stuf)
*/
void Scheduler::run() {
    /* [isto nao eh uma thread de usuario]
     * // Run this thread before
    if(_onRun != NULL)
        _onRun();
    */
    long time = millis();
    unsigned int checks = 0;
    for(int i = 0; i < MAX_THREADS && checks <= cached_size; i++){
        // Object exists? Is enabled? Timeout exceeded?
        if(thread[i]){
            checks++;
            if(thread[i]->shouldRun(time)){
                thread[i]->run();
            }
        }
    }
    /* [essa versao nao extende Thread]
    // ThreadController extends Thread, so we should flag as runned thread
    runned();
    */
}

/*
    List controller (boring part)
*/
/*unsigned int createThread(void (*callback)(void), long _interval) {

    Scheduler::Thread t = Scheduler::Thread(callback, _interval);

    t[cached_size] = t;

    cached_size++;

    return thread[cached_size-1].getThreadID();
}*/

void Scheduler::remove(unsigned int id){
    // Find Threads with the id, and removes
    for(unsigned int i = 0; i < MAX_THREADS; i++){
        if(thread[i]->getThreadID() == id){
            thread[i] = NULL;
            cached_size--;
            return;
        }
    }
}

void Scheduler::clear(){
    for(unsigned int i = 0; i < MAX_THREADS; i++){
        thread[i] = NULL;
    }
    cached_size = 0;
}

unsigned int Scheduler::size(bool cached){
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

/*Thread* Scheduler::get(unsigned int index){
    int pos = -1;
    for(int i = 0; i < MAX_THREADS; i++){
        if(thread[i] != NULL){
            pos++;

            if(pos == index)
                return thread[i];
        }
    }

    return NULL;
}
*/