#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "segel.h"

// Function to create a new task
Task* createTask(int data, struct timeval *arrival) {
    Task* newTask = (Task*)malloc(sizeof(Task));
    if (!newTask) {
        printf("Memory error\n");
        return NULL;
    }
    newTask->data = data;
    if (arrival != NULL){
        newTask->_arrival = *arrival;
        newTask->arrival = &newTask->_arrival;
    }else{
        newTask->arrival = NULL;
    }
    newTask->next = NULL;
    newTask->prev = NULL;
    return newTask;
}

// Function to create a queue
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        printf("Memory error\n");
        return NULL;
    }
    queue->front = queue->rear = NULL;
    queue->size = 0;
    return queue;
}

// Function to enqueue an element to the queue
Task* enqueue(Queue* queue, int data, struct timeval *arrival) {
    Task* newTask = createTask(data, arrival);
    if (!newTask) return NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = newTask;
    } else {
        queue->rear->next = newTask;
        newTask->prev = queue->rear;
        queue->rear = newTask;
    }
    queue->size++;
    return newTask;
}


// Function to dequeue an element from the queue
int dequeue(Queue* queue) {
    if (queue->front == NULL) {
        printf("Queue is empty\n");
        return -1;
    }
    Task* temp = queue->front;
    int data = temp->data;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    } else {
        queue->front->prev = NULL;
    }
    free(temp);
    queue->size--;
    return data;
}

// Function to get the size of the queue
int size(Queue* queue) {
    return queue->size;
}

// Function to remove a task from the middle of the queue
void removeTask(Queue* queue, Task* task) {
    if (queue->front == NULL || task == NULL) return;
    if (task == queue->front) {
        dequeue(queue);
        return;
    }
    if (task == queue->rear) {
        queue->rear = task->prev;
        queue->rear->next = NULL;
        free(task);
        queue->size--;
        return;
    }
    task->prev->next = task->next;
    task->next->prev = task->prev;
    free(task);
    queue->size--;
}


// Function to pop the last element from the queue
int popLast(Queue* queue) {
    if (queue->rear == NULL) {
        printf("Queue is empty\n");
        return -1;
    }
    Task* temp = queue->rear;
    int data = temp->data;
    queue->rear = queue->rear->prev;
    if (queue->rear == NULL) {
        queue->front = NULL;
    } else {
        queue->rear->next = NULL;
    }
    free(temp);
    queue->size--;
    return data;
}

// Function to print the queue
void printQueue(Queue* queue) {
    Task* temp = queue->front;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

// // Example usage
// int main() {
//     Queue* queue = createQueue();
//     Task* task1 = enqueue(queue, 10);
//     Task* task2 = enqueue(queue, 20);
//     Task* task3 = enqueue(queue, 30);
//     printf("Queue: ");
//     printQueue(queue);
    
//     printf("Dequeued: %d\n", dequeue(queue));
//     printf("Queue: ");
//     printQueue(queue);
    
//     removeTask(queue, task2);
//     printf("Queue after removing middle task: ");
//     printQueue(queue);
    
//     printf("Size of the queue: %d\n", size(queue));
//     return 0;
// }
