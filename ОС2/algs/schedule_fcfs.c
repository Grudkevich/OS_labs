#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "list.h"
#include "cpu.h"

struct node *task_list = NULL;

void add(char *name, int priority, int burst) {
    Task *new_task = malloc(sizeof(Task));
    new_task->name = name;
    new_task->priority = priority;
    new_task->burst = burst;
    new_task->tid = 0; // not used
    
    insert(&task_list, new_task);
}

Task *pick_next_task() {
    if (task_list == NULL) return NULL;
    
    struct node *temp = task_list;
    Task *next = temp->task;
    
    while (temp != NULL) {
        // FCFS: pick the first task in the list order
        // Since tasks are added in arrival order, the list is in FCFS order
        temp = temp->next;
    }
    
    return next;
}

void schedule() {
    Task *current;
    
    while (task_list != NULL) {
        current = pick_next_task();
        if (current == NULL) break;
        
        // Run the task for its full burst time
        run(current, current->burst);
        
        // Remove task from list
        delete(&task_list, current);
        free(current);
    }
}