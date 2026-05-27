/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

struct data
{
    int a;
    int b;
    int task_id;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(void)
{
    struct data tasks[10];
    
    for (int i = 0; i < 10; i++) {
        tasks[i].a = i * 10;
        tasks[i].b = i * 5;
        tasks[i].task_id = i;
    }
    
    printf("Initializing thread pool with 3 threads...\n");
    pool_init();
    
    printf("Submitting 10 tasks to the pool...\n\n");
    
    for (int i = 0; i < 10; i++) {
        pool_submit(&add, &tasks[i]);
        printf("Task %d submitted\n", i);
    }
    
    pool_shutdown();
    
    printf("\nAll tasks done!\n");
    
    return 0;
}
