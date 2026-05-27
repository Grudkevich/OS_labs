#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "list.h"
#include "cpu.h"

#define QUANTUM 10

struct node *task_list = NULL;
struct node *queue = NULL; // circular queue

void add(char *name, int priority, int burst) {
    Task *new_task = malloc(sizeof(Task));
    new_task->name = name;
    new_task->priority = priority;
    new_task->burst = burst;
    new_task->tid = 0;
    
    insert(&task_list, new_task);
    insert(&queue, new_task);
}

Task *pop_queue() {
    if (queue == NULL) return NULL;
    
    struct node *first = queue;
    Task *task = first->task;
    
    // Remove first node
    queue = queue->next;
    free(first);
    
    return task;
}

void push_queue(Task *task) {
    insert(&queue, task);
}

int is_task_list_empty() {
    return task_list == NULL;
}

void schedule() {
    Task *current;
    int time_slice;
    
    while (task_list != NULL) {
        if (queue == NULL) break;
        
        current = pop_queue();
        if (current == NULL) break;
        
        time_slice = (current->burst < QUANTUM) ? current->burst : QUANTUM;
        run(current, time_slice);
        current->burst -= time_slice;
        
        if (current->burst > 0) {
            // Task not finished, add back to queue
            push_queue(current);
        } else {
            // Task finished, remove from task list
            delete(&task_list, current);
            free(current);
        }
    }
}