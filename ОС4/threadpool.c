/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task work_queue[QUEUE_SIZE];
int queue_front;
int queue_rear;
int queue_count;

// synchronization primitives
pthread_mutex_t queue_mutex;
sem_t queue_sem;

// thread management
pthread_t workers[NUMBER_OF_THREADS];
int shutdown_flag;

// insert a task into the queue
// returns 0 if successful or 1 otherwise
int enqueue(task t) 
{
    if (queue_count >= QUEUE_SIZE) {
        return 1; // queue is full
    }
    
    work_queue[queue_rear] = t;
    queue_rear = (queue_rear + 1) % QUEUE_SIZE;
    queue_count++;
    
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    task t = work_queue[queue_front];
    queue_front = (queue_front + 1) % QUEUE_SIZE;
    queue_count--;
    return t;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    while (1) {
        // wait for a task to be available
        sem_wait(&queue_sem);
        
        // check for shutdown signal
        pthread_mutex_lock(&queue_mutex);
        if (shutdown_flag && queue_count == 0) {
            pthread_mutex_unlock(&queue_mutex);
            pthread_exit(NULL);
        }
        
        // get the task from the queue
        task my_task = dequeue();
        pthread_mutex_unlock(&queue_mutex);
        
        // execute the task
        execute(my_task.function, my_task.data);
    }
    
    pthread_exit(NULL);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    task new_task;
    new_task.function = somefunction;
    new_task.data = p;
    
    pthread_mutex_lock(&queue_mutex);
    
    if (enqueue(new_task) != 0) {
        pthread_mutex_unlock(&queue_mutex);
        return 1; // queue full
    }
    
    pthread_mutex_unlock(&queue_mutex);
    
    // signal that a new task is available
    sem_post(&queue_sem);
    
    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    queue_front = 0;
    queue_rear = 0;
    queue_count = 0;
    shutdown_flag = 0;
    
    pthread_mutex_init(&queue_mutex, NULL);
    sem_init(&queue_sem, 0, 0);
    
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_create(&workers[i], NULL, worker, NULL);
    }
}

// shutdown the thread pool
void pool_shutdown(void)
{
    pthread_mutex_lock(&queue_mutex);
    shutdown_flag = 1;
    pthread_mutex_unlock(&queue_mutex);
    
    // wake up all threads so they can exit
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        sem_post(&queue_sem);
    }
    
    // wait for all threads to finish
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }
    
    pthread_mutex_destroy(&queue_mutex);
    sem_destroy(&queue_sem);
}