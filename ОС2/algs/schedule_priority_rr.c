#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "list.h"
#include "cpu.h"

#define QUANTUM 10

struct node *task_list = NULL;

// Array of queues for each priority level (1-10)
struct node *priority_queues[11]; // index 1..10

void add(char *name, int priority, int burst) {
    Task *new_task = malloc(sizeof(Task));
    new_task->name = name;
    new_task->priority = priority;
    new_task->burst = burst;
    new_task->tid = 0;
    
    insert(&task_list, new_task);
    
    // Add to appropriate priority queue
    if (priority >= 1 && priority <= 10) {
        insert(&priority_queues[priority], new_task);
    }
}

Task *pop_queue(int priority) {
    if (priority_queues[priority] == NULL) return NULL;
    
    struct node *first = priority_queues[priority];
    Task *task = first->task;
    
    priority_queues[priority] = priority_queues[priority]->next;
    free(first);
    
    return task;
}

void push_queue(int priority, Task *task) {
    insert(&priority_queues[priority], task);
}

int is_task_finished(Task *task) {
    return task->burst <= 0;
}

void schedule() {
    Task *current;
    int time_slice;
    int priority_level;
    
    while (task_list != NULL) {
        // Find highest priority non-empty queue
        priority_level = -1;
        for (int p = 10; p >= 1; p--) {
            if (priority_queues[p] != NULL) {
                priority_level = p;
                break;
            }
        }
        
        if (priority_level == -1) break;
        
        // Pop from highest priority queue
        current = pop_queue(priority_level);
        if (current == NULL) continue;
        
        // Round robin within same priority level
        time_slice = (current->burst < QUANTUM) ? current->burst : QUANTUM;
        run(current, time_slice);
        current->burst -= time_slice;
        
        if (current->burst > 0) {
            // Not finished, push back to same priority queue
            push_queue(priority_level, current);
        } else {
            // Finished, remove from task list
            delete(&task_list, current);
            free(current);
        }
    }
}