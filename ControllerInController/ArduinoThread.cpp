#include "ArduinoThread.h"

/*****************
 * Classe Thread *
 *****************/

Scheduler::Thread::Thread(void (*callback)(void), long _interval) {
    enabled = true;
    onRun(callback);
    _cached_next_run = 0;
    last_run = 0;

    ThreadID = (int)this;

    setInterval(_interval);
}

void Scheduler::Thread::runned(long time) {
    // If less than 0, than get current ticks
    if(time < 0)
        time = millis();

    // Saves last_run
    last_run = time;

    // Cache next run
    _cached_next_run = last_run + interval;
}

void Scheduler::Thread::setInterval(long _interval) {
    // Filter intervals less than 0
    interval = (_interval < 0 ? 0 : _interval);

    // Cache the next run based on the last_run
    _cached_next_run = last_run + interval;
}

bool Scheduler::Thread::shouldRun(long time) {
    // If less than 0, than get current ticks
    if(time < 0)
        time = millis();

    // Exceeded the time limit, AND is enabled? Then should run...
    return ((time >= _cached_next_run) && enabled);
}

void Scheduler::Thread::onRun(void (*callback)(void)) {
    _onRun = callback;
}

void Scheduler::Thread::run() {
    if(_onRun != NULL)
        _onRun();

    // Update last_run and _cached_next_run
    runned();
}

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
