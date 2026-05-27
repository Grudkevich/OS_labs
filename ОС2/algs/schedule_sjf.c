#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "task.h"
#include "list.h"
#include "cpu.h"

struct node *task_list = NULL;

void add(char *name, int priority, int burst) {
    Task *new_task = malloc(sizeof(Task));
    new_task->name = name;
    new_task->priority = priority;
    new_task->burst = burst;
    new_task->tid = 0;
    
    insert(&task_list, new_task);
}

Task *pick_next_task() {
    if (task_list == NULL) return NULL;
    
    struct node *temp = task_list;
    Task *shortest = temp->task;
    int min_burst = temp->task->burst;
    
    temp = temp->next;
    while (temp != NULL) {
        if (temp->task->burst < min_burst) {
            min_burst = temp->task->burst;
            shortest = temp->task;
        }
        temp = temp->next;
    }
    
    return shortest;
}

void schedule() {
    Task *current;
    
    while (task_list != NULL) {
        current = pick_next_task();
        if (current == NULL) break;
        
        run(current, current->burst);
        delete(&task_list, current);
        free(current);
    }
}