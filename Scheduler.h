#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MAX_THREADS		15

#include <Arduino.h>

class SchedulerClass {

private:

    /*****************
     * Classe Thread *
     *****************/
    class Thread {

        // Desired interval between runs
        long interval;

        // Last runned time in Ms
        long last_run;

        // Scheduled run in Ms (MUST BE CACHED)
        long _cached_next_run;

        // If the current Thread is enabled or not
        bool enabled;

        // ID of the Thread (initialized from memory adr.)
        unsigned int ThreadID;


    public:

        /*
            IMPORTANT! Run after all calls to run()
            Updates last_run and cache next run.
            NOTE: This MUST be called if extending
            this class and implementing run() method
        */

        Thread(void (*callback)(void) = NULL, long _interval = 0);

        void runned(long time = -1);

        // Callback for run() if not implemented
        void (*_onRun)(void);

        // Set the desired interval for calls, and update _cached_next_run
        void setInterval(long _interval);

        // Return if the Thread should be runned or not
        bool shouldRun(long time = -1);

        // Callback set
        void onRun(void (*callback)(void));

        // Runs Thread
        //[REVISAR]
        void run();

        bool getEnabled() const { return enabled; }

        unsigned int getThreadID() const { return ThreadID; }

    };

    /********************
     * Classe Scheduler *
     ********************/

    Thread* thread[MAX_THREADS];
    unsigned int cached_size;

public:

    SchedulerClass();

    // run() Method is overrided
    //[REVISAR]
    void run();

    // Adds a thread in the first available slot (remove first)
    // Returns if the Thread could be added or not
    //bool add(Thread* _thread);
    unsigned int createThread(void (*callback)(void), long _interval) {
        Thread *t = new Thread(callback, _interval);
        thread[cached_size] = t;
        cached_size++;
        delete t;
        return thread[cached_size-1]->getThreadID();
    }

    // remove the thread (given the Thread* or ThreadID)
    void remove(unsigned int _id);

    // Removes all threads
    void clear();

    // Return the quantity of Threads
    unsigned int size(bool cached = true);

    // Return the I Thread on the array
    // Returns NULL if none found
    //[COMO ASSIM RETORNA UMA THREAD?]
    //Thread* get(unsigned int index);

};

extern SchedulerClass Scheduler;

#endif // SCHEDULER_H
